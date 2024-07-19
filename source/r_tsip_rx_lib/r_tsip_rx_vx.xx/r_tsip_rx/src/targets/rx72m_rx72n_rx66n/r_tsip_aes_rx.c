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
 * File Name    : r_tsip_aes_rx.c
 * Version      : 1.21
 * Description  : Interface definition for the r_tsip_aes_rx TSIP module.
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
#include <string.h>
#include "r_tsip_rx_private.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/* GCM related */
#define GCM_IV_MAX_BYTE_SIZE    (12)
#define GCM_INPUT_DATA_TEXT     (1)
#define GCM_INPUT_DATA_AAD      (0)
/* Initialization function call state */
#define CALL_ONLY_INIT          (0)
#define CALL_ONLY_UPDATE_FINAL  (1)

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
extern uint32_t g_private_id_counter;
#if TSIP_AES_128_ECB_ENCRYPT == 1
uint32_t g_aes128ecbenc_private_id;
#endif
#if TSIP_AES_128_ECB_DECRYPT == 1
uint32_t g_aes128ecbdec_private_id;
#endif
#if TSIP_AES_256_ECB_ENCRYPT == 1
uint32_t g_aes256ecbenc_private_id;
#endif
#if TSIP_AES_256_ECB_DECRYPT == 1
uint32_t g_aes256ecbdec_private_id;
#endif
#if TSIP_AES_128_CBC_ENCRYPT == 1
uint32_t g_aes128cbcenc_private_id;
#endif
#if TSIP_AES_128_CBC_DECRYPT == 1
uint32_t g_aes128cbcdec_private_id;
#endif
#if TSIP_AES_256_CBC_ENCRYPT == 1
uint32_t g_aes256cbcenc_private_id;
#endif
#if TSIP_AES_256_CBC_DECRYPT == 1
uint32_t g_aes256cbcdec_private_id;
#endif
#if TSIP_AES_128_CTR == 1
uint32_t g_aes128ctr_private_id;
#endif
#if TSIP_AES_256_CTR == 1
uint32_t g_aes256ctr_private_id;
#endif
#if TSIP_AES_128_CMAC == 1
uint32_t g_aes128cmacgen_private_id;
uint32_t g_aes128cmacver_private_id;
#endif
#if TSIP_AES_256_CMAC == 1
uint32_t g_aes256cmacgen_private_id;
uint32_t g_aes256cmacver_private_id;
#endif
#if TSIP_AES_128_GCM_ENCRYPT == 1
uint32_t g_aes128gcmenc_private_id;
#endif
#if TSIP_AES_128_GCM_DECRYPT == 1
uint32_t g_aes128gcmdec_private_id;
#endif
#if TSIP_AES_256_GCM_ENCRYPT == 1
uint32_t g_aes256gcmenc_private_id;
#endif
#if TSIP_AES_256_GCM_DECRYPT == 1
uint32_t g_aes256gcmdec_private_id;
#endif
#if TSIP_AES_128_CCM_ENCRYPT == 1
uint32_t g_aes128ccmenc_private_id;
#endif
#if TSIP_AES_128_CCM_DECRYPT == 1
uint32_t g_aes128ccmdec_private_id;
#endif
#if TSIP_AES_256_CCM_ENCRYPT == 1
uint32_t g_aes256ccmenc_private_id;
#endif
#if TSIP_AES_256_CCM_DECRYPT == 1
uint32_t g_aes256ccmdec_private_id;
#endif

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
static e_tsip_err_t prepare_gcm_iv(uint8_t *ivec, uint32_t ivec_len, tsip_aes_key_index_t *key_index,
        uint32_t key_index_word_size, uint32_t *hashed_ivec);

/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateAes128KeyIndex
*******************************************************************************************************************/ /**
* @details       The API for outputting User Key Generation Information of AES128 bit.
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information (13 words) of AES128 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateAes128KeyIndexSub()
* @note          None
*/
TSIP_SEC_P_SECURE_BOOT_ERASE
e_tsip_err_t R_TSIP_GenerateAes128KeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv, uint8_t *encrypted_key,
        tsip_aes_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateAes128KeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_AES128;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/******************************************
 End of function R_TSIP_GenerateAes128KeyIndex
 ******************************************/
TSIP_SEC_DEFAULT

/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateAes256KeyIndex
*******************************************************************************************************************/ /**
* @details       The API for outputting User Key Generation Information of AES128 bit.
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information (17 words) of AES256 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateAes256KeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateAes256KeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv, uint8_t *encrypted_key,
        tsip_aes_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateAes256KeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_AES256;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/******************************************
 End of function R_TSIP_GenerateAes256KeyIndex
 ******************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateAes128KeyIndex
*******************************************************************************************************************/ /**
* @details       The API for updating User Key Generation Information of AES128 bit.
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information (13 words) of AES128 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateAes128KeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateAes128KeyIndex(uint8_t *iv, uint8_t *encrypted_key, tsip_aes_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_UpdateAes128KeyIndexSub((uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_AES128;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/******************************************
 End of function R_TSIP_UpdateAes128KeyIndex
 ******************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateAes256KeyIndex
*******************************************************************************************************************/ /**
* @details       The API for updating User Key Generation Information of AES256 bit.
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information (17 words) of AES256 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateAes256KeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateAes256KeyIndex(uint8_t *iv, uint8_t *encrypted_key, tsip_aes_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_UpdateAes256KeyIndexSub((uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_AES256;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/******************************************
 End of function R_TSIP_UpdateAes256KeyIndex
 ******************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateAes128RandomKeyIndex
*******************************************************************************************************************/ /**
* @details       The API for outputting random User Key Generation Information of AES128 bit.
* @param[out]    key_index Output the user Key Generation Information (13 words)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @see           R_TSIP_GenerateAes128RandomKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateAes128RandomKeyIndex(tsip_aes_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    error_code = R_TSIP_GenerateAes128RandomKeyIndexSub(key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_AES128;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/***************************************
 End of function R_TSIP_GenerateAes128RandomKeyIndex
 ***************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateAes256RandomKeyIndex
*******************************************************************************************************************/ /**
* @details       The API for outputting random User Key Generation Information of AES256 bit.
* @param[out]    key_index Output the user Key Generation Information (17 words)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @see           R_TSIP_GenerateAes256RandomKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateAes256RandomKeyIndex(tsip_aes_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    error_code = R_TSIP_GenerateAes256RandomKeyIndexSub(key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_AES256;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/***************************************
 End of function R_TSIP_GenerateAes256RandomKeyIndex
 ***************************************/

#if TSIP_AES_128_ECB_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128EcbEncryptInit
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES ECB mode algorithm with AES 128 key.
* @param[out]    handle Handler for AES(work area).
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128EcbEncryptInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128EcbEncryptInit(tsip_aes_handle_t *handle, tsip_aes_key_index_t *key_index)
{
    memset(handle, 0, sizeof(tsip_aes_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes128ecbenc_private_id = g_private_id_counter;
    handle->id = g_aes128ecbenc_private_id;
    return R_TSIP_Aes128EcbEncryptInitPrivate(key_index);
}
/*******************************
 End of function R_TSIP_Aes128EcbEncryptInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128EcbEncryptUpdate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES ECB mode algorithm with AES 128 key.
* @param[in]     handle Handler for AES(work area).
* @param[in]     plain Input plain text.
* @param[out]    cipher Output cipher text.
* @param[in]     plain_length Input byte size of plain.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128EcbEncryptUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128EcbEncryptUpdate(tsip_aes_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_length)
{
    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes128ecbenc_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    /* Process of fractions using last_1_block_as_fraction is not implemented. */
    /* Casting uint32_t pointer is used for address. */
    return R_TSIP_Aes128EcbEncryptUpdatePrivate((uint32_t*)plain, (uint32_t*)cipher, plain_length >> 2);
}
/*******************************
 End of function R_TSIP_Aes128EcbEncryptUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128EcbEncryptFinal
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES ECB mode algorithm with AES 128 key.
* @param[in,out] handle Handler for AES(work area).
* @param[out]    cipher Output cipher text.
* @param[out]    cipher_length Output byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128EcbEncryptFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128EcbEncryptFinal(tsip_aes_handle_t *handle, uint8_t *cipher, uint32_t *cipher_length)
{
    e_tsip_err_t error_code;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes128ecbenc_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    *cipher_length = 0;
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_Aes128EcbEncryptFinalPrivate((uint32_t*)cipher, cipher_length);
    memset(handle, 0, sizeof(tsip_aes_handle_t));
    return error_code;
}
/*******************************
 End of function R_TSIP_Aes128EcbEncryptFinal
 *******************************/
#endif /* TSIP_AES_128_ECB_ENCRYPT == 1 */

#if TSIP_AES_128_ECB_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128EcbDecryptInit
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES ECB mode algorithm with AES 128 key.
* @param[out]    handle Handler for AES(work area).
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128EcbDecryptInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128EcbDecryptInit(tsip_aes_handle_t *handle, tsip_aes_key_index_t *key_index)
{
    memset(handle, 0, sizeof(tsip_aes_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes128ecbdec_private_id = g_private_id_counter;
    handle->id = g_aes128ecbdec_private_id;
    return R_TSIP_Aes128EcbDecryptInitPrivate(key_index);
}
/*******************************
 End of function R_TSIP_Aes128EcbDecryptInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128EcbDecryptUpdate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES ECB mode algorithm with AES 128 key.
* @param[in]     handle Handler for AES(work area).
* @param[in]     cipher Input cipher text.
* @param[out]    plain Output plain text.
* @param[in]     cipher_length Input byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128EcbDecryptUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128EcbDecryptUpdate(tsip_aes_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_length)
{
    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes128ecbdec_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    /* Process of fractions using last_1_block_as_fraction is not implemented. */
    /* Casting uint32_t pointer is used for address. */
    return R_TSIP_Aes128EcbDecryptUpdatePrivate((uint32_t*)cipher, (uint32_t*)plain, cipher_length >> 2);
}
/*******************************
 End of function R_TSIP_Aes128EcbDecryptUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128EcbDecryptFinal
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES ECB mode algorithm with AES 128 key.
* @param[in,out] handle Handler for AES(work area).
* @param[out]    plain Output plain text.
* @param[out]    plain_length Output byte size of plain.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128EcbDecryptFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128EcbDecryptFinal(tsip_aes_handle_t *handle, uint8_t *plain, uint32_t *plain_length)
{
    e_tsip_err_t error_code;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes128ecbdec_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    *plain_length = 0;
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_Aes128EcbDecryptFinalPrivate((uint32_t*)plain, plain_length);
    memset(handle, 0, sizeof(tsip_aes_handle_t));
    return error_code;
}
/*******************************
 End of function R_TSIP_Aes128EcbDecryptFinal
 *******************************/
#endif /* TSIP_AES_128_ECB_DECRYPT == 1 */

#if TSIP_AES_128_CBC_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CbcEncryptInit
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CBC mode algorithm with AES 128 key.
* @param[out]    handle Handler for AES(work area).
* @param[in]     key_index User key generation information area.
* @param[in]     ivec - Input initial vector.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128CbcEncryptInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CbcEncryptInit(tsip_aes_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *ivec)
{
    memset(handle, 0, sizeof(tsip_aes_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes128cbcenc_private_id = g_private_id_counter;
    handle->id = g_aes128cbcenc_private_id;
    /* Casting uint32_t pointer is used for address. */
    return R_TSIP_Aes128CbcEncryptInitPrivate(key_index, (uint32_t*)ivec);
}
/*******************************
 End of function R_TSIP_Aes128CbcEncryptInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CbcEncryptUpdate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CBC mode algorithm with AES 128 key.
* @param[in]     handle Handler for AES(work area).
* @param[in]     plain Input plain text.
* @param[out]    cipher Output cipher text.
* @param[in]     plain_length Input byte size of plain.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128CbcEncryptUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CbcEncryptUpdate(tsip_aes_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_length)
{
    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes128cbcenc_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    /* Process of fractions using last_1_block_as_fraction is not implemented. */
    /* Casting uint32_t pointer is used for address. */
    R_TSIP_Aes128CbcEncryptUpdatePrivate((uint32_t*)plain, (uint32_t*)cipher, plain_length >> 2);
    return TSIP_SUCCESS;
}
/*******************************
 End of function R_TSIP_Aes128CbcEncryptUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CbcEncryptFinal
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CBC mode algorithm with AES 128 key.
* @param[in,out] handle Handler for AES(work area).
* @param[out]    cipher Output cipher text.
* @param[out]    cipher_length Output byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128CbcEncryptFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CbcEncryptFinal(tsip_aes_handle_t *handle, uint8_t *cipher, uint32_t *cipher_length)
{
    e_tsip_err_t error_code;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes128cbcenc_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    *cipher_length = 0;
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_Aes128CbcEncryptFinalPrivate((uint32_t*)cipher, cipher_length);
    memset(handle, 0, sizeof(tsip_aes_handle_t));
    return error_code;
}
/*******************************
 End of function R_TSIP_Aes128CbcEncryptFinal
 *******************************/
#endif /* TSIP_AES_128_CBC_ENCRYPT == 1 */

#if TSIP_AES_128_CBC_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CbcDecryptInit
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES CBC mode algorithm with AES 128 key.
* @param[out]    handle Handler for AES(work area).
* @param[in]     key_index User key generation information area.
* @param[in]     ivec Input initial vector.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128CbcDecryptInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CbcDecryptInit(tsip_aes_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *ivec)
{
    memset(handle, 0, sizeof(tsip_aes_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes128cbcdec_private_id = g_private_id_counter;
    handle->id = g_aes128cbcdec_private_id;
    /* Casting uint32_t pointer is used for address. */
    return R_TSIP_Aes128CbcDecryptInitPrivate(key_index, (uint32_t*)ivec);
}
/*******************************
 End of function R_TSIP_Aes128CbcDecryptInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CbcDecryptUpdate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES CBC mode algorithm with AES 128 key.
* @param[in]     handle Handler for AES(work area).
* @param[in]     cipher Input cipher text.
* @param[out]    plain Output plain text.
* @param[in]     cipher_length Input byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128CbcDecryptUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CbcDecryptUpdate(tsip_aes_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_length)
{
    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes128cbcdec_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    /* Process of fractions using last_1_block_as_fraction is not implemented. */
    /* Casting uint32_t pointer is used for address. */
    R_TSIP_Aes128CbcDecryptUpdatePrivate((uint32_t*)cipher, (uint32_t*)plain, cipher_length >> 2);
    return TSIP_SUCCESS;
}
/*******************************
 End of function R_TSIP_Aes128CbcDecryptUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CbcDecryptFinal
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES CBC mode algorithm with AES 128 key.
* @param[in,out] handle Handler for AES(work area).
* @param[out]    plain Output plain text.
* @param[out]    plain_length Output byte size of plain.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128CbcDecryptFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CbcDecryptFinal(tsip_aes_handle_t *handle, uint8_t *plain, uint32_t *plain_length)
{
    e_tsip_err_t error_code;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes128cbcdec_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    *plain_length = 0;
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_Aes128CbcDecryptFinalPrivate((uint32_t*)plain, plain_length);
    memset(handle, 0, sizeof(tsip_aes_handle_t));
    return error_code;
}
/*******************************
 End of function R_TSIP_Aes128CbcDecryptFinal
 *******************************/
#endif /* TSIP_AES_128_CBC_DECRYPT == 1 */

#if TSIP_AES_256_ECB_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256EcbEncryptInit
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES ECB mode algorithm with AES 256 key.
* @param[out]    handle Handler for AES(work area).
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256EcbEncryptInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256EcbEncryptInit(tsip_aes_handle_t *handle, tsip_aes_key_index_t *key_index)
{
    memset(handle, 0, sizeof(tsip_aes_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes256ecbenc_private_id = g_private_id_counter;
    handle->id = g_aes256ecbenc_private_id;
    return R_TSIP_Aes256EcbEncryptInitPrivate(key_index);
}
/*******************************
 End of function R_TSIP_Aes256EcbEncryptInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256EcbEncryptUpdate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES ECB mode algorithm with AES 256 key.
* @param[in]     handle Handler for AES(work area).
* @param[in]     plain Input plain text.
* @param[out]    cipher Output cipher text.
* @param[in]     plain_length Input byte size of plain.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256EcbEncryptUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256EcbEncryptUpdate(tsip_aes_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_length)
{
    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes256ecbenc_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    /* Process of fractions using last_1_block_as_fraction is not implemented. */
    /* Casting uint32_t pointer is used for address. */
    return R_TSIP_Aes256EcbEncryptUpdatePrivate((uint32_t*)plain, (uint32_t*)cipher, plain_length >> 2);
}
/*******************************
 End of function R_TSIP_Aes256EcbEncryptUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256EcbEncryptFinal
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES ECB mode algorithm with AES 256 key.
* @param[in,out] handle Handler for AES(work area).
* @param[out]    cipher Output cipher text.
* @param[out]    cipher_length Output byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256EcbEncryptFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256EcbEncryptFinal(tsip_aes_handle_t *handle, uint8_t *cipher, uint32_t *cipher_length)
{
    e_tsip_err_t error_code;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes256ecbenc_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    *cipher_length = 0;
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_Aes256EcbEncryptFinalPrivate((uint32_t*)cipher, cipher_length);
    memset(handle, 0, sizeof(tsip_aes_handle_t));
    return error_code;
}
/*******************************
 End of function R_TSIP_Aes256EcbEncryptFinal
 *******************************/
#endif /* TSIP_AES_256_ECB_ENCRYPT == 1 */

#if TSIP_AES_256_ECB_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256EcbDecryptInit
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES ECB mode algorithm with AES 256 key.
* @param[out]    handle Handler for AES(work area).
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256EcbDecryptInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256EcbDecryptInit(tsip_aes_handle_t *handle, tsip_aes_key_index_t *key_index)
{
    memset(handle, 0, sizeof(tsip_aes_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes256ecbdec_private_id = g_private_id_counter;
    handle->id = g_aes256ecbdec_private_id;
    return R_TSIP_Aes256EcbDecryptInitPrivate(key_index);
}
/*******************************
 End of function R_TSIP_Aes256EcbDecryptInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256EcbDecryptUpdate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES ECB mode algorithm with AES 256 key.
* @param[in]     handle Handler for AES(work area).
* @param[in]     cipher Input cipher text.
* @param[out]    plain Output plain text.
* @param[in]     cipher_length Input byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256EcbDecryptUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256EcbDecryptUpdate(tsip_aes_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_length)
{
    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes256ecbdec_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    /* Process of fractions using last_1_block_as_fraction is not implemented. */
    /* Casting uint32_t pointer is used for address. */
    return R_TSIP_Aes256EcbDecryptUpdatePrivate((uint32_t*)cipher, (uint32_t*)plain, cipher_length >> 2);
}
/*******************************
 End of function R_TSIP_Aes256EcbDecryptUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256EcbDecryptFinal
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES ECB mode algorithm with AES 128 key.
* @param[in,out] handle Handler for AES(work area).
* @param[out]    plain Output plain text.
* @param[out]    plain_length Output byte size of plain.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256EcbDecryptFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256EcbDecryptFinal(tsip_aes_handle_t *handle, uint8_t *plain, uint32_t *plain_length)
{
    e_tsip_err_t error_code;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes256ecbdec_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    *plain_length = 0;
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_Aes256EcbDecryptFinalPrivate((uint32_t*)plain, plain_length);
    memset(handle, 0, sizeof(tsip_aes_handle_t));
    return error_code;
}
/*******************************
 End of function R_TSIP_Aes256EcbDecryptFinal
 *******************************/
#endif /* TSIP_AES_256_ECB_DECRYPT == 1 */

#if TSIP_AES_256_CBC_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CbcEncryptInit
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CBC mode algorithm with AES 256 key.
* @param[out]    handle Handler for AES(work area).
* @param[in]     key_index User key generation information area.
* @param[in]     ivec - Input initial vector.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256CbcEncryptInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CbcEncryptInit(tsip_aes_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *ivec)
{
    memset(handle, 0, sizeof(tsip_aes_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes256cbcenc_private_id = g_private_id_counter;
    handle->id = g_aes256cbcenc_private_id;
    /* Casting uint32_t pointer is used for address. */
    return R_TSIP_Aes256CbcEncryptInitPrivate(key_index, (uint32_t*)ivec);
}
/*******************************
 End of function R_TSIP_Aes256CbcEncryptInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CbcEncryptUpdate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CBC mode algorithm with AES 256 key.
* @param[in]     handle Handler for AES(work area).
* @param[in]     plain Input plain text.
* @param[out]    cipher Output cipher text.
* @param[in]     plain_length Input byte size of plain.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256CbcEncryptUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CbcEncryptUpdate(tsip_aes_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_length)
{
    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes256cbcenc_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    /* Process of fractions using last_1_block_as_fraction is not implemented. */
    /* Casting uint32_t pointer is used for address. */
    R_TSIP_Aes256CbcEncryptUpdatePrivate((uint32_t*)plain, (uint32_t*)cipher, plain_length >> 2);
    return TSIP_SUCCESS;
}
/*******************************
 End of function R_TSIP_Aes256CbcEncryptUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CbcEncryptFinal
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CBC mode algorithm with AES 256 key.
* @param[in,out] handle Handler for AES(work area).
* @param[out]    cipher Output cipher text.
* @param[out]    cipher_length Output byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256CbcEncryptFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CbcEncryptFinal(tsip_aes_handle_t *handle, uint8_t *cipher, uint32_t *cipher_length)
{
    e_tsip_err_t error_code;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes256cbcenc_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    *cipher_length = 0;
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_Aes256CbcEncryptFinalPrivate((uint32_t*)cipher, cipher_length);
    memset(handle, 0, sizeof(tsip_aes_handle_t));
    return error_code;
}
/*******************************
 End of function R_TSIP_Aes256CbcEncryptFinal
 *******************************/
#endif /* TSIP_AES_256_CBC_ENCRYPT == 1 */

#if TSIP_AES_256_CBC_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CbcDecryptInit
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES CBC mode algorithm with AES 256 key.
* @param[out]    handle Handler for AES(work area).
* @param[in]     key_index User key generation information area.
* @param[in]     ivec Input initial vector.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256CbcDecryptInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CbcDecryptInit(tsip_aes_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *ivec)
{
    memset(handle, 0, sizeof(tsip_aes_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes256cbcdec_private_id = g_private_id_counter;
    handle->id = g_aes256cbcdec_private_id;
    /* Casting uint32_t pointer is used for address. */
    return R_TSIP_Aes256CbcDecryptInitPrivate(key_index, (uint32_t*)ivec);
}
/*******************************
 End of function R_TSIP_Aes256CbcDecryptInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CbcDecryptUpdate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES CBC mode algorithm with AES 256 key.
* @param[in]     handle Handler for AES(work area).
* @param[in]     cipher Input cipher text.
* @param[out]    plain Output plain text.
* @param[in]     cipher_length Input byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256CbcDecryptUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CbcDecryptUpdate(tsip_aes_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_length)
{
    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes256cbcdec_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    /* Process of fractions using last_1_block_as_fraction is not implemented. */
    /* Casting uint32_t pointer is used for address. */
    R_TSIP_Aes256CbcDecryptUpdatePrivate((uint32_t*)cipher, (uint32_t*)plain, cipher_length >> 2);
    return TSIP_SUCCESS;
}
/*******************************
 End of function R_TSIP_Aes256CbcDecryptUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CbcDecryptFinal
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES CBC mode algorithm with AES 256 key.
* @param[in,out] handle Handler for AES(work area).
* @param[out]    plain Output plain text.
* @param[out]    plain_length Output byte size of plain.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256CbcDecryptFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CbcDecryptFinal(tsip_aes_handle_t *handle, uint8_t *plain, uint32_t *plain_length)
{
    e_tsip_err_t error_code;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes256cbcdec_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    *plain_length = 0;
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_Aes256CbcDecryptFinalPrivate((uint32_t*)plain, plain_length);
    memset(handle, 0, sizeof(tsip_aes_handle_t));
    return error_code;
}
/*******************************
 End of function R_TSIP_Aes256CbcDecryptFinal
 *******************************/
#endif /* TSIP_AES_256_CBC_DECRYPT == 1 */

#if TSIP_AES_128_CTR == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CtrInit
*******************************************************************************************************************/ /**
* @details       Use AES CTR mode algorithm with AES 128 key.
* @param[out]    handle Handler for AES(work area).
* @param[in]     key_index User key generation information area.
* @param[in]     ictr Input initial counter.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128CtrInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CtrInit(tsip_aes_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *ictr)
{
    memset(handle, 0, sizeof(tsip_aes_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes128ctr_private_id = g_private_id_counter;
    handle->id = g_aes128ctr_private_id;
    /* Casting uint32_t pointer is used for address. */
    return R_TSIP_Aes128CtrInitPrivate(key_index, (uint32_t*)ictr);
}
/*******************************
 End of function R_TSIP_Aes128CtrInit
 *******************************/
#endif /* TSIP_AES_128_CTR == 1 */

#if TSIP_AES_256_CTR == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CtrInit
*******************************************************************************************************************/ /**
* @details       Use AES CTR mode algorithm with AES 256 key.
* @param[out]    handle Handler for AES(work area).
* @param[in]     key_index User key generation information area.
* @param[in]     ictr Input initial counter.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256CtrInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CtrInit(tsip_aes_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *ictr)
{
    memset(handle, 0, sizeof(tsip_aes_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes256ctr_private_id = g_private_id_counter;
    handle->id = g_aes256ctr_private_id;
    /* Casting uint32_t pointer is used for address. */
    return R_TSIP_Aes256CtrInitPrivate(key_index, (uint32_t*)ictr);
}
/*******************************
 End of function R_TSIP_Aes256CtrInit
 *******************************/
#endif /* TSIP_AES_256_CTR == 1 */

#if TSIP_AES_128_CTR == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CtrUpdate
*******************************************************************************************************************/ /**
* @details       Use AES CTR mode algorithm with AES 128 key.
* @param[in]     handle Handler for AES(work area).
* @param[in]     itext Input text.
* @param[out]    otext Output text.
* @param[in]     itext_length Input byte size of itext.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128CtrUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CtrUpdate(tsip_aes_handle_t *handle, uint8_t *itext, uint8_t *otext,
        uint32_t itext_length)
{
    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes128ctr_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    /* Casting uint32_t pointer is used for address. */
    R_TSIP_Aes128CtrUpdatePrivate((uint32_t*)itext, (uint32_t*)otext, itext_length >> 2);
    return TSIP_SUCCESS;
}
/*******************************
 End of function R_TSIP_Aes128CtrUpdate
 *******************************/
#endif /* TSIP_AES_128_CTR == 1 */

#if TSIP_AES_256_CTR == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CtrUpdate
*******************************************************************************************************************/ /**
* @details       Use AES CTR mode algorithm with AES 256 key.
* @param[in]     handle Handler for AES(work area).
* @param[in]     itext Input text.
* @param[out]    otext Output text.
* @param[in]     itext_length Input byte size of itext.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256CtrUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CtrUpdate(tsip_aes_handle_t *handle, uint8_t *itext, uint8_t *otext,
        uint32_t itext_length)
{
    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes256ctr_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    /* Casting uint32_t pointer is used for address. */
    R_TSIP_Aes256CtrUpdatePrivate((uint32_t*)itext, (uint32_t*)otext, itext_length >> 2);
    return TSIP_SUCCESS;
}
/*******************************
 End of function R_TSIP_Aes256CtrUpdate
 *******************************/
#endif /* TSIP_AES_256_CTR == 1 */

#if TSIP_AES_128_CTR == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CtrFinal
*******************************************************************************************************************/ /**
* @details       Use AES CTR mode algorithm with AES 128 key.
* @param[in,out] handle Handler for AES(work area).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128CtrFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CtrFinal(tsip_aes_handle_t *handle)
{
    e_tsip_err_t error_code;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes128ctr_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    error_code = R_TSIP_Aes128CtrFinalPrivate();
    memset(handle, 0, sizeof(tsip_aes_handle_t));
    return error_code;
}
/*******************************
 End of function R_TSIP_Aes128CtrFinal
 *******************************/
#endif /* TSIP_AES_128_CTR == 1 */

#if TSIP_AES_256_CTR == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CtrFinal
*******************************************************************************************************************/ /**
* @details       Use AES CTR mode algorithm with AES 256 key.
* @param[in,out] handle Handler for AES(work area).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256CtrFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CtrFinal(tsip_aes_handle_t *handle)
{
    e_tsip_err_t error_code;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes256ctr_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    error_code = R_TSIP_Aes256CtrFinalPrivate();
    memset(handle, 0, sizeof(tsip_aes_handle_t));
    return error_code;
}
/*******************************
 End of function R_TSIP_Aes256CtrFinal
 *******************************/
#endif /* TSIP_AES_256_CTR == 1 */

#if TSIP_AES_128_GCM_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128GcmEncryptInit
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES GCM algorithm with AES 128 key.
* @param[out]    handle Handler for AES for GCM(work area).
* @param[in]     key_index User key generation information area.
* @param[in]     ivec Input initial vector.
* @param[in]     ivec_len Input initial vector byte size.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           prepare_gcm_iv()
* @see           R_TSIP_Aes128GcmEncryptInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128GcmEncryptInit(tsip_gcm_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *ivec,
        uint32_t ivec_len)
{
    e_tsip_tls13_phase_t  tls13_phase      = TSIP_TLS13_PHASE_HANDSHAKE;
    e_tsip_tls13_mode_t   tls13_mode       = TSIP_TLS13_MODE_FULL_HANDSHAKE;
    tsip_tls13_handle_t * p_tls13_handle   = NULL;
    uint32_t hashed_ivec[4] =
    {
        0
    };
    e_tsip_err_t ercd = TSIP_SUCCESS;

    if ((TSIP_KEY_INDEX_TYPE_AES128 == key_index->type) || (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH == key_index->type))
    {
        if (0 == ivec_len)
        {
            return TSIP_ERR_PARAMETER;
        }
    }
    else
    {
        if ((TSIP_KEY_INDEX_TYPE_TLS_CLIENT_AES128_FOR_CLIENT != key_index->type)
        && (TSIP_KEY_INDEX_TYPE_TLS_SERVER_AES128_FOR_SERVER != key_index->type))
        {
            return TSIP_ERR_KEY_SET;
        }
    }

    memset(handle, 0, sizeof(tsip_gcm_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes128gcmenc_private_id = g_private_id_counter;
    handle->id                = g_aes128gcmenc_private_id;
    if ((TSIP_KEY_INDEX_TYPE_AES128 == key_index->type) || (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH == key_index->type))
    {
        ercd = prepare_gcm_iv(ivec, ivec_len, key_index, R_TSIP_AES128_KEY_INDEX_WORD_SIZE, hashed_ivec);
        if (TSIP_SUCCESS != ercd)
        {
            memset(handle, 0, sizeof(tsip_gcm_handle_t));
            return ercd;
        }
    }
    handle->flag_update_input_data = GCM_INPUT_DATA_AAD;
    return R_TSIP_Aes128GcmEncryptInitPrivate(key_index, tls13_phase, tls13_mode, p_tls13_handle, hashed_ivec);
}
/*******************************
 End of function R_TSIP_Aes128GcmEncryptInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128GcmEncryptUpdate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES GCM algorithm with AES 128 key.
* @param[in,out] handle Handler for AES for GCM(work area).
* @param[in]     plain Input plain text.
* @param[out]    cipher Output cipher text.
* @param[in]     plain_data_len Input byte size of plain.
* @param[in]     aad Input additional authentication data.
* @param[in]     aad_len Input additional authenticated data byte size.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128GcmEncryptUpdateAadPrivate()
* @see           R_TSIP_Aes128GcmEncryptUpdateTransitionPrivate()
* @see           R_TSIP_Aes128GcmEncryptUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128GcmEncryptUpdate(tsip_gcm_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_data_len, uint8_t *aad, uint32_t aad_len)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t length_rest = 0;
    uint32_t length_aad_rest = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes128gcmenc_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    if (0 != aad_len)
    {
        if (GCM_INPUT_DATA_TEXT == handle->flag_update_input_data)
        {
            return TSIP_ERR_PARAMETER;
        }
        handle->all_received_aad_length += aad_len;
        if ((handle->buffering_aad_length + aad_len) >= R_TSIP_AES_BLOCK_BYTE_SIZE)
        {
            memcpy((handle->gcm_aad_buffer + handle->buffering_aad_length), aad,
                    R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_aad_length);

            /* Casting uint32_t pointer is used for address. */
            ercd = R_TSIP_Aes128GcmEncryptUpdateAadPrivate((uint32_t *) (handle->gcm_aad_buffer),
                    R_TSIP_AES_BLOCK_BYTE_SIZE >> 2);
            length_aad_rest = aad_len - (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_aad_length);
            memset(handle->gcm_aad_buffer, 0, sizeof(handle->gcm_aad_buffer));
            if (length_aad_rest >= R_TSIP_AES_BLOCK_BYTE_SIZE)
            {
                ercd = R_TSIP_Aes128GcmEncryptUpdateAadPrivate(
                        /* Casting uint32_t pointer is used for address. */
                        (uint32_t *) (aad + (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_aad_length)),
                        ((length_aad_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE) >> 2);
                length_aad_rest -= ((length_aad_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE);
            }
            handle->buffering_aad_length = 0;
            memcpy(handle->gcm_aad_buffer, aad + (aad_len - length_aad_rest), length_aad_rest);
            handle->buffering_aad_length = length_aad_rest;
        }
        else
        {
            memcpy(handle->gcm_aad_buffer + handle->buffering_aad_length, aad, aad_len);
            handle->buffering_aad_length += aad_len;
        }
    }

    if (0 != plain_data_len)
    {
        if (GCM_INPUT_DATA_AAD == handle->flag_update_input_data)
        {
            handle->flag_update_input_data = GCM_INPUT_DATA_TEXT;
            if (0 != (handle->buffering_aad_length % R_TSIP_AES_BLOCK_BYTE_SIZE))
            {
                memset(handle->gcm_aad_buffer + handle->buffering_aad_length, 0,
                        R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_aad_length);

                /* Casting uint32_t pointer is used for address. */
                ercd = R_TSIP_Aes128GcmEncryptUpdateAadPrivate((uint32_t *) (handle->gcm_aad_buffer),
                        R_TSIP_AES_BLOCK_BYTE_SIZE >> 2);
                handle->buffering_aad_length = 0;
            }
            R_TSIP_Aes128GcmEncryptUpdateTransitionPrivate();
        }
        handle->all_received_length += plain_data_len;
        if ((handle->buffering_length + plain_data_len) >= R_TSIP_AES_BLOCK_BYTE_SIZE)
        {
            memcpy((handle->gcm_buffer + handle->buffering_length), plain,
                    R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);

            ercd = R_TSIP_Aes128GcmEncryptUpdatePrivate(
                    /* Casting uint32_t pointer is used for address. */
                    (uint32_t *) (handle->gcm_buffer), R_TSIP_AES_BLOCK_BYTE_SIZE >> 2, (uint32_t *) (cipher));
            length_rest = plain_data_len - (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);
            memset(handle->gcm_buffer, 0, sizeof(handle->gcm_buffer));
            if (length_rest >= R_TSIP_AES_BLOCK_BYTE_SIZE)
            {
                ercd = R_TSIP_Aes128GcmEncryptUpdatePrivate(
                        /* Casting uint32_t pointer is used for address. */
                        (uint32_t *) (plain + (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length)),
                        ((length_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE) >> 2,
                        /* Casting uint32_t pointer is used for address. */
                        (uint32_t *) (cipher + R_TSIP_AES_BLOCK_BYTE_SIZE));
                length_rest -= ((length_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE);
            }
            handle->buffering_length = 0;
            memcpy(handle->gcm_buffer, plain + (plain_data_len - length_rest), length_rest);
            handle->buffering_length = length_rest;
        }
        else
        {
            memcpy(handle->gcm_buffer + handle->buffering_length, plain, plain_data_len);
            handle->buffering_length += plain_data_len;
        }
    }
    return ercd;
}
/*******************************
 End of function R_TSIP_Aes128GcmEncryptUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128GcmEncryptFinal
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES GCM algorithm with AES 128 key.
* @param[in,out] handle Handler for AES for GCM(work area).
* @param[out]    cipher Output cipher text.
* @param[out]    cipher_data_len Output byte size of cipher.
* @param[out]    atag Output atag.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128GcmEncryptUpdateAadPrivate()
* @see           R_TSIP_Aes128GcmEncryptUpdateTransitionPrivate()
* @see           R_TSIP_Aes128GcmEncryptFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128GcmEncryptFinal(tsip_gcm_handle_t *handle, uint8_t *cipher, uint32_t *cipher_data_len,
        uint8_t *atag)
{
    uint32_t aad_bit_size[2] =
    {
        0
    };
    uint32_t data_bit_size[2] =
    {
        0
    };
    e_tsip_err_t ercd = TSIP_SUCCESS;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes128gcmenc_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    if (GCM_INPUT_DATA_AAD == handle->flag_update_input_data)
    {
        handle->flag_update_input_data = GCM_INPUT_DATA_TEXT;
        if (0 != (handle->buffering_aad_length % R_TSIP_AES_BLOCK_BYTE_SIZE))
        {
            memset(handle->gcm_aad_buffer + handle->buffering_aad_length, 0,
                    R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_aad_length);

            /* Casting uint32_t pointer is used for address. */
            ercd = R_TSIP_Aes128GcmEncryptUpdateAadPrivate((uint32_t *) (handle->gcm_aad_buffer),
                    R_TSIP_AES_BLOCK_BYTE_SIZE >> 2);
            handle->buffering_aad_length = 0;
        }
        R_TSIP_Aes128GcmEncryptUpdateTransitionPrivate();
    }

    if ((0 != (handle->all_received_length % R_TSIP_AES_BLOCK_BYTE_SIZE)) || (0 == handle->all_received_length))
    {
        memset(handle->gcm_buffer + handle->buffering_length, 0,
                (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length));
    }

    aad_bit_size[0] = change_endian_long((handle->all_received_aad_length & 0xe0000000u) >> 29u);
    aad_bit_size[1] = change_endian_long(handle->all_received_aad_length << 3u);

    data_bit_size[0] = change_endian_long((handle->all_received_length & 0xe0000000u) >> 29u);
    data_bit_size[1] = change_endian_long(handle->all_received_length << 3u);
    *cipher_data_len = handle->all_received_length;

    ercd = R_TSIP_Aes128GcmEncryptFinalPrivate(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)(handle->gcm_buffer), aad_bit_size, data_bit_size, (uint32_t*)cipher, (uint32_t*)atag);
    memset(handle, 0, sizeof(tsip_gcm_handle_t));
    return ercd;
}
/*******************************
 End of function R_TSIP_Aes128GcmEncryptFinal
 *******************************/
#endif /* TSIP_AES_128_GCM_ENCRYPT == 1 */

#if TSIP_AES_128_GCM_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128GcmDecryptInit
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using AES GCM algorithm with AES 128 key.
* @param[out]    handle Handler for AES for GCM(work area).
* @param[in]     key_index User key generation information area.
* @param[in]     ivec Input initial vector.
* @param[in]     ivec_len Input initial vector byte size.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           prepare_gcm_iv()
* @see           R_TSIP_Aes128GcmDecryptInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128GcmDecryptInit(tsip_gcm_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *ivec,
        uint32_t ivec_len)
{
    e_tsip_tls13_phase_t  tls13_phase      = TSIP_TLS13_PHASE_HANDSHAKE;
    e_tsip_tls13_mode_t   tls13_mode       = TSIP_TLS13_MODE_FULL_HANDSHAKE;
    tsip_tls13_handle_t * p_tls13_handle   = NULL;
    uint32_t hashed_ivec[4] =
    {
        0
    };
    e_tsip_err_t ercd = TSIP_SUCCESS;

    if ((TSIP_KEY_INDEX_TYPE_AES128 == key_index->type) || (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH == key_index->type))
    {
        if (0 == ivec_len)
        {
            return TSIP_ERR_PARAMETER;
        }
    }
    else
    {
        if ((TSIP_KEY_INDEX_TYPE_TLS_SERVER_AES128_FOR_CLIENT != key_index->type)
        && (TSIP_KEY_INDEX_TYPE_TLS_CLIENT_AES128_FOR_SERVER != key_index->type))
        {
            return TSIP_ERR_KEY_SET;
        }
    }

    memset(handle, 0, sizeof(tsip_gcm_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes128gcmdec_private_id = g_private_id_counter;
    handle->id                = g_aes128gcmdec_private_id;
    if ((TSIP_KEY_INDEX_TYPE_AES128 == key_index->type) || (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH == key_index->type))
    {
        ercd = prepare_gcm_iv(ivec, ivec_len, key_index, R_TSIP_AES128_KEY_INDEX_WORD_SIZE, hashed_ivec);
        if (TSIP_SUCCESS != ercd)
        {
            memset(handle, 0, sizeof(tsip_gcm_handle_t));
            return ercd;
        }
    }
    handle->flag_update_input_data = GCM_INPUT_DATA_AAD;
    return R_TSIP_Aes128GcmDecryptInitPrivate(key_index, tls13_phase, tls13_mode, p_tls13_handle, hashed_ivec);
}
/*******************************
 End of function R_TSIP_Aes128GcmDecryptInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128GcmDecryptUpdate
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using AES GCM algorithm with AES 128 key.
* @param[in,out] handle Handler for AES for GCM(work area).
* @param[in]     cipher Input cipher text.
* @param[out]    plain Output plain text.
* @param[in]     cipher_data_len Input byte size of cipher.
* @param[in]     aad Input additional authentication data.
* @param[in]     aad_len Input additional authenticated data byte size.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128GcmDecryptUpdateAadPrivate()
* @see           R_TSIP_Aes128GcmDecryptUpdateTransitionPrivate()
* @see           R_TSIP_Aes128GcmDecryptUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128GcmDecryptUpdate(tsip_gcm_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_data_len, uint8_t *aad, uint32_t aad_len)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t length_rest = 0;
    uint32_t length_aad_rest = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes128gcmdec_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    if (0 != aad_len)
    {
        if (GCM_INPUT_DATA_TEXT == handle->flag_update_input_data)
        {
            return TSIP_ERR_PARAMETER;
        }
        handle->all_received_aad_length += aad_len;
        if ((handle->buffering_aad_length + aad_len) >= R_TSIP_AES_BLOCK_BYTE_SIZE)
        {
            memcpy((handle->gcm_aad_buffer + handle->buffering_aad_length), aad,
                    R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_aad_length);

            /* Casting uint32_t pointer is used for address. */
            ercd = R_TSIP_Aes128GcmDecryptUpdateAadPrivate((uint32_t *) (handle->gcm_aad_buffer),
                    R_TSIP_AES_BLOCK_BYTE_SIZE >> 2);
            length_aad_rest = aad_len - (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_aad_length);
            memset(handle->gcm_aad_buffer, 0, sizeof(handle->gcm_aad_buffer));
            if (length_aad_rest >= R_TSIP_AES_BLOCK_BYTE_SIZE)
            {
                ercd = R_TSIP_Aes128GcmDecryptUpdateAadPrivate(
                        /* Casting uint32_t pointer is used for address. */
                        (uint32_t *) (aad + (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_aad_length)),
                        ((length_aad_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE) >> 2);
                length_aad_rest -= ((length_aad_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE);
            }
            handle->buffering_aad_length = 0;
            memcpy(handle->gcm_aad_buffer, aad + (aad_len - length_aad_rest), length_aad_rest);
            handle->buffering_aad_length = length_aad_rest;
        }
        else
        {
            memcpy(handle->gcm_aad_buffer + handle->buffering_aad_length, aad, aad_len);
            handle->buffering_aad_length += aad_len;
        }
    }

    if (0 != cipher_data_len)
    {
        if (GCM_INPUT_DATA_AAD == handle->flag_update_input_data)
        {
            handle->flag_update_input_data = GCM_INPUT_DATA_TEXT;
            if (0 != (handle->buffering_aad_length % R_TSIP_AES_BLOCK_BYTE_SIZE))
            {
                memset(handle->gcm_aad_buffer + handle->buffering_aad_length, 0,
                        R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_aad_length);

                /* Casting uint32_t pointer is used for address. */
                ercd = R_TSIP_Aes128GcmDecryptUpdateAadPrivate((uint32_t *) (handle->gcm_aad_buffer),
                        R_TSIP_AES_BLOCK_BYTE_SIZE >> 2);
                handle->buffering_aad_length = 0;
            }
            R_TSIP_Aes128GcmDecryptUpdateTransitionPrivate();
        }
        handle->all_received_length += cipher_data_len;
        if ((handle->buffering_length + cipher_data_len) >= R_TSIP_AES_BLOCK_BYTE_SIZE)
        {
            memcpy((handle->gcm_buffer + handle->buffering_length), cipher,
                    R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);

            ercd = R_TSIP_Aes128GcmDecryptUpdatePrivate(
                    /* Casting uint32_t pointer is used for address. */
                    (uint32_t *) (handle->gcm_buffer), R_TSIP_AES_BLOCK_BYTE_SIZE >> 2, (uint32_t *) (plain));
            length_rest = cipher_data_len - (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);
            memset(handle->gcm_buffer, 0, sizeof(handle->gcm_buffer));
            if (length_rest >= R_TSIP_AES_BLOCK_BYTE_SIZE)
            {
                ercd = R_TSIP_Aes128GcmDecryptUpdatePrivate(
                        /* Casting uint32_t pointer is used for address. */
                        (uint32_t *) (cipher + (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length)),
                        ((length_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE) >> 2,
                        /* Casting uint32_t pointer is used for address. */
                        (uint32_t *) (plain + R_TSIP_AES_BLOCK_BYTE_SIZE));
                length_rest -= ((length_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE);
            }
            handle->buffering_length = 0;
            memcpy(handle->gcm_buffer, cipher + (cipher_data_len - length_rest), length_rest);
            handle->buffering_length = length_rest;
        }
        else
        {
            memcpy(handle->gcm_buffer + handle->buffering_length, cipher, cipher_data_len);
            handle->buffering_length += cipher_data_len;
        }
    }
    return ercd;
}
/*******************************
 End of function R_TSIP_Aes128GcmDecryptUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128GcmDecryptFinal
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using AES GCM algorithm with AES 128 key.
* @param[in,out] handle Handler for AES for GCM(work area).
* @param[out]    plain Output plain text.
* @param[out]    plain_data_len Output byte size of plain.
* @param[in]     atag Input atag.
* @param[in]     atag_len Input atag byte size.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @retval        TSIP_ERR_AUTHENTICATION: Authentication NG
* @see           R_TSIP_Aes128GcmDecryptUpdateAadPrivate()
* @see           R_TSIP_Aes128GcmDecryptUpdateTransitionPrivate()
* @see           R_TSIP_Aes128GcmDecryptFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128GcmDecryptFinal(tsip_gcm_handle_t *handle, uint8_t *plain, uint32_t *plain_data_len,
        uint8_t *atag, uint32_t atag_len)
{
    uint32_t aad_bit_size[2] =
    {
        0
    };
    uint32_t data_bit_size[2] =
    {
        0
    };
    uint32_t atag_len_tmp = 0;
    uint32_t atag_tmp[16 / sizeof(uint32_t)] =
    {
        0   /* atag_tmp is initialized with 0. */
    };
    e_tsip_err_t ercd = TSIP_SUCCESS;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes128gcmdec_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    if (((1 > atag_len) || (16 < atag_len)) || ((atag_len < 12) && (0 != (atag_len % 4))))
    {
        return TSIP_ERR_PARAMETER;
    }

    if (GCM_INPUT_DATA_AAD == handle->flag_update_input_data)
    {
        handle->flag_update_input_data = GCM_INPUT_DATA_TEXT;
        if (0 != (handle->buffering_aad_length % R_TSIP_AES_BLOCK_BYTE_SIZE))
        {
            memset(handle->gcm_aad_buffer + handle->buffering_aad_length, 0,
                    R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_aad_length);

            /* Casting uint32_t pointer is used for address. */
            ercd = R_TSIP_Aes128GcmDecryptUpdateAadPrivate((uint32_t *) (handle->gcm_aad_buffer),
                    R_TSIP_AES_BLOCK_BYTE_SIZE >> 2);
            handle->buffering_aad_length = 0;
        }
        R_TSIP_Aes128GcmDecryptUpdateTransitionPrivate();
    }

    if ((0 != (handle->all_received_length % R_TSIP_AES_BLOCK_BYTE_SIZE)) || (0 == handle->all_received_length))
    {
        memset(handle->gcm_buffer + handle->buffering_length, 0,
                (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length));
    }

    memcpy(atag_tmp, atag, atag_len);

    aad_bit_size[0] = change_endian_long((handle->all_received_aad_length & 0xe0000000u) >> 29u);
    aad_bit_size[1] = change_endian_long(handle->all_received_aad_length << 3u);

    data_bit_size[0] = change_endian_long((handle->all_received_length & 0xe0000000u) >> 29u);
    data_bit_size[1] = change_endian_long(handle->all_received_length << 3u);

    atag_len_tmp = change_endian_long(atag_len);

    *plain_data_len = handle->all_received_length;

    ercd = R_TSIP_Aes128GcmDecryptFinalPrivate(
    /* Casting uint32_t pointer is used for address. */
    (uint32_t*)handle->gcm_buffer, (uint32_t*)atag_tmp, aad_bit_size, data_bit_size, &atag_len_tmp, (uint32_t*)plain);
    memset(handle, 0, sizeof(tsip_gcm_handle_t));
    return ercd;
}
/*******************************
 End of function R_TSIP_Aes128GcmDecryptFinal
 *******************************/
#endif /* TSIP_AES_128_GCM_DECRYPT == 1 */

#if TSIP_AES_256_GCM_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256GcmEncryptInit
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES GCM algorithm with AES 256 key.
* @param[out]    handle Handler for AES for GCM(work area).
* @param[in]     key_index User key generation information area.
* @param[in]     ivec Input initial vector.
* @param[in]     ivec_len Input initial vector byte size.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           prepare_gcm_iv()
* @see           R_TSIP_Aes256GcmEncryptInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256GcmEncryptInit(tsip_gcm_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *ivec,
        uint32_t ivec_len)
{
    uint32_t hashed_ivec[4] =
    {
        0
    };
    e_tsip_err_t ercd = TSIP_SUCCESS;

    if (0 == ivec_len)
    {
        return TSIP_ERR_PARAMETER;
    }

    memset(handle, 0, sizeof(tsip_gcm_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes256gcmenc_private_id = g_private_id_counter;
    handle->id = g_aes256gcmenc_private_id;
    ercd = prepare_gcm_iv(ivec, ivec_len, key_index, R_TSIP_AES256_KEY_INDEX_WORD_SIZE, hashed_ivec);
    if (TSIP_SUCCESS != ercd)
    {
        memset(handle, 0, sizeof(tsip_gcm_handle_t));
        return ercd;
    }
    handle->flag_update_input_data = GCM_INPUT_DATA_AAD;
    return R_TSIP_Aes256GcmEncryptInitPrivate(key_index, hashed_ivec);
}
/*******************************
 End of function R_TSIP_Aes256GcmEncryptInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256GcmEncryptUpdate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES GCM algorithm with AES 256 key.
* @param[in,out] handle Handler for AES for GCM(work area).
* @param[in]     plain Input plain text.
* @param[out]    cipher Output cipher text.
* @param[in]     plain_data_len Input byte size of plain.
* @param[in]     aad Input additional authentication data.
* @param[in]     aad_len Input additional authenticated data byte size.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256GcmEncryptUpdateAadPrivate()
* @see           R_TSIP_Aes256GcmEncryptUpdateTransitionPrivate()
* @see           R_TSIP_Aes256GcmEncryptUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256GcmEncryptUpdate(tsip_gcm_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_data_len, uint8_t *aad, uint32_t aad_len)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t length_rest = 0;
    uint32_t length_aad_rest = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes256gcmenc_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    if (0 != aad_len)
    {
        if (GCM_INPUT_DATA_TEXT == handle->flag_update_input_data)
        {
            return TSIP_ERR_PARAMETER;
        }
        handle->all_received_aad_length += aad_len;
        if ((handle->buffering_aad_length + aad_len) >= R_TSIP_AES_BLOCK_BYTE_SIZE)
        {
            memcpy((handle->gcm_aad_buffer + handle->buffering_aad_length), aad,
                    R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_aad_length);

            /* Casting uint32_t pointer is used for address. */
            ercd = R_TSIP_Aes256GcmEncryptUpdateAadPrivate((uint32_t *) (handle->gcm_aad_buffer),
                    (R_TSIP_AES_BLOCK_BYTE_SIZE) >> 2);
            length_aad_rest = aad_len - (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_aad_length);
            memset(handle->gcm_aad_buffer, 0, sizeof(handle->gcm_aad_buffer));
            if (length_aad_rest >= R_TSIP_AES_BLOCK_BYTE_SIZE)
            {
                ercd = R_TSIP_Aes256GcmEncryptUpdateAadPrivate(
                        /* Casting uint32_t pointer is used for address. */
                        (uint32_t *) (aad + (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_aad_length)),
                        ((length_aad_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE) >> 2);
                length_aad_rest -= ((length_aad_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE);
            }
            handle->buffering_aad_length = 0;
            memcpy(handle->gcm_aad_buffer, aad + (aad_len - length_aad_rest), length_aad_rest);
            handle->buffering_aad_length = length_aad_rest;
        }
        else
        {
            memcpy(handle->gcm_aad_buffer + handle->buffering_aad_length, aad, aad_len);
            handle->buffering_aad_length += aad_len;
        }
    }

    if (0 != plain_data_len)
    {
        if (GCM_INPUT_DATA_AAD == handle->flag_update_input_data)
        {
            handle->flag_update_input_data = GCM_INPUT_DATA_TEXT;
            if (0 != (handle->buffering_aad_length % R_TSIP_AES_BLOCK_BYTE_SIZE))
            {
                memset(handle->gcm_aad_buffer + handle->buffering_aad_length, 0,
                        R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_aad_length);
                /* Casting uint32_t pointer is used for address. */
                ercd = R_TSIP_Aes256GcmEncryptUpdateAadPrivate((uint32_t *) (handle->gcm_aad_buffer),
                        R_TSIP_AES_BLOCK_BYTE_SIZE >> 2);
                handle->buffering_aad_length = 0;
            }
            R_TSIP_Aes256GcmEncryptUpdateTransitionPrivate();
        }
        handle->all_received_length += plain_data_len;
        if ((handle->buffering_length + plain_data_len) >= R_TSIP_AES_BLOCK_BYTE_SIZE)
        {
            memcpy((handle->gcm_buffer + handle->buffering_length), plain,
                    R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);

            ercd = R_TSIP_Aes256GcmEncryptUpdatePrivate(
                    /* Casting uint32_t pointer is used for address. */
                    (uint32_t *) (handle->gcm_buffer), R_TSIP_AES_BLOCK_BYTE_SIZE >> 2, (uint32_t *) (cipher));
            length_rest = plain_data_len - (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);
            memset(handle->gcm_buffer, 0, sizeof(handle->gcm_buffer));
            if (length_rest >= R_TSIP_AES_BLOCK_BYTE_SIZE)
            {
                ercd = R_TSIP_Aes256GcmEncryptUpdatePrivate(
                        /* Casting uint32_t pointer is used for address. */
                        (uint32_t *) (plain + (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length)),
                        ((length_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE) >> 2,
                        /* Casting uint32_t pointer is used for address. */
                        (uint32_t *) (cipher + R_TSIP_AES_BLOCK_BYTE_SIZE));
                length_rest -= ((length_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE);
            }
            handle->buffering_length = 0;
            memcpy(handle->gcm_buffer, plain + (plain_data_len - length_rest), length_rest);
            handle->buffering_length = length_rest;
        }
        else
        {
            memcpy(handle->gcm_buffer + handle->buffering_length, plain, plain_data_len);
            handle->buffering_length += plain_data_len;
        }
    }
    return ercd;
}
/*******************************
 End of function R_TSIP_Aes256GcmEncryptUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256GcmEncryptFinal
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES GCM algorithm with AES 256 key.
* @param[in,out] handle Handler for AES for GCM(work area).
* @param[out]    cipher Output cipher text.
* @param[out]    cipher_data_len Output byte size of cipher.
* @param[out]    atag Output atag.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256GcmEncryptUpdateAadPrivate()
* @see           R_TSIP_Aes256GcmEncryptUpdateTransitionPrivate()
* @see           R_TSIP_Aes256GcmEncryptFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256GcmEncryptFinal(tsip_gcm_handle_t *handle, uint8_t *cipher, uint32_t *cipher_data_len,
        uint8_t *atag)
{
    uint32_t aad_bit_size[2] =
    {
        0
    };
    uint32_t data_bit_size[2] =
    {
        0
    };
    e_tsip_err_t ercd = TSIP_SUCCESS;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes256gcmenc_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    if (GCM_INPUT_DATA_AAD == handle->flag_update_input_data)
    {
        handle->flag_update_input_data = GCM_INPUT_DATA_TEXT;
        if (0 != (handle->buffering_aad_length % R_TSIP_AES_BLOCK_BYTE_SIZE))
        {
            memset(handle->gcm_aad_buffer + handle->buffering_aad_length, 0,
                    R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_aad_length);

            /* Casting uint32_t pointer is used for address. */
            ercd = R_TSIP_Aes256GcmEncryptUpdateAadPrivate((uint32_t *) (handle->gcm_aad_buffer),
                    R_TSIP_AES_BLOCK_BYTE_SIZE >> 2);
            handle->buffering_aad_length = 0;
        }
        R_TSIP_Aes256GcmEncryptUpdateTransitionPrivate();
    }

    if ((0 != (handle->all_received_length % R_TSIP_AES_BLOCK_BYTE_SIZE)) || (0 == handle->all_received_length))
    {
        memset(handle->gcm_buffer + handle->buffering_length, 0,
                (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length));
    }

    aad_bit_size[0] = change_endian_long((handle->all_received_aad_length & 0xe0000000u) >> 29u);
    aad_bit_size[1] = change_endian_long(handle->all_received_aad_length << 3u);

    data_bit_size[0] = change_endian_long((handle->all_received_length & 0xe0000000u) >> 29u);
    data_bit_size[1] = change_endian_long(handle->all_received_length << 3u);
    *cipher_data_len = handle->all_received_length;

    ercd = R_TSIP_Aes256GcmEncryptFinalPrivate(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)(handle->gcm_buffer), aad_bit_size, data_bit_size, (uint32_t*)cipher, (uint32_t*)atag);
    memset(handle, 0, sizeof(tsip_gcm_handle_t));
    return ercd;
}
/*******************************
 End of function R_TSIP_Aes256GcmEncryptFinal
 *******************************/
#endif /* TSIP_AES_256_GCM_ENCRYPT == 1 */

#if TSIP_AES_256_GCM_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256GcmDecryptInit
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using AES GCM algorithm with AES 256 key.
* @param[out]    handle Handler for AES for GCM(work area).
* @param[in]     key_index User key generation information area.
* @param[in]     ivec Input initial vector.
* @param[in]     ivec_len Input initial vector byte size.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           prepare_gcm_iv()
* @see           R_TSIP_Aes256GcmDecryptInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256GcmDecryptInit(tsip_gcm_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *ivec,
        uint32_t ivec_len)
{
    uint32_t hashed_ivec[4] =
    {
        0
    };
    e_tsip_err_t ercd = TSIP_SUCCESS;

    if (0 == ivec_len)
    {
        return TSIP_ERR_PARAMETER;
    }

    memset(handle, 0, sizeof(tsip_gcm_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes256gcmdec_private_id = g_private_id_counter;
    handle->id = g_aes256gcmdec_private_id;
    ercd = prepare_gcm_iv(ivec, ivec_len, key_index, R_TSIP_AES256_KEY_INDEX_WORD_SIZE, hashed_ivec);
    if (TSIP_SUCCESS != ercd)
    {
        memset(handle, 0, sizeof(tsip_gcm_handle_t));
        return ercd;
    }
    handle->flag_update_input_data = GCM_INPUT_DATA_AAD;
    return R_TSIP_Aes256GcmDecryptInitPrivate(key_index, hashed_ivec);
}
/*******************************
 End of function R_TSIP_Aes256GcmDecryptInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256GcmDecryptUpdate
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using AES GCM algorithm with AES 256 key.
* @param[in,out] handle Handler for AES for GCM(work area).
* @param[in]     cipher Input cipher text.
* @param[out]    plain Output plain text.
* @param[in]     cipher_data_len Input byte size of cipher.
* @param[in]     aad Input additional authentication data.
* @param[in]     aad_len Input additional authenticated data byte size.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256GcmDecryptUpdateAadPrivate()
* @see           R_TSIP_Aes256GcmDecryptUpdateTransitionPrivate()
* @see           R_TSIP_Aes256GcmDecryptUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256GcmDecryptUpdate(tsip_gcm_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_data_len, uint8_t *aad, uint32_t aad_len)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t length_rest = 0;
    uint32_t length_aad_rest = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes256gcmdec_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    if (0 != aad_len)
    {
        if (GCM_INPUT_DATA_TEXT == handle->flag_update_input_data)
        {
            return TSIP_ERR_PARAMETER;
        }
        handle->all_received_aad_length += aad_len;
        if ((handle->buffering_aad_length + aad_len) >= R_TSIP_AES_BLOCK_BYTE_SIZE)
        {
            memcpy((handle->gcm_aad_buffer + handle->buffering_aad_length), aad,
                    R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_aad_length);

            /* Casting uint32_t pointer is used for address. */
            ercd = R_TSIP_Aes256GcmDecryptUpdateAadPrivate((uint32_t *) (handle->gcm_aad_buffer),
                    R_TSIP_AES_BLOCK_BYTE_SIZE >> 2);
            length_aad_rest = aad_len - (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_aad_length);
            memset(handle->gcm_aad_buffer, 0, sizeof(handle->gcm_aad_buffer));
            if (length_aad_rest >= R_TSIP_AES_BLOCK_BYTE_SIZE)
            {
                ercd = R_TSIP_Aes256GcmDecryptUpdateAadPrivate(
                        /* Casting uint32_t pointer is used for address. */
                        (uint32_t *) (aad + (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_aad_length)),
                        ((length_aad_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE) >> 2);
                length_aad_rest -= ((length_aad_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE);
            }
            handle->buffering_aad_length = 0;
            memcpy(handle->gcm_aad_buffer, aad + (aad_len - length_aad_rest), length_aad_rest);
            handle->buffering_aad_length = length_aad_rest;
        }
        else
        {
            memcpy(handle->gcm_aad_buffer + handle->buffering_aad_length, aad, aad_len);
            handle->buffering_aad_length += aad_len;
        }
    }

    if (0 != cipher_data_len)
    {
        if (GCM_INPUT_DATA_AAD == handle->flag_update_input_data)
        {
            handle->flag_update_input_data = GCM_INPUT_DATA_TEXT;
            if (0 != (handle->buffering_aad_length % R_TSIP_AES_BLOCK_BYTE_SIZE))
            {
                memset(handle->gcm_aad_buffer + handle->buffering_aad_length, 0,
                        R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_aad_length);

                /* Casting uint32_t pointer is used for address. */
                ercd = R_TSIP_Aes256GcmDecryptUpdateAadPrivate((uint32_t *) (handle->gcm_aad_buffer),
                        R_TSIP_AES_BLOCK_BYTE_SIZE >> 2);
                handle->buffering_aad_length = 0;
            }
            R_TSIP_Aes256GcmDecryptUpdateTransitionPrivate();
        }
        handle->all_received_length += cipher_data_len;
        if ((handle->buffering_length + cipher_data_len) >= R_TSIP_AES_BLOCK_BYTE_SIZE)
        {
            memcpy((handle->gcm_buffer + handle->buffering_length), cipher,
                    R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);

            ercd = R_TSIP_Aes256GcmDecryptUpdatePrivate(
                    /* Casting uint32_t pointer is used for address. */
                    (uint32_t *) (handle->gcm_buffer), R_TSIP_AES_BLOCK_BYTE_SIZE >> 2, (uint32_t *) (plain));
            length_rest = cipher_data_len - (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);
            memset(handle->gcm_buffer, 0, sizeof(handle->gcm_buffer));
            if (length_rest >= R_TSIP_AES_BLOCK_BYTE_SIZE)
            {
                ercd = R_TSIP_Aes256GcmDecryptUpdatePrivate(
                        /* Casting uint32_t pointer is used for address. */
                        (uint32_t *) (cipher + (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length)),
                        ((length_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE) >> 2,
                        /* Casting uint32_t pointer is used for address. */
                        (uint32_t *) (plain + R_TSIP_AES_BLOCK_BYTE_SIZE));
                length_rest -= ((length_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE);
            }
            handle->buffering_length = 0;
            memcpy(handle->gcm_buffer, cipher + (cipher_data_len - length_rest), length_rest);
            handle->buffering_length = length_rest;
        }
        else
        {
            memcpy(handle->gcm_buffer + handle->buffering_length, cipher, cipher_data_len);
            handle->buffering_length += cipher_data_len;
        }
    }
    return ercd;
}
/*******************************
 End of function R_TSIP_Aes256GcmDecryptUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256GcmDecryptFinal
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using AES GCM algorithm with AES 256 key.
* @param[in,out] handle Handler for AES for GCM(work area).
* @param[out]    plain Output plain text.
* @param[out]    plain_data_len Output byte size of plain.
* @param[in]     atag Input atag.
* @param[in]     atag_len Input atag byte size.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @retval        TSIP_ERR_AUTHENTICATION: Authentication NG
* @see           R_TSIP_Aes256GcmDecryptUpdateAadPrivate()
* @see           R_TSIP_Aes256GcmDecryptUpdateTransitionPrivate()
* @see           R_TSIP_Aes256GcmDecryptFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256GcmDecryptFinal(tsip_gcm_handle_t *handle, uint8_t *plain, uint32_t *plain_data_len,
        uint8_t *atag, uint32_t atag_len)
{
    uint32_t aad_bit_size[2] =
    {
        0
    };
    uint32_t data_bit_size[2] =
    {
        0
    };
    uint32_t atag_len_tmp = 0;
    uint32_t atag_tmp[16 / sizeof(uint32_t)] =
    {
        0   /* atag_tmp is initialized with 0. */
    };
    e_tsip_err_t ercd = TSIP_SUCCESS;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes256gcmdec_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    if (((1 > atag_len) || (16 < atag_len)) || ((atag_len < 12) && (0 != (atag_len % 4))))
    {
        return TSIP_ERR_PARAMETER;
    }

    if (GCM_INPUT_DATA_AAD == handle->flag_update_input_data)
    {
        handle->flag_update_input_data = GCM_INPUT_DATA_TEXT;
        if (0 != (handle->buffering_aad_length % R_TSIP_AES_BLOCK_BYTE_SIZE))
        {
            memset(handle->gcm_aad_buffer + handle->buffering_aad_length, 0,
                    R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_aad_length);

            /* Casting uint32_t pointer is used for address. */
            ercd = R_TSIP_Aes256GcmDecryptUpdateAadPrivate((uint32_t *) (handle->gcm_aad_buffer),
                    R_TSIP_AES_BLOCK_BYTE_SIZE >> 2);
            handle->buffering_aad_length = 0;
        }
        R_TSIP_Aes256GcmDecryptUpdateTransitionPrivate();
    }

    if ((0 != (handle->all_received_length % R_TSIP_AES_BLOCK_BYTE_SIZE)) || (0 == handle->all_received_length))
    {
        memset(handle->gcm_buffer + handle->buffering_length, 0,
                (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length));
    }

    memcpy(atag_tmp, atag, atag_len);

    aad_bit_size[0] = change_endian_long((handle->all_received_aad_length & 0xe0000000u) >> 29u);
    aad_bit_size[1] = change_endian_long(handle->all_received_aad_length << 3u);

    data_bit_size[0] = change_endian_long((handle->all_received_length & 0xe0000000u) >> 29u);
    data_bit_size[1] = change_endian_long(handle->all_received_length << 3u);

    atag_len_tmp = change_endian_long(atag_len);

    *plain_data_len = handle->all_received_length;

    ercd = R_TSIP_Aes256GcmDecryptFinalPrivate(
    /* Casting uint32_t pointer is used for address. */
    (uint32_t*)handle->gcm_buffer, (uint32_t*)atag_tmp, aad_bit_size, data_bit_size, &atag_len_tmp, (uint32_t*)plain);
    memset(handle, 0, sizeof(tsip_gcm_handle_t));
    return ercd;
}
/*******************************
 End of function R_TSIP_Aes256GcmDecryptFinal
 *******************************/
#endif /* TSIP_AES_256_GCM_DECRYPT == 1 */

#if TSIP_PRV_USE_AES_GCM
/***********************************************************************************************************************
* Function Name: prepare_gcm_iv
*******************************************************************************************************************/ /**
* @details       Generation of input parameter for R_TSIP_AesXXXGcmXXX function.
* @param[in]     ivec Input initial vector.
* @param[in]     ivec_len Input initial vector byte size.
* @param[in]     key_index user Key Generation Information area.
* @param[in]     key_index_word_size R_TSIP_AES128_KEY_INDEX_WORD_SIZE or R_TSIP_AES256_KEY_INDEX_WORD_SIZE (13u)
* @param[out]    hashed_ivec output initialization vector( using length of ivec_len as a condition)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Aes128EncryptDecryptInitSub()
* @see           R_TSIP_Aes128EncryptDecryptUpdateSub()
* @see           R_TSIP_Aes128EncryptDecryptFinalSub()
* @see           R_TSIP_Aes256EncryptDecryptInitSub()
* @see           R_TSIP_Aes256EncryptDecryptUpdateSub()
* @see           R_TSIP_Aes256EncryptDecryptFinalSub()
* @see           R_TSIP_Ghash()
* @note          None
*/
static e_tsip_err_t prepare_gcm_iv(uint8_t *ivec, uint32_t ivec_len, tsip_aes_key_index_t *key_index,
        uint32_t key_index_word_size, uint32_t *hashed_ivec)
{
    uint32_t hash_subkey[4] =
    {
        0
    };
    uint32_t hashed_ivec_tmp[4] =
    {
        0
    };
    uint32_t zero[4] =
    {
        0
    };
    uint32_t ivec_length_rest = 0;
    uint32_t ivec_bit_len[4] =
    {
        0
    };
    uint32_t ivec_tmp[4] =
    {
        0
    };
    e_tsip_err_t ret = TSIP_SUCCESS;

    /* when iv_len is 12 (96 bit), add 0x00000001 padding */
    if (12u == ivec_len)
    {
        memcpy(hashed_ivec, ivec, 12u);
        hashed_ivec[3] = change_endian_long(0x00000001u);
    }

    /* when iv_len is not 12 (96 bit), add ghash padding */
    else
    {
        uint32_t indata_keytype = 0; /* For normal */
        uint32_t indata_cmd = 0;

        /* generate hash_subkey */
        indata_cmd = change_endian_long(0); /* ECB-Encrypt command */
#if TSIP_PRV_USE_AES_128_GCM
        if (R_TSIP_AES128_KEY_INDEX_WORD_SIZE == key_index_word_size)
        {
            if (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH == key_index->type)
            {
                indata_keytype = change_endian_long(2); /* For ECDH */
            }
            ret = R_TSIP_Aes128EncryptDecryptInitSub(&indata_keytype, &indata_cmd, key_index->value, zero);
            if (TSIP_SUCCESS == ret)
            {
                R_TSIP_Aes128EncryptDecryptUpdateSub(zero, hash_subkey, 4);
                ret = R_TSIP_Aes128EncryptDecryptFinalSub();
            }
        }
#endif /* TSIP_PRV_USE_AES_128_GCM */
#if TSIP_PRV_USE_AES_256_GCM
        if (R_TSIP_AES256_KEY_INDEX_WORD_SIZE == key_index_word_size)
        {
            if (TSIP_KEY_INDEX_TYPE_AES256_FOR_ECDH == key_index->type)
            {
                indata_keytype = change_endian_long(2); /* For ECDH */
            }
            ret = R_TSIP_Aes256EncryptDecryptInitSub(&indata_keytype, &indata_cmd, key_index->value, zero);
            if (TSIP_SUCCESS == ret)
            {
                R_TSIP_Aes256EncryptDecryptUpdateSub(zero, hash_subkey, 4);
                ret = R_TSIP_Aes256EncryptDecryptFinalSub();
            }
        }
#endif /* TSIP_PRV_USE_AES_256_GCM */
        if (TSIP_SUCCESS == ret)
        {
            if (16 <= ivec_len)
            {
                /* Casting uint32_t pointer is used for address. */
                ret = R_TSIP_Ghash(hash_subkey, zero, (uint32_t*)ivec, hashed_ivec_tmp, (ivec_len / 16u) * 4);
                if (TSIP_SUCCESS == ret)
                {
                    ivec_length_rest = ivec_len % 16;
                    if (0 != ivec_length_rest)
                    {
                        memcpy(ivec_tmp, ivec + (ivec_len - ivec_length_rest), ivec_length_rest);
                        /* Casting uint32_t pointer is used for address. */
                        ret = R_TSIP_Ghash(hash_subkey, hashed_ivec_tmp, ivec_tmp, hashed_ivec_tmp, 4);
                    }
                }
            }
            else
            {
                memcpy(ivec_tmp, ivec, ivec_len);
                /* Casting uint32_t pointer is used for address. */
                ret = R_TSIP_Ghash(hash_subkey, zero, ivec_tmp, hashed_ivec_tmp, 4);
            }
            if (TSIP_SUCCESS == ret)
            {
                /* calculate ivec bit length */
                ivec_bit_len[0] = 0u;
                ivec_bit_len[1] = 0u;
                ivec_bit_len[2] = change_endian_long(ivec_len >> 29u); /* need endian change when big endian */
                ivec_bit_len[3] = change_endian_long(ivec_len << 3u); /* need endian change when big endian */

                ret = R_TSIP_Ghash(hash_subkey, hashed_ivec_tmp, ivec_bit_len, hashed_ivec, 4);
            }
        }
    }

    return ret;
}
/*****************************
 End of function prepare_gcm_iv
 *****************************/
#endif /* TSIP_PRV_USE_AES_GCM */

#if TSIP_PRV_USE_AES_CCM
/***********************************************************************************************************************
* Function Name: aes_ccm_b_counter_formatter
*******************************************************************************************************************/ /**
* @details       Formatting and counter generation.
* @param[in]     nonce Nonce.
* @param[in]     nonce_len Nonce data length(byte).
* @param[in]     adata Associated data.
* @param[in]     a_len Associated data length(byte).
* @param[in]     payload_len Payload length(byte).
* @param[in]     mac_len MAC data length(byte).
* @param[out]    counter Counter blocks area.
* @param[out]    formatted_data Formatted data area.
* @param[out]    formatted_length Formatted data length.
* @note          None
*/
static void aes_ccm_b_counter_formatter(uint8_t *nonce, uint32_t nonce_len, uint8_t *adata, uint8_t a_len,
        uint32_t payload_len, uint32_t mac_len, uint8_t *counter, uint8_t *formatted_data, uint32_t *formatted_length)
{
    uint8_t flag = 0;
    uint8_t mac_len_tmp = 0;
    uint8_t q_len = 0;
    uint32_t formatted_length_tmp = 0;

    /* Out of range check */
    if (((7 > nonce_len) || (13 < nonce_len)) ||
        (((4 > mac_len) || (16 < mac_len)) || ((0 != (mac_len % 2)) || (110 < a_len))))
    {
        return;
    }

    /* formatting flag section in formatted data B */
    if (0 < a_len)
    {
        flag = 0x40;
    }

    mac_len_tmp = (mac_len - 2) / 2;
    mac_len_tmp <<= 3;
    flag |= mac_len_tmp;
    q_len = 15 - nonce_len;
    flag |= (q_len - 1);
    formatted_data[formatted_length_tmp] = flag;
    formatted_length_tmp++;

    /* adding nonce to formatted data B */
    memcpy(formatted_data + formatted_length_tmp, nonce, nonce_len);
    formatted_length_tmp += 11;
    if (12 <= nonce_len)
    {
        formatted_length_tmp++;
    }
    if (13 <= nonce_len)
    {
        formatted_length_tmp++;
    }

    /* adding Q to formatted data B */
    do
    {
        /* Casting uint32_t data to uint8_t data array. */
        formatted_data[formatted_length_tmp] = (uint8_t)(payload_len >> (8 * (15 - formatted_length_tmp)));
        formatted_length_tmp++;
    }
    while (16 != formatted_length_tmp);

    /* adding Adata to formatted data B */
    if (0 < a_len)
    {
        formatted_length_tmp++;
        formatted_data[formatted_length_tmp] = a_len;
        formatted_length_tmp++;
        memcpy(formatted_data + formatted_length_tmp, adata, a_len);
        formatted_length_tmp += a_len;
        if (0 != (formatted_length_tmp % R_TSIP_AES_BLOCK_BYTE_SIZE))
        {
            formatted_length_tmp += (R_TSIP_AES_BLOCK_BYTE_SIZE - (formatted_length_tmp % R_TSIP_AES_BLOCK_BYTE_SIZE));
        }
    }
    *formatted_length = formatted_length_tmp >> 2;

    /* formatting flag section in counter0 */
    flag = q_len - 1;
    counter[0] = flag;

    /* adding nonce to counter0 */
    memcpy(counter + 1, nonce, nonce_len);

    return;
}
/**********************************************************************************************************************
 End of function aes_ccm_b_counter_formatter
 *********************************************************************************************************************/
#endif /* TSIP_PRV_USE_AES_CCM */

#if TSIP_AES_128_CCM_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CcmEncryptInit
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CCM mode algorithm with AES 128 key.
* @param[out]    handle Handler for AES-CCM(work area).
* @param[in]     key_index User key generation information area.
* @param[in]     nonce Nonce.
* @param[in]     nonce_len Nonce data length(byte).
* @param[in]     adata Associated data.
* @param[in]     a_len Associated data length(byte).
* @param[in]     payload_len Payload length(byte).
* @param[in]     mac_len MAC data length(byte).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           aes_ccm_b_counter_formatter()
* @see           R_TSIP_Aes128CcmEncryptInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CcmEncryptInit(tsip_ccm_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *nonce,
        uint32_t nonce_len, uint8_t *adata, uint8_t a_len, uint32_t payload_len, uint32_t mac_len)
{
    e_tsip_tls13_phase_t  tls13_phase      = TSIP_TLS13_PHASE_HANDSHAKE;
    e_tsip_tls13_mode_t   tls13_mode       = TSIP_TLS13_MODE_FULL_HANDSHAKE;
    tsip_tls13_handle_t * p_tls13_handle   = NULL;
    uint32_t              formatted_length = 0;

    memset(handle, 0, sizeof(tsip_ccm_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes128ccmenc_private_id = g_private_id_counter;
    handle->id                = g_private_id_counter;

    aes_ccm_b_counter_formatter(nonce, nonce_len, adata, a_len, payload_len, mac_len,
            handle->counter, handle->formatted_data, &formatted_length);

    return R_TSIP_Aes128CcmEncryptInitPrivate(key_index, tls13_phase, tls13_mode, p_tls13_handle, payload_len,
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)handle->counter, (uint32_t*)handle->formatted_data, formatted_length);
}
/*******************************
 End of function R_TSIP_Aes128CcmEncryptInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CcmEncryptUpdate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CCM mode algorithm with AES 128 key.
* @param[in,out] handle Handler for AES-CCM(work area).
* @param[in]     plain Input plain text.
* @param[out]    cipher Output cipher text.
* @param[in]     plain_len Input byte size of plain.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128CcmEncryptUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CcmEncryptUpdate(tsip_ccm_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_length)
{
    uint32_t length_rest = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes128ccmenc_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->all_received_length += plain_length;
    if ((handle->buffering_length + plain_length) >= R_TSIP_AES_BLOCK_BYTE_SIZE)
    {
        memcpy((handle->ccm_buffer + handle->buffering_length), plain,
                R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);
        /* Casting uint32_t pointer is used for address. */
        R_TSIP_Aes128CcmEncryptUpdatePrivate((uint32_t*)handle->ccm_buffer, (uint32_t*)cipher,
                R_TSIP_AES_BLOCK_BYTE_SIZE >> 2);
        length_rest = plain_length - (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);
        if (length_rest >= R_TSIP_AES_BLOCK_BYTE_SIZE)
        {
            R_TSIP_Aes128CcmEncryptUpdatePrivate(
                    /* Casting uint32_t pointer is used for address. */
                    (uint32_t*)(plain + (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length)), (uint32_t*)
                    (cipher + R_TSIP_AES_BLOCK_BYTE_SIZE),
                    ((length_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE) >> 2);
            length_rest -= ((length_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE);
        }
        handle->buffering_length = 0;
        memset(handle->ccm_buffer, 0, sizeof(handle->ccm_buffer));
        memcpy(handle->ccm_buffer, plain + (plain_length - length_rest), length_rest);
        handle->buffering_length = length_rest;
    }
    else
    {
        memcpy(handle->ccm_buffer + handle->buffering_length, plain, plain_length);
        handle->buffering_length += plain_length;
    }
    return TSIP_SUCCESS;
}
/*******************************
 End of function R_TSIP_Aes128CcmEncryptUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CcmEncryptFinal
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CCM mode algorithm with AES 128 key.
* @param[in,out] handle Handler for AES-CCM(work area).
* @param[out]    cipher Output cipher text.
* @param[out]    cipher_length Output byte size of cipher.
* @param[out]    mac MAC data area.
* @param[in]     mac_length MAC data length(byte).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128CcmEncryptFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CcmEncryptFinal(tsip_ccm_handle_t *handle, uint8_t *cipher, uint32_t *cipher_length,
        uint8_t *mac, uint32_t mac_length)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t mac_length_tmp = 0;
    uint32_t mac_tmp[16 / sizeof(uint32_t)] =
    {
        0
    };
    uint32_t cipher_tmp[16 / sizeof(uint32_t)] =
    {
        0
    };

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes128ccmenc_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    mac_length_tmp = (((handle->formatted_data[0] >> 3) & 0x07) * 2) + 2;
    if ((mac_length != mac_length_tmp) || (2 == mac_length_tmp))
    {
        return TSIP_ERR_PARAMETER;
    }

    ercd = R_TSIP_Aes128CcmEncryptFinalPrivate(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)handle->ccm_buffer, (uint32_t*)cipher_tmp, (uint32_t*)mac_tmp);
    if (TSIP_SUCCESS == ercd)
    {
        *cipher_length = handle->all_received_length;
        memcpy(mac, mac_tmp, mac_length);
        memcpy(cipher, cipher_tmp, (handle->all_received_length & 0x0000000F));
    }
    memset(handle, 0, sizeof(tsip_ccm_handle_t));
    return ercd;
}
/*******************************
 End of function R_TSIP_Aes128CcmEncryptFinal
 *******************************/
#endif /* TSIP_AES_128_CCM_ENCRYPT == 1 */

#if TSIP_AES_128_CCM_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CcmDecryptInit
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using AES CCM mode algorithm with AES 128 key.
* @param[out]    handle Handler for AES-CCM(work area).
* @param[in]     key_index User key generation information area.
* @param[in]     nonce Nonce.
* @param[in]     nonce_len Nonce data length(byte).
* @param[in]     adata Associated data.
* @param[in]     a_len Associated data length(byte).
* @param[in]     payload_len Payload length(byte).
* @param[in]     mac_len MAC data length(byte).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           aes_ccm_b_counter_formatter()
* @see           R_TSIP_Aes128CcmDecryptInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CcmDecryptInit(tsip_ccm_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *nonce,
        uint32_t nonce_len, uint8_t *adata, uint8_t a_len, uint32_t payload_len, uint32_t mac_len)
{
    e_tsip_tls13_phase_t  tls13_phase      = TSIP_TLS13_PHASE_HANDSHAKE;
    e_tsip_tls13_mode_t   tls13_mode       = TSIP_TLS13_MODE_FULL_HANDSHAKE;
    tsip_tls13_handle_t * p_tls13_handle   = NULL;
    uint32_t              formatted_length = 0;

    memset(handle, 0, sizeof(tsip_ccm_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes128ccmdec_private_id = g_private_id_counter;
    handle->id                = g_private_id_counter;

    aes_ccm_b_counter_formatter(nonce, nonce_len, adata, a_len, payload_len, mac_len,
            handle->counter, handle->formatted_data, &formatted_length);

    return R_TSIP_Aes128CcmDecryptInitPrivate(key_index, tls13_phase, tls13_mode, p_tls13_handle, payload_len, mac_len,
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)handle->counter, (uint32_t*)handle->formatted_data, formatted_length);
}
/*******************************
 End of function R_TSIP_Aes128CcmDecryptInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CcmDecryptUpdate
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using AES CCM mode algorithm with AES 128 key.
* @param[in,out] handle Handler for AES-CCM(work area).
* @param[in]     cipher Input cipher text.
* @param[out]    plain Output plain text.
* @param[in]     cipher_len Input byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128CcmDecryptUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CcmDecryptUpdate(tsip_ccm_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_length)
{
    uint32_t length_rest = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes128ccmdec_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->all_received_length += cipher_length;
    if ((handle->buffering_length + cipher_length) >= R_TSIP_AES_BLOCK_BYTE_SIZE)
    {
        memcpy((handle->ccm_buffer + handle->buffering_length), cipher,
                R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);
        /* Casting uint32_t pointer is used for address. */
        R_TSIP_Aes128CcmDecryptUpdatePrivate((uint32_t*)handle->ccm_buffer, (uint32_t*)plain,
                R_TSIP_AES_BLOCK_BYTE_SIZE >> 2);
        length_rest = cipher_length - (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);
        if (length_rest >= R_TSIP_AES_BLOCK_BYTE_SIZE)
        {
            R_TSIP_Aes128CcmDecryptUpdatePrivate(
                    /* Casting uint32_t pointer is used for address. */
                    (uint32_t*)(cipher + (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length)), (uint32_t*)
                    (plain + R_TSIP_AES_BLOCK_BYTE_SIZE),
                    ((length_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE) >> 2);
            length_rest -= ((length_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE);
        }
        handle->buffering_length = 0;
        memset(handle->ccm_buffer, 0, sizeof(handle->ccm_buffer));
        memcpy(handle->ccm_buffer, cipher + (cipher_length - length_rest), length_rest);
        handle->buffering_length = length_rest;
    }
    else
    {
        memcpy(handle->ccm_buffer + handle->buffering_length, cipher, cipher_length);
        handle->buffering_length += cipher_length;
    }
    return TSIP_SUCCESS;
}
/*******************************
 End of function R_TSIP_Aes128CcmDecryptUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CcmDecryptFinal
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using AES CCM mode algorithm with AES 128 key.
* @param[in,out] handle Handler for AES-CCM(work area).
* @param[out]    plain Output plain text.
* @param[out]    plain_length Output byte size of cipher.
* @param[in]     mac MAC data area.
* @param[in]     mac_length MAC data length(byte).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128CcmDecryptFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CcmDecryptFinal(tsip_ccm_handle_t *handle, uint8_t *plain, uint32_t *plain_length,
        uint8_t *mac, uint32_t mac_length)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t mac_length_tmp = 0;
    uint32_t mac_tmp[16 / sizeof(uint32_t)] =
    {
        0
    };
    uint32_t plain_tmp[16 / sizeof(uint32_t)] =
    {
        0
    };

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes128ccmdec_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    mac_length_tmp = (((handle->formatted_data[0] >> 3) & 0x07) * 2) + 2;
    if ((mac_length != mac_length_tmp) || (2 == mac_length_tmp))
    {
        return TSIP_ERR_PARAMETER;
    }

    memcpy(mac_tmp, mac, mac_length);
    /* Casting uint32_t pointer is used for address. */
    ercd = R_TSIP_Aes128CcmDecryptFinalPrivate((uint32_t*)handle->ccm_buffer, (uint32_t*)mac_tmp, (uint32_t*)plain_tmp);
    if (TSIP_SUCCESS == ercd)
    {
        *plain_length = handle->all_received_length;
        memcpy(plain, plain_tmp, (handle->all_received_length & 0x0000000F));
    }
    memset(handle, 0, sizeof(tsip_ccm_handle_t));
    return ercd;
}
/*******************************
 End of function R_TSIP_Aes128CcmDecryptFinal
 *******************************/
#endif /* TSIP_AES_128_CCM_DECRYPT == 1 */

#if TSIP_AES_256_CCM_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CcmEncryptInit
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CCM mode algorithm with AES 256 key.
* @param[out]    handle Handler for AES-CCM(work area).
* @param[in]     key_index User key generation information area.
* @param[in]     nonce Nonce.
* @param[in]     nonce_len Nonce data length(byte).
* @param[in]     adata Associated data.
* @param[in]     a_len Associated data length(byte).
* @param[in]     payload_len Payload length(byte).
* @param[in]     mac_len MAC data length(byte).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           aes_ccm_b_counter_formatter()
* @see           R_TSIP_Aes256CcmEncryptInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CcmEncryptInit(tsip_ccm_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *nonce,
        uint32_t nonce_len, uint8_t *adata, uint8_t a_len, uint32_t payload_len, uint32_t mac_len)
{
    uint32_t formatted_length = 0;

    memset(handle, 0, sizeof(tsip_ccm_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes256ccmenc_private_id = g_private_id_counter;
    handle->id = g_private_id_counter;

    aes_ccm_b_counter_formatter(nonce, nonce_len, adata, a_len, payload_len, mac_len,
            handle->counter, handle->formatted_data, &formatted_length);

    return R_TSIP_Aes256CcmEncryptInitPrivate(key_index,
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)handle->counter, (uint32_t*)handle->formatted_data, formatted_length);
}
/*******************************
 End of function R_TSIP_Aes256CcmEncryptInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CcmEncryptUpdate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CCM mode algorithm with AES 256 key.
* @param[in,out] handle Handler for AES-CCM(work area).
* @param[in]     plain Input plain text.
* @param[out]    cipher Output cipher text.
* @param[in]     plain_len Input byte size of plain.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256CcmEncryptUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CcmEncryptUpdate(tsip_ccm_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_length)
{
    uint32_t length_rest = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes256ccmenc_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->all_received_length += plain_length;
    if ((handle->buffering_length + plain_length) >= R_TSIP_AES_BLOCK_BYTE_SIZE)
    {
        memcpy((handle->ccm_buffer + handle->buffering_length), plain,
                R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);
        /* Casting uint32_t pointer is used for address. */
        R_TSIP_Aes256CcmEncryptUpdatePrivate((uint32_t*)handle->ccm_buffer, (uint32_t*)cipher,
                R_TSIP_AES_BLOCK_BYTE_SIZE >> 2);
        length_rest = plain_length - (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);
        if (length_rest >= R_TSIP_AES_BLOCK_BYTE_SIZE)
        {
            R_TSIP_Aes256CcmEncryptUpdatePrivate(
                    /* Casting uint32_t pointer is used for address. */
                    (uint32_t*)(plain + (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length)), (uint32_t*)
                    (cipher + R_TSIP_AES_BLOCK_BYTE_SIZE),
                    ((length_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE) >> 2);
            length_rest -= ((length_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE);
        }
        handle->buffering_length = 0;
        memset(handle->ccm_buffer, 0, sizeof(handle->ccm_buffer));
        memcpy(handle->ccm_buffer, plain + (plain_length - length_rest), length_rest);
        handle->buffering_length = length_rest;
    }
    else
    {
        memcpy(handle->ccm_buffer + handle->buffering_length, plain, plain_length);
        handle->buffering_length += plain_length;
    }
    return TSIP_SUCCESS;
}
/*******************************
 End of function R_TSIP_Aes256CcmEncryptUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CcmEncryptFinal
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CCM mode algorithm with AES 256 key.
* @param[in,out] handle Handler for AES-CCM(work area).
* @param[out]    cipher Output cipher text.
* @param[out]    cipher_length Output byte size of cipher.
* @param[out]    mac MAC data area.
* @param[in]     mac_length MAC data length(byte).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256CcmEncryptFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CcmEncryptFinal(tsip_ccm_handle_t *handle, uint8_t *cipher, uint32_t *cipher_length,
        uint8_t *mac, uint32_t mac_length)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t length_tmp = 0;
    uint32_t mac_length_tmp = 0;
    uint32_t mac_tmp[16 / sizeof(uint32_t)] =
    {
        0
    };
    uint32_t cipher_tmp[16 / sizeof(uint32_t)] =
    {
        0
    };

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes256ccmenc_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    mac_length_tmp = (((handle->formatted_data[0] >> 3) & 0x07) * 2) + 2;
    if ((mac_length != mac_length_tmp) || (2 == mac_length_tmp))
    {
        return TSIP_ERR_PARAMETER;
    }

    length_tmp = change_endian_long(handle->all_received_length);
    ercd = R_TSIP_Aes256CcmEncryptFinalPrivate(&length_tmp,
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)handle->ccm_buffer, (uint32_t*)cipher_tmp, (uint32_t*)mac_tmp);
    if (TSIP_SUCCESS == ercd)
    {
        *cipher_length = handle->all_received_length;
        memcpy(mac, mac_tmp, mac_length);
        memcpy(cipher, cipher_tmp, (handle->all_received_length & 0x0000000F));
    }
    memset(handle, 0, sizeof(tsip_ccm_handle_t));
    return ercd;
}
/*******************************
 End of function R_TSIP_Aes256CcmEncryptFinal
 *******************************/
#endif /* TSIP_AES_256_CCM_ENCRYPT == 1 */

#if TSIP_AES_256_CCM_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CcmDecryptInit
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using AES CCM mode algorithm with AES 256 key.
* @param[out]    handle Handler for AES-CCM(work area).
* @param[in]     key_index User key generation information area.
* @param[in]     nonce Nonce.
* @param[in]     nonce_len Nonce data length(byte).
* @param[in]     adata Associated data.
* @param[in]     a_len Associated data length(byte).
* @param[in]     payload_len Payload length(byte).
* @param[in]     mac_len MAC data length(byte).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           aes_ccm_b_counter_formatter()
* @see           R_TSIP_Aes256CcmDecryptInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CcmDecryptInit(tsip_ccm_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *nonce,
        uint32_t nonce_len, uint8_t *adata, uint8_t a_len, uint32_t payload_len, uint32_t mac_len)
{
    uint32_t formatted_length = 0;

    memset(handle, 0, sizeof(tsip_ccm_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes256ccmdec_private_id = g_private_id_counter;
    handle->id = g_private_id_counter;

    aes_ccm_b_counter_formatter(nonce, nonce_len, adata, a_len, payload_len, mac_len,
            handle->counter, handle->formatted_data, &formatted_length);

    return R_TSIP_Aes256CcmDecryptInitPrivate(key_index,
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)handle->counter, (uint32_t*)handle->formatted_data, formatted_length);
}
/*******************************
 End of function R_TSIP_Aes256CcmDecryptInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CcmDecryptUpdate
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using AES CCM mode algorithm with AES 256 key.
* @param[in,out] handle Handler for AES-CCM(work area).
* @param[in]     cipher Input cipher text.
* @param[out]    plain Output plain text.
* @param[in]     cipher_len Input byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256CcmDecryptUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CcmDecryptUpdate(tsip_ccm_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_length)
{
    uint32_t length_rest = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes256ccmdec_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->all_received_length += cipher_length;
    if ((handle->buffering_length + cipher_length) >= R_TSIP_AES_BLOCK_BYTE_SIZE)
    {
        memcpy((handle->ccm_buffer + handle->buffering_length), cipher,
                R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);
        /* Casting uint32_t pointer is used for address. */
        R_TSIP_Aes256CcmDecryptUpdatePrivate((uint32_t*)handle->ccm_buffer, (uint32_t*)plain,
                R_TSIP_AES_BLOCK_BYTE_SIZE >> 2);
        length_rest = cipher_length - (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);
        if (length_rest >= R_TSIP_AES_BLOCK_BYTE_SIZE)
        {
            R_TSIP_Aes256CcmDecryptUpdatePrivate(
                    /* Casting uint32_t pointer is used for address. */
                    (uint32_t*)(cipher + (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length)), (uint32_t*)
                    (plain + R_TSIP_AES_BLOCK_BYTE_SIZE),
                    ((length_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE) >> 2);
            length_rest -= ((length_rest / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE);
        }
        handle->buffering_length = 0;
        memset(handle->ccm_buffer, 0, sizeof(handle->ccm_buffer));
        memcpy(handle->ccm_buffer, cipher + (cipher_length - length_rest), length_rest);
        handle->buffering_length = length_rest;
    }
    else
    {
        memcpy(handle->ccm_buffer + handle->buffering_length, cipher, cipher_length);
        handle->buffering_length += cipher_length;
    }
    return TSIP_SUCCESS;
}
/*******************************
 End of function R_TSIP_Aes256CcmDecryptUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CcmDecryptFinal
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using AES CCM mode algorithm with AES 256 key.
* @param[in,out] handle Handler for AES-CCM(work area).
* @param[out]    plain Output plain text.
* @param[out]    plain_length Output byte size of cipher.
* @param[in]     mac MAC data area.
* @param[in]     mac_length MAC data length(byte).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256CcmDecryptFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CcmDecryptFinal(tsip_ccm_handle_t *handle, uint8_t *plain, uint32_t *plain_length,
        uint8_t *mac, uint32_t mac_length)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t length_tmp = 0;
    uint32_t mac_length_tmp = 0;
    uint32_t mac_tmp[16 / sizeof(uint32_t)] =
    {
        0
    };
    uint32_t plain_tmp[16 / sizeof(uint32_t)] =
    {
        0
    };

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes256ccmdec_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    mac_length_tmp = (((handle->formatted_data[0] >> 3) & 0x07) * 2) + 2;
    if ((mac_length != mac_length_tmp) || (2 == mac_length_tmp))
    {
        return TSIP_ERR_PARAMETER;
    }

    length_tmp = change_endian_long(handle->all_received_length);
    mac_length_tmp = change_endian_long(mac_length);
    memcpy(mac_tmp, mac, mac_length);
    ercd = R_TSIP_Aes256CcmDecryptFinalPrivate(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)handle->ccm_buffer, &length_tmp, (uint32_t*)mac_tmp, &mac_length_tmp, (uint32_t*)plain_tmp);
    if (TSIP_SUCCESS == ercd)
    {
        *plain_length = handle->all_received_length;
        memcpy(plain, plain_tmp, (handle->all_received_length & 0x0000000F));
    }
    memset(handle, 0, sizeof(tsip_ccm_handle_t));
    return ercd;
}
/*******************************
 End of function R_TSIP_Aes256CcmDecryptFinal
 *******************************/
#endif /* TSIP_AES_256_CCM_DECRYPT == 1 */

#if TSIP_AES_128_CMAC == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CmacGenerateInit
*******************************************************************************************************************/ /**
* @details       Prepare to execute AES calculation and write the result in fist argument "handle"
* @param[out]    handle Handler for AES-CMAC (work area)
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128CmacGenerateInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CmacGenerateInit(tsip_cmac_handle_t *handle, tsip_aes_key_index_t *key_index)
{
    memset(handle, 0, sizeof(tsip_cmac_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes128cmacgen_private_id = g_private_id_counter;
    handle->id = g_aes128cmacgen_private_id;
    memset(handle->cmac_buffer, 0, sizeof(handle->cmac_buffer));
    return R_TSIP_Aes128CmacGenerateInitPrivate(key_index);
}
/*************************************
 End of function R_TSIP_Aes128CmacGenerateInit
 *************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CmacGenerateUpdate
*******************************************************************************************************************/ /**
* @details       generates a MAC value from the message specified by the second argument "message" using "key_index"
*                specified.
* @param[in,out] handle Handler for AES-CMAC (work area)
* @param[in]     message Message data area (data_len byte)
* @param[in]     message_length Message data length (0 to arbitrary byte)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128CmacGenerateUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CmacGenerateUpdate(tsip_cmac_handle_t *handle, uint8_t *message, uint32_t message_length)
{
    uint32_t length_rest = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes128cmacgen_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->all_received_length += message_length;
    if ((handle->buffering_length + message_length) > R_TSIP_AES_BLOCK_BYTE_SIZE)
    {
        memcpy((handle->cmac_buffer + handle->buffering_length), message,
                R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);

        /* Casting uint32_t pointer is used for address. */
        R_TSIP_Aes128CmacGenerateUpdatePrivate((uint32_t *) (handle->cmac_buffer), R_TSIP_AES_BLOCK_BYTE_SIZE >> 2);
        length_rest = message_length - (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);
        memset(handle->cmac_buffer, 0, sizeof(handle->cmac_buffer));
        if (length_rest > R_TSIP_AES_BLOCK_BYTE_SIZE)
        {
            R_TSIP_Aes128CmacGenerateUpdatePrivate(
                    /* Casting uint32_t pointer is used for address. */
                    (uint32_t *) (message + (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length)),
                    (((length_rest - 1) / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE) >> 2);
            length_rest -= (((length_rest - 1) / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE);
        }
        handle->buffering_length = 0;
        memcpy(handle->cmac_buffer, message + (message_length - length_rest), length_rest);
        handle->buffering_length = length_rest;
    }
    else
    {
        memcpy(handle->cmac_buffer + handle->buffering_length, message, message_length);
        handle->buffering_length += message_length;
    }
    return TSIP_SUCCESS;
}
/*************************************
 End of function R_TSIP_Aes128CmacGenerateUpdate
 *************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CmacGenerateFinal
*******************************************************************************************************************/ /**
* @details       outputs the Mac value to "mac" specified by the second argument and terminates CMAC operation.
* @param[in,out] handle Handler for AES-CMAC (work area)
* @param[out]    mac MAC data area (data_len byte)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128CmacGenerateFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CmacGenerateFinal(tsip_cmac_handle_t *handle, uint8_t *mac)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes128cmacgen_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    if ((0 != (handle->buffering_length % R_TSIP_AES_BLOCK_BYTE_SIZE)) || (0 == handle->all_received_length))
    {
        handle->cmac_buffer[handle->buffering_length] = 0x80;
        memset(handle->cmac_buffer + (handle->buffering_length + 1), 0,
                R_TSIP_AES_BLOCK_BYTE_SIZE - (handle->buffering_length + 1));
    }

    ercd = R_TSIP_Aes128CmacGenerateFinalPrivate(handle->all_received_length, 
            /* Casting uint32_t pointer is used for address. */
            (uint32_t *) (handle->cmac_buffer), (uint32_t *) (mac));
    memset(handle, 0, sizeof(tsip_cmac_handle_t));
    return ercd;
}
/*************************************
 End of function R_TSIP_Aes128CmacGenerateFinal
 *************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CmacVerifyInit
*******************************************************************************************************************/ /**
* @details       prepare to execute CMAC calculation and write the result in fist argument "handle".
* @param[out]    handle Handler for AES-CMAC (work area)
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128CmacVerifyInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CmacVerifyInit(tsip_cmac_handle_t *handle, tsip_aes_key_index_t *key_index)
{
    memset(handle, 0, sizeof(tsip_cmac_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes128cmacver_private_id = g_private_id_counter;
    handle->id = g_aes128cmacver_private_id;
    memset(handle->cmac_buffer, 0, sizeof(handle->cmac_buffer));
    return R_TSIP_Aes128CmacVerifyInitPrivate(key_index);
}
/***********************************
 End of function R_TSIP_Aes128CmacVerifyInit
 ***********************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CmacVerifyUpdate
*******************************************************************************************************************/ /**
* @details       veryfies the MAC value using the "key_index" specified by R_TSIP_Aes128CmacVerifyInit()
*                from the message specified by the second argument "message".
* @param[in,out] handle Handler for AES-CMAC (work area)
* @param[in]     message Message data area (data_len byte)
* @param[in]     message_length Message data length (0 to arbitrary byte)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128CmacVerifyUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CmacVerifyUpdate(tsip_cmac_handle_t *handle, uint8_t *message, uint32_t message_length)
{
    uint32_t length_rest = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes128cmacver_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->all_received_length += message_length;
    if ((handle->buffering_length + message_length) > R_TSIP_AES_BLOCK_BYTE_SIZE)
    {
        memcpy((handle->cmac_buffer + handle->buffering_length), message,
                R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);

        /* Casting uint32_t pointer is used for address. */
        R_TSIP_Aes128CmacVerifyUpdatePrivate((uint32_t *) (handle->cmac_buffer), R_TSIP_AES_BLOCK_BYTE_SIZE >> 2);
        length_rest = message_length - (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);
        memset(handle->cmac_buffer, 0, sizeof(handle->cmac_buffer));
        if (length_rest > R_TSIP_AES_BLOCK_BYTE_SIZE)
        {
            R_TSIP_Aes128CmacVerifyUpdatePrivate(
                    /* Casting uint32_t pointer is used for address. */
                    (uint32_t *) (message + (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length)),
                    (((length_rest - 1) / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE) >> 2);
            length_rest -= (((length_rest - 1) / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE);
        }
        handle->buffering_length = 0;
        memcpy(handle->cmac_buffer, message + (message_length - length_rest), length_rest);
        handle->buffering_length = length_rest;
    }
    else
    {
        memcpy(handle->cmac_buffer + handle->buffering_length, message, message_length);
        handle->buffering_length += message_length;
    }
    return TSIP_SUCCESS;
}
/***********************************
 End of function R_TSIP_Aes128CmacVerifyUpdate
 ***********************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CmacVerifyFinal
*******************************************************************************************************************/ /**
* @details       validates the Mac value by entering the Mac value in "mac" specified by the second argument.
* @param[in,out] handle Handler for AES-CMAC (work area)
* @param[out]    mac MAC data area
* @param[out]    mac_length MAC data length (2 to 16 bytes)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_AUTHENTICATION: Authentication NG
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128CmacVerifyFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CmacVerifyFinal(tsip_cmac_handle_t *handle, uint8_t *mac, uint32_t mac_length)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t mac_tmp[16 / sizeof(uint32_t)] =
    {
        0   /* mac_tmp is initialized with 0. */
    };
    uint32_t mac_length_bit = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes128cmacver_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    if ((2 > mac_length) || (mac_length > R_TSIP_AES_BLOCK_BYTE_SIZE))
    {
        return TSIP_ERR_PARAMETER;
    }

    if ((0 != (handle->buffering_length % R_TSIP_AES_BLOCK_BYTE_SIZE)) || (0 == handle->all_received_length))
    {
        handle->cmac_buffer[handle->buffering_length] = 0x80;
        memset(handle->cmac_buffer + (handle->buffering_length + 1), 0,
                R_TSIP_AES_BLOCK_BYTE_SIZE - (handle->buffering_length + 1));
    }
    memcpy(mac_tmp, mac, mac_length);
    mac_length_bit = change_endian_long(mac_length * 8);

    ercd = R_TSIP_Aes128CmacVerifyFinalPrivate(handle->all_received_length, 
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)handle->cmac_buffer, (uint32_t*)mac_tmp, &mac_length_bit);
    memset(handle, 0, sizeof(tsip_cmac_handle_t));
    return ercd;
}
/***********************************
 End of function R_TSIP_Aes128CmacVerifyFinal
 ***********************************/
#endif /* TSIP_AES_128_CMAC == 1 */

#if TSIP_AES_256_CMAC == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CmacGenerateInit
*******************************************************************************************************************/ /**
* @details       Prepare to execute AES calculation and write the result in fist argument "handle"
* @param[out]    handle Handler for AES-CMAC (work area)
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256CmacGenerateInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CmacGenerateInit(tsip_cmac_handle_t *handle, tsip_aes_key_index_t *key_index)
{
    memset(handle, 0, sizeof(tsip_cmac_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes256cmacgen_private_id = g_private_id_counter;
    handle->id = g_aes256cmacgen_private_id;
    memset(handle->cmac_buffer, 0, sizeof(handle->cmac_buffer));
    return R_TSIP_Aes256CmacGenerateInitPrivate(key_index);
}
/*************************************
 End of function R_TSIP_Aes256CmacGenerateInit
 *************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CmacGenerateUpdate
*******************************************************************************************************************/ /**
* @details       generates a MAC value from the message specified by the second argument "message" using "key_index"
*                specified.
* @param[in,out] handle Handler for AES-CMAC (work area)
* @param[in]     message Message data area (data_len byte)
* @param[in]     message_length Message data length (0 to arbitrary byte)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256CmacGenerateUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CmacGenerateUpdate(tsip_cmac_handle_t *handle, uint8_t *message, uint32_t message_length)
{
    uint32_t length_rest = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes256cmacgen_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->all_received_length += message_length;
    if ((handle->buffering_length + message_length) > R_TSIP_AES_BLOCK_BYTE_SIZE)
    {
        memcpy((handle->cmac_buffer + handle->buffering_length), message,
                R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);

        /* Casting uint32_t pointer is used for address. */
        R_TSIP_Aes256CmacGenerateUpdatePrivate((uint32_t *) (handle->cmac_buffer), R_TSIP_AES_BLOCK_BYTE_SIZE >> 2);
        length_rest = message_length - (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);
        memset(handle->cmac_buffer, 0, sizeof(handle->cmac_buffer));
        if (length_rest > R_TSIP_AES_BLOCK_BYTE_SIZE)
        {
            R_TSIP_Aes256CmacGenerateUpdatePrivate(
                    /* Casting uint32_t pointer is used for address. */
                    (uint32_t *) (message + (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length)),
                    (((length_rest - 1) / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE) >> 2);
            length_rest -= (((length_rest - 1) / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE);
        }
        handle->buffering_length = 0;
        memcpy(handle->cmac_buffer, message + (message_length - length_rest), length_rest);
        handle->buffering_length = length_rest;
    }
    else
    {
        memcpy(handle->cmac_buffer + handle->buffering_length, message, message_length);
        handle->buffering_length += message_length;
    }
    return TSIP_SUCCESS;
}
/*************************************
 End of function R_TSIP_Aes256CmacGenerateUpdate
 *************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CmacGenerateFinal
*******************************************************************************************************************/ /**
* @details       outputs the Mac value to "mac" specified by the second argument and terminates CMAC operation.
* @param[in,out] handle Handler for AES-CMAC (work area)
* @param[out]    mac MAC data area (data_len byte)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256CmacGenerateFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CmacGenerateFinal(tsip_cmac_handle_t *handle, uint8_t *mac)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes256cmacgen_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    if ((0 != (handle->buffering_length % R_TSIP_AES_BLOCK_BYTE_SIZE)) || (0 == handle->all_received_length))
    {
        handle->cmac_buffer[handle->buffering_length] = 0x80;
        memset(handle->cmac_buffer + (handle->buffering_length + 1), 0,
                R_TSIP_AES_BLOCK_BYTE_SIZE - (handle->buffering_length + 1));
    }

    ercd = R_TSIP_Aes256CmacGenerateFinalPrivate(handle->all_received_length, 
            /* Casting uint32_t pointer is used for address. */
            (uint32_t *) (handle->cmac_buffer), (uint32_t *) (mac));
    memset(handle, 0, sizeof(tsip_cmac_handle_t));
    return ercd;
}
/*************************************
 End of function R_TSIP_Aes256CmacGenerateFinal
 *************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CmacVerifyInit
*******************************************************************************************************************/ /**
* @details       prepare to execute CMAC calculation and write the result in fist argument "handle".
* @param[out]    handle Handler for AES-CMAC (work area)
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256CmacVerifyInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CmacVerifyInit(tsip_cmac_handle_t *handle, tsip_aes_key_index_t *key_index)
{
    memset(handle, 0, sizeof(tsip_cmac_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_aes256cmacver_private_id = g_private_id_counter;
    handle->id = g_aes256cmacver_private_id;
    memset(handle->cmac_buffer, 0, sizeof(handle->cmac_buffer));
    return R_TSIP_Aes256CmacVerifyInitPrivate(key_index);
}
/***********************************
 End of function R_TSIP_Aes256CmacVerifyInit
 ***********************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CmacVerifyUpdate
*******************************************************************************************************************/ /**
* @details       veryfies the MAC value using the "key_index" specified by R_TSIP_Aes256CmacVerifyInit()
*                from the message specified by the second argument "message".
* @param[in,out] handle Handler for AES-CMAC (work area)
* @param[in]     message Message data area (data_len byte)
* @param[in]     message_length Message data length (0 to arbitrary byte)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256CmacVerifyUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CmacVerifyUpdate(tsip_cmac_handle_t *handle, uint8_t *message, uint32_t message_length)
{
    uint32_t length_rest = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_aes256cmacver_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->all_received_length += message_length;
    if ((handle->buffering_length + message_length) > R_TSIP_AES_BLOCK_BYTE_SIZE)
    {
        memcpy((handle->cmac_buffer + handle->buffering_length), message,
                R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);

        /* Casting uint32_t pointer is used for address. */
        R_TSIP_Aes256CmacVerifyUpdatePrivate((uint32_t *) (handle->cmac_buffer), R_TSIP_AES_BLOCK_BYTE_SIZE >> 2);
        length_rest = message_length - (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length);
        memset(handle->cmac_buffer, 0, sizeof(handle->cmac_buffer));
        if (length_rest > R_TSIP_AES_BLOCK_BYTE_SIZE)
        {
            R_TSIP_Aes256CmacVerifyUpdatePrivate(
                    /* Casting uint32_t pointer is used for address. */
                    (uint32_t *) (message + (R_TSIP_AES_BLOCK_BYTE_SIZE - handle->buffering_length)),
                    (((length_rest - 1) / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE) >> 2);
            length_rest -= (((length_rest - 1) / R_TSIP_AES_BLOCK_BYTE_SIZE) * R_TSIP_AES_BLOCK_BYTE_SIZE);
        }
        handle->buffering_length = 0;
        memcpy(handle->cmac_buffer, message + (message_length - length_rest), length_rest);
        handle->buffering_length = length_rest;
    }
    else
    {
        memcpy(handle->cmac_buffer + handle->buffering_length, message, message_length);
        handle->buffering_length += message_length;
    }
    return TSIP_SUCCESS;
}
/***********************************
 End of function R_TSIP_Aes256CmacVerifyUpdate
 ***********************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CmacVerifyFinal
*******************************************************************************************************************/ /**
* @details       validates the Mac value by entering the Mac value in "mac" specified by the second argument.
* @param[in,out] handle Handler for AES-CMAC (work area)
* @param[out]    mac MAC data area
* @param[out]    mac_length MAC data length (2 to 16 bytes)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_AUTHENTICATION: Authentication NG
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes256CmacVerifyFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CmacVerifyFinal(tsip_cmac_handle_t *handle, uint8_t *mac, uint32_t mac_length)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t mac_tmp[16 / sizeof(uint32_t)] =
    {
        0   /* mac_tmp is initialized with 0. */
    };
    uint32_t mac_length_bit = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_aes256cmacver_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    if ((2 > mac_length) || (mac_length > R_TSIP_AES_BLOCK_BYTE_SIZE))
    {
        return TSIP_ERR_PARAMETER;
    }

    if ((0 != (handle->buffering_length % R_TSIP_AES_BLOCK_BYTE_SIZE)) || (0 == handle->all_received_length))
    {
        handle->cmac_buffer[handle->buffering_length] = 0x80;
        memset(handle->cmac_buffer + (handle->buffering_length + 1), 0,
                R_TSIP_AES_BLOCK_BYTE_SIZE - (handle->buffering_length + 1));
    }
    memcpy(mac_tmp, mac, mac_length);
    mac_length_bit = change_endian_long(mac_length * 8);

    ercd = R_TSIP_Aes256CmacVerifyFinalPrivate(handle->all_received_length, 
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)handle->cmac_buffer, (uint32_t*)mac_tmp, &mac_length_bit);
    memset(handle, 0, sizeof(tsip_cmac_handle_t));
    return ercd;
}
/***********************************
 End of function R_TSIP_Aes256CmacVerifyFinal
 ***********************************/
#endif /* TSIP_AES_256_CMAC == 1 */

#if TSIP_AES_128_KEY_WRAP == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128KeyWrap
*******************************************************************************************************************/ /**
* @details       Wrap target_key_index with wrap_key_index.
* @param[in]     wrap_key_index key index used for wrapping
* @param[in]     target_key_type select key to be wrapped(R_TSIP_KEYWRAP_AES128 or R_TSIP_KEYWRAP_AES256)
* @param[in]     target_key_index key index to be wrapped
* @param[out]    wrapped_key wrapped key
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Aes128KeyWrapPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128KeyWrap(tsip_aes_key_index_t *wrap_key_index, uint32_t target_key_type,
        tsip_aes_key_index_t *target_key_index, uint32_t *wrapped_key)
{
    e_tsip_err_t error_code;

    error_code = R_TSIP_Aes128KeyWrapPrivate(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)&wrap_key_index->value, &target_key_type, (uint32_t*)&target_key_index->value, wrapped_key);

    return error_code;
}
/*******************************
 End of function R_TSIP_Aes128KeyWrap
 *******************************/
#endif /* TSIP_AES_128_KEY_WRAP == 1 */

#if TSIP_AES_256_KEY_WRAP == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256KeyWrap
*******************************************************************************************************************/ /**
* @details       Wrap target_key_index with wrap_key_index.
* @param[in]     wrap_key_index key index used for wrapping
* @param[in]     target_key_type select key to be wrapped(R_TSIP_KEYWRAP_AES128 or R_TSIP_KEYWRAP_AES256)
* @param[in]     target_key_index key index to be wrapped
* @param[out]    wrapped_key wrapped key
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Aes256KeyWrapPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256KeyWrap(tsip_aes_key_index_t *wrap_key_index, uint32_t target_key_type,
        tsip_aes_key_index_t *target_key_index, uint32_t *wrapped_key)
{
    e_tsip_err_t error_code;

    error_code = R_TSIP_Aes256KeyWrapPrivate(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)&wrap_key_index->value, &target_key_type, (uint32_t*)&target_key_index->value, wrapped_key);

    return error_code;
}
/*******************************
 End of function R_TSIP_Aes256KeyWrap
 *******************************/
#endif /* TSIP_AES_256_KEY_WRAP == 1 */

#if TSIP_AES_128_KEY_WRAP == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128KeyUnwrap
*******************************************************************************************************************/ /**
* @details       Unwrap wrapped_key with wrap_key_index.
* @param[in]     wrap_key_index key key index used for unwrapping
* @param[in]     target_key_type select key to be unwrapped(R_TSIP_KEYWRAP_AES128 or R_TSIP_KEYWRAP_AES256)
* @param[in]     wrapped_key wrapped key
* @param[out]    target_key_index key index to be unwrapped
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Aes128KeyUnWrapPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128KeyUnwrap(tsip_aes_key_index_t *wrap_key_index, uint32_t target_key_type,
        uint32_t *wrapped_key, tsip_aes_key_index_t *target_key_index)
{
    e_tsip_err_t error_code;

    error_code = R_TSIP_Aes128KeyUnWrapPrivate(wrap_key_index, &target_key_type, wrapped_key, target_key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        if (R_TSIP_KEYWRAP_AES128 == target_key_type)
        {
            target_key_index->type = TSIP_KEY_INDEX_TYPE_AES128;
        }
        else    /* if (R_TSIP_KEYWRAP_AES256 == target_key_type) */
        {
            target_key_index->type = TSIP_KEY_INDEX_TYPE_AES256;
        }
    }
    else
    {
        target_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_Aes128KeyUnwrap
 *******************************/
#endif /* TSIP_AES_128_KEY_WRAP == 1 */

#if TSIP_AES_256_KEY_WRAP == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256KeyUnwrap
*******************************************************************************************************************/ /**
* @details       Unwrap wrapped_key with wrap_key_index.
* @param[in]     wrap_key_index key key index used for unwrapping
* @param[in]     target_key_type select key to be unwrapped(R_TSIP_KEYWRAP_AES128 or R_TSIP_KEYWRAP_AES256)
* @param[in]     wrapped_key wrapped key
* @param[out]    target_key_index key index to be unwrapped
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Aes256KeyUnWrapPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256KeyUnwrap(tsip_aes_key_index_t *wrap_key_index, uint32_t target_key_type,
        uint32_t *wrapped_key, tsip_aes_key_index_t *target_key_index)
{
    e_tsip_err_t error_code;

    error_code = R_TSIP_Aes256KeyUnWrapPrivate(wrap_key_index, &target_key_type, wrapped_key, target_key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        if (R_TSIP_KEYWRAP_AES128 == target_key_type)
        {
            target_key_index->type = TSIP_KEY_INDEX_TYPE_AES128;
        }
        else    /* if (R_TSIP_KEYWRAP_AES256 == target_key_type) */
        {
            target_key_index->type = TSIP_KEY_INDEX_TYPE_AES256;
        }
    }
    else
    {
        target_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_Aes256KeyUnwrap
 *******************************/
#endif /* TSIP_AES_256_KEY_WRAP == 1 */

