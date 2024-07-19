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
 * Copyright (C) 2018-2024 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_tsip_tls_rx.c
 * Version      : 1.21
 * Description  : Interface definition for the r_tsip_tls_rx TSIP module.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 28.02.2018 1.04     First Release
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
#define TSIP_TLS_SIG_ALG_HASH_SHA256                (0x04)
#define TSIP_TLS_SIG_ALG_SIG_ECDSA                  (0x03)
#define TSIP_TLS_SIG_ALG_SIG_RSA_PKCS1              (0x01)
#define TSIP_TLS_SIG_ALG_SIG_RSA_PSS                (0x08)
#define TSIP_TLS_CERTIFICATEVERIFY_SIG_DATA_SIZE    (32)
#define TSIP_TLS13_DATA_A_INPUT_SIZE                (4)             /* To store (type||legacy_record_version||length) */
#define TSIP_TLS13_DATA_A_BYTE_SIZE                 (5)
#define TSIP_TLS13_DATA_A_PREFIX                    (0x17030300)    /* To show (type||legacy_record_version) */
#define TSIP_TLS13_DATA_A_TYPE                      (0x17)
#define TSIP_TLS13_DATA_A_VERSION_1ST               (0x03)
#define TSIP_TLS13_DATA_A_VERSION_2ND               (0x03)
#define TSIP_TLS13_TAG_SIZE                         (16)
#define TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE   (130)
#define TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE  (32)
#define TSIP_TLS13_ASN1_TAG_SEQUENCE                (0x30)
#define TSIP_TLS13_ASN1_TAG_INTEGER                 (0x02)
#define TSIP_TLS13_SIG_ALG_HASH_SHA256              (0x04)
#define TSIP_TLS13_SIG_ALG_SIG_ECDSA                (0x03)
#define TSIP_TLS13_SIG_ALG_SIG_RSA_PSS              (0x08)

/* GCM related */
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
#if TSIP_TLS == 1
extern uint32_t g_private_id_counter;
extern uint32_t g_aes128gcmenc_private_id;
extern uint32_t g_aes128gcmdec_private_id;
extern uint32_t g_aes128ccmenc_private_id;
extern uint32_t g_aes128ccmdec_private_id;
#endif /* TSIP_TLS == 1 */

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
#if TSIP_TLS == 1
static const uint8_t s_certificate_verify_content[TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE -32] =
{
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x54, 0x4c, 0x53, 0x20, 0x31, 0x2e, 0x33, 0x2c, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20,
        0x43, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x65, 0x56, 0x65, 0x72, 0x69, 0x66,
        0x79,
        0x00
};
static const uint8_t s_certificate_verify_content_client[6] =
{
    0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74
};
static const uint8_t s_certificate_verify_content_server[6] =
{
    0x73, 0x65, 0x72, 0x76, 0x65, 0x72
};

static e_tsip_err_t gen_sign_ecdsa (tsip_ecc_private_key_index_t * key_index, uint8_t * message, uint32_t message_len,
        uint8_t * signature, uint32_t * signature_len);
static e_tsip_err_t gen_sign_rsa_pss (tsip_rsa2048_private_key_index_t * key_index, uint8_t * message,
        uint32_t message_len, uint8_t * signature, uint32_t * signature_len);
#endif /* TSIP_TLS == 1 */

#if TSIP_TLS == 1
/***********************************************************************************************************************
* Function Name: gen_sign_ecdsa
*******************************************************************************************************************/ /**
* @details       Generate signature with ecdsa_secp256_sha256.
* @param[in]     key_index key index to sign.
* @param[in]     message input message.
* @param[in]     message_len byte size of message.
* @param[out]    signature output signature.
* @param[out]    signature_len byte size of sugnature.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           R_TSIP_EcdsaP256SignatureGenerate()
* @note          None
*/
static e_tsip_err_t gen_sign_ecdsa(tsip_ecc_private_key_index_t *key_index, uint8_t *message, uint32_t message_len,
        uint8_t *signature, uint32_t *signature_len)
{
    e_tsip_err_t            error_code;
    tsip_ecdsa_byte_data_t  indata_message_hash;
    tsip_ecdsa_byte_data_t  outdata_signature;

    indata_message_hash.pdata       = message;
    indata_message_hash.data_length = message_len;
    indata_message_hash.data_type   = 0;
    outdata_signature.pdata         = signature;

    error_code = R_TSIP_EcdsaP256SignatureGenerate(&indata_message_hash, &outdata_signature, key_index);

    if (TSIP_SUCCESS == error_code)
    {
        *signature_len = outdata_signature.data_length;
    }

    return error_code;
}
/*******************************
 End of function gen_sign_ecdsa
 *******************************/

/***********************************************************************************************************************
* Function Name: gen_sign_rsa_pss
*******************************************************************************************************************/ /**
* @details       Generate signature with rsa_pss_rsae_sha256.
* @param[in]     key_index key index to sign.
* @param[in]     message input message.
* @param[in]     message_len byte size of message.
* @param[out]    signature output signature.
* @param[out]    signature_len byte size of sugnature.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           rsassa_emsa_pss_encode()
* @see           R_TSIP_Rsa2048ModularExponentDecryptPrivate()
* @note          None
*/
static e_tsip_err_t gen_sign_rsa_pss(tsip_rsa2048_private_key_index_t *key_index, uint8_t *message,
        uint32_t message_len, uint8_t *signature, uint32_t *signature_len)
{
    e_tsip_err_t            error_code;
    tsip_rsa_byte_data_t    message_data;
    uint8_t                 em[256];

    message_data.pdata       = message;
    message_data.data_length = message_len;
    message_data.data_type   = 0;

    /* EMSA-PSS-ENCODE */
    error_code = rsassa_emsa_pss_encode(&message_data, R_TSIP_RSA_HASH_SHA256,
        (R_TSIP_RSA_2048_KEY_N_LENGTH_BYTE_SIZE *8) - 1, em);

    /* RSA signature */
    if (TSIP_SUCCESS == error_code)
    {
        /* Casting uint32_t pointer is used for address. */
        error_code = R_TSIP_Rsa2048ModularExponentDecryptPrivate((uint32_t *)&key_index->value, (uint32_t *)em,
            (uint32_t *)signature);
    }

    if (TSIP_SUCCESS == error_code)
    {
        *signature_len = 0x0100;
    }

    return error_code;
}
/*******************************
 End of function gen_sign_rsa_pss
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateTlsRsaPublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate TLS RSA Public key index data
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information (141 words) of RSA2048 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateTlsRsaInstallDataSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateTlsRsaPublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_tls_ca_certification_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateTlsRsaInstallDataSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CA_CERTIFICATION_PUBLIC_KEY;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateTlsRsaPublicKeyIndex
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateTlsRsaPublicKeyIndex
*******************************************************************************************************************/ /**
* @details       The API for updating TLS RSA public key index
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information (141 words) of RSA2048 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateTlsRsaDataSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateTlsRsaPublicKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_tls_ca_certification_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_UpdateTlsRsaDataSub((uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CA_CERTIFICATION_PUBLIC_KEY;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/************************************************
 End of function R_TSIP_UpdateTlsRsaPublicKeyIndex
 ************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateTlsP256EccKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate TLS ECC key pair
* @param[out]    tls_p256_ecc_key_index P256 ECC key index for TLS
* @param[out]    ephemeral_ecdh_public_key ephemeral ECDH public key
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateTlsP256EccKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateTlsP256EccKeyIndex(tsip_tls_p256_ecc_key_index_t *tls_p256_ecc_key_index,
        uint8_t *ephemeral_ecdh_public_key)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    error_code = R_TSIP_GenerateTlsP256EccKeyIndexSub(tls_p256_ecc_key_index->value,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)ephemeral_ecdh_public_key);
    if (TSIP_SUCCESS == error_code)
    {
        tls_p256_ecc_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_P256_ECC_KEY;
    }
    else
    {
        tls_p256_ecc_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateTlsP256EccKeyIndex
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsRootCertificateVerification
*******************************************************************************************************************/ /**
* @details       Verify root CA certificate.
* @param[in]     public_key_type key type
* @param[in]     certificate certificates.
* @param[in]     certificate_length byte size of certificates.
* @param[in]     public_key_n_start_position start position of public key n.
* @param[in]     public_key_n_end_position end position of public key n.
* @param[in]     public_key_e_start_position start position of public key e.
* @param[in]     public_key_e_end_position end position of public key e.
* @param[in]     signature signature for certificates.
* @param[out]    encrypted_root_public_key public key for RSA 2048bit.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TlsRootCertificateVerificationSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsRootCertificateVerification(uint32_t public_key_type, uint8_t *certificate,
        uint32_t certificate_length, uint32_t public_key_n_start_position, uint32_t public_key_n_end_position,
        uint32_t public_key_e_start_position, uint32_t public_key_e_end_position, uint8_t *signature,
        uint32_t *encrypted_root_public_key)
{
    e_tsip_err_t error_code;
    uint32_t     information[4] =
    {
        0
    };
    uint32_t     certificate_length_sub = 0;
    uint32_t     public_key_type_sub = 0;

    public_key_type_sub = change_endian_long(public_key_type);
    certificate_length_sub = change_endian_long(certificate_length);

    information[0] = change_endian_long(public_key_n_start_position);
    information[1] = change_endian_long(public_key_n_end_position);
    information[2] = change_endian_long(public_key_e_start_position);
    information[3] = change_endian_long(public_key_e_end_position);

    error_code = R_TSIP_TlsRootCertificateVerificationSub(
    /* Casting uint32_t pointer is used for address. */
    /* uint32_t *InData_Sel_PubKeyType,       */&public_key_type_sub,
    /* uint32_t *InData_Certificates,         */(uint32_t *) (certificate),
    /* uint32_t *InData_CertificatesLength,   */&certificate_length_sub,
    /* uint32_t *InData_Signature,            */(uint32_t *) (signature),
    /* uint32_t *InData_CertificatesInfo,     */information,
    /* uint32_t *OutData_PubKey,              */encrypted_root_public_key);

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsRootCertificateVerification
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsCertificateVerification
*******************************************************************************************************************/ /**
* @details       Verify server certificate and intermediate certificate.
* @param[in]     public_key_type key type
* @param[in]     encrypted_input_public_key encrypted public key.
* @param[in]     certificate certificates.
* @param[in]     certificate_length byte size of certificates.
* @param[in]     signature signature for certificates.
* @param[in]     public_key_n_start_position start position of public key n.
* @param[in]     public_key_n_end_position end position of public key n.
* @param[in]     public_key_e_start_position start position of public key e.
* @param[in]     public_key_e_end_position end position of public key e.
* @param[out]    encrypted_output_public_key public key for RSA 2048bit.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TlsCertificateVerificationSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsCertificateVerification(uint32_t public_key_type, uint32_t *encrypted_input_public_key,
        uint8_t *certificate, uint32_t certificate_length, uint8_t *signature, uint32_t public_key_n_start_position,
        uint32_t public_key_n_end_position, uint32_t public_key_e_start_position, uint32_t public_key_e_end_position,
        uint32_t *encrypted_output_public_key)
{
    e_tsip_err_t error_code;
    uint32_t     information[4] =
    {
        0
    };
    uint32_t     certificate_length_sub = 0;
    uint32_t     tmp_public_key_type = 0;
    uint32_t     tmp_public_key_output_type = 0;

    tmp_public_key_type = change_endian_long(public_key_type);
    certificate_length_sub = change_endian_long(certificate_length);

    /* InData_Sel_OutDataPubKeyType value is same to InData_Sel_InDataPubKeyType. */
    tmp_public_key_output_type = change_endian_long(public_key_type);

    information[0] = change_endian_long(public_key_n_start_position);
    information[1] = change_endian_long(public_key_n_end_position);
    information[2] = change_endian_long(public_key_e_start_position);
    information[3] = change_endian_long(public_key_e_end_position);

    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_TlsCertificateVerificationSub(
    /* uint32_t *InData_Sel_InDataPubKeyType, */&tmp_public_key_type,
    /* uint32_t *InData_Sel_OutDataPubKeyType,*/&tmp_public_key_output_type,
    /* uint32_t *InData_PubKey,               */encrypted_input_public_key,
    /* uint32_t *InData_TBSCertificate,       */(uint32_t *) certificate,
    /* uint32_t *InData_TBSCertificateLength, */&certificate_length_sub,
    /* uint32_t *InData_Signature,            */(uint32_t *) signature,
    /* uint32_t *InData_TBSCertificatesInfo,  */information,
    /* uint32_t *OutData_PubKey               */encrypted_output_public_key);

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsCertificateVerification
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsCertificateVerificationExtension
*******************************************************************************************************************/ /**
* @details       Verify server certificate and intermediate certificate with extension facility.
* @param[in]     public_key_type key type
* @param[in]     public_key_output_type output key type
* @param[in]     encrypted_input_public_key encrypted public key.
* @param[in]     certificate certificates.
* @param[in]     certificate_length byte size of certificates.
* @param[in]     signature signature for certificates.
* @param[in]     public_key_n_start_position start position of public key n.
* @param[in]     public_key_n_end_position end position of public key n.
* @param[in]     public_key_e_start_position start position of public key e.
* @param[in]     public_key_e_end_position end position of public key e.
* @param[out]    encrypted_output_public_key public key for RSA 2048bit.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TlsCertificateVerificationSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsCertificateVerificationExtension(uint32_t public_key_type, uint32_t public_key_output_type,
        uint32_t *encrypted_input_public_key,
        uint8_t *certificate, uint32_t certificate_length, uint8_t *signature, uint32_t public_key_n_start_position,
        uint32_t public_key_n_end_position, uint32_t public_key_e_start_position, uint32_t public_key_e_end_position,
        uint32_t *encrypted_output_public_key)
{
    e_tsip_err_t error_code;
    uint32_t     information[4] =
    {
        0
    };
    uint32_t     certificate_length_sub = 0;
    uint32_t     tmp_public_key_type = 0;
    uint32_t     tmp_public_key_output_type = 0;

    tmp_public_key_type = change_endian_long(public_key_type);
    tmp_public_key_output_type = change_endian_long(public_key_output_type);
    certificate_length_sub = change_endian_long(certificate_length);

    information[0] = change_endian_long(public_key_n_start_position);
    information[1] = change_endian_long(public_key_n_end_position);
    information[2] = change_endian_long(public_key_e_start_position);
    information[3] = change_endian_long(public_key_e_end_position);

    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_TlsCertificateVerificationSub(
    /* uint32_t *InData_Sel_InDataPubKeyType, */&tmp_public_key_type,
    /* uint32_t *InData_Sel_OutDataPubKeyType,*/&tmp_public_key_output_type,
    /* uint32_t *InData_PubKey,               */encrypted_input_public_key,
    /* uint32_t *InData_TBSCertificate,       */(uint32_t *) certificate,
    /* uint32_t *InData_TBSCertificateLength, */&certificate_length_sub,
    /* uint32_t *InData_Signature,            */(uint32_t *) signature,
    /* uint32_t *InData_TBSCertificatesInfo,  */information,
    /* uint32_t *OutData_PubKey               */encrypted_output_public_key);

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsCertificateVerificationExtension
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsGeneratePreMasterSecret
*******************************************************************************************************************/ /**
* @details       Generate encrypted pre-master secret.
* @param[out]    tsip_pre_master_secret pre-master secret value for TSIP.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @see           R_TSIP_TlsGeneratePreMasterSecretSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsGeneratePreMasterSecret(uint32_t *tsip_pre_master_secret)
{
    e_tsip_err_t error_code;

    error_code = R_TSIP_TlsGeneratePreMasterSecretSub(tsip_pre_master_secret);

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsGeneratePreMasterSecret
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsGenerateMasterSecret
*******************************************************************************************************************/ /**
* @details       Generate encrypted master secret.
* @param[in]     select_cipher_suite cipher suite type
* @param[in]     tsip_pre_master_secret pre-master secret value for TSIP.
* @param[in]     client_random random value reported ClientHello.
* @param[in]     server_random random value reported ServerHello.
* @param[out]    tsip_master_secret master secret value for TSIP.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TlsGenerateMasterSecretSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsGenerateMasterSecret(uint32_t select_cipher_suite, uint32_t *tsip_pre_master_secret,
        uint8_t *client_random, uint8_t *server_random, uint32_t *tsip_master_secret)
{
    e_tsip_err_t error_code;
    uint32_t     cipher_suite_sub = 0;

    cipher_suite_sub = change_endian_long(select_cipher_suite);

    error_code = R_TSIP_TlsGenerateMasterSecretSub(&cipher_suite_sub, tsip_pre_master_secret, 
            /* Casting uint32_t pointer is used for address. */
            (uint32_t *) (client_random), (uint32_t *) (server_random), tsip_master_secret);

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsGenerateMasterSecret
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsEncryptPreMasterSecretWithRsa2048PublicKey
*******************************************************************************************************************/ /**
* @details       Output the result encrypted pre-master secret with RSA 2048bit
* @param[in]     encrypted_public_key public key data.
* @param[in]     tsip_pre_master_secret pre-master secret value.
* @param[out]    encrypted_pre_master_secret the value encrypted pre-master secret.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TlsEncryptPreMasterSecretSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsEncryptPreMasterSecretWithRsa2048PublicKey(uint32_t *encrypted_public_key,
        uint32_t *tsip_pre_master_secret, uint8_t *encrypted_pre_master_secret)
{
    e_tsip_err_t error_code;

    error_code = R_TSIP_TlsEncryptPreMasterSecretSub(encrypted_public_key, tsip_pre_master_secret,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t *) encrypted_pre_master_secret);

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsEncryptPreMasterSecretWithRsa2048PublicKey
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsGenerateSessionKey
*******************************************************************************************************************/ /**
* @details       Output various key information.
* @param[in]     select_cipher_suite Key suite information number.
* @param[in]     tsip_master_secret master secret value.
* @param[in]     client_random random value reported ClientHello.
* @param[in]     server_random random value reported ServerHello.
* @param[in]     nonce_explicit nonce value
* @param[out]    client_mac_key_index the mac key during communication from client to server.
* @param[out]    server_mac_key_index the mac key during communication from server to client.
* @param[out]    client_crypto_key_index the crypto key during communication from client to server.
* @param[out]    server_crypto_key_index the crypto key during communication from server to client.
* @param[out]    client_iv not use.
* @param[out]    server_iv not use.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TlsGenerateSessionKeySub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsGenerateSessionKey(uint32_t select_cipher_suite, uint32_t *tsip_master_secret,
        uint8_t *client_random, uint8_t *server_random, uint8_t* nonce_explicit,
        tsip_hmac_sha_key_index_t *client_mac_key_index, tsip_hmac_sha_key_index_t *server_mac_key_index,
        tsip_aes_key_index_t *client_crypto_key_index, tsip_aes_key_index_t *server_crypto_key_index,
        uint8_t *client_iv, uint8_t *server_iv)
{
    e_tsip_err_t error_code;
    uint32_t     cipher_suite = 0;

    cipher_suite = change_endian_long(select_cipher_suite);

    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_TlsGenerateSessionKeySub(
    /* uint32_t *InData_Sel_CipherSuite,            */&cipher_suite,
    /* uint32_t *InData_MasterSecret,               */tsip_master_secret,
    /* uint32_t *InData_ClientRandom,               */(uint32_t *) (client_random),
    /* uint32_t *InData_ServerRandom,               */(uint32_t *) (server_random),
    /* uint32_t *InData_NonceExplicit,              */(uint32_t *) (nonce_explicit),
    /* uint32_t *OutData_ClientMACKeyOperationCode, */(uint32_t *) client_mac_key_index->value,
    /* uint32_t *OutData_ServerMACKeyOperationCode, */(uint32_t *) server_mac_key_index->value,
    /* uint32_t *OutData_ClientEncKeyOperationCode, */(uint32_t *) client_crypto_key_index->value,
    /* uint32_t *OutData_ServerEncKeyOperationCode, */(uint32_t *) server_crypto_key_index->value,
    /* uint32_t *OutData_ClientIV,                  */(uint32_t *)client_iv,
    /* uint32_t *OutData_ServerIV                   */(uint32_t *)server_iv
    );

    if (TSIP_SUCCESS == error_code)
    {
        if (R_TSIP_TLS_RSA_WITH_AES_128_CBC_SHA == select_cipher_suite)
        {
            client_mac_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CLIENT_HMAC_SHA1_FOR_CLIENT;
            server_mac_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_SERVER_HMAC_SHA1_FOR_CLIENT;
            client_crypto_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CLIENT_AES128_FOR_CLIENT;
            server_crypto_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_SERVER_AES128_FOR_CLIENT;
        }
        else if (R_TSIP_TLS_RSA_WITH_AES_256_CBC_SHA == select_cipher_suite)
        {
            client_mac_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CLIENT_HMAC_SHA1_FOR_CLIENT;
            server_mac_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_SERVER_HMAC_SHA1_FOR_CLIENT;
            client_crypto_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CLIENT_AES256_FOR_CLIENT;
            server_crypto_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_SERVER_AES256_FOR_CLIENT;
        }
        else if (R_TSIP_TLS_RSA_WITH_AES_256_CBC_SHA256 == select_cipher_suite)
        {
            client_mac_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CLIENT_HMAC_SHA256_FOR_CLIENT;
            server_mac_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_SERVER_HMAC_SHA256_FOR_CLIENT;
            client_crypto_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CLIENT_AES256_FOR_CLIENT;
            server_crypto_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_SERVER_AES256_FOR_CLIENT;
        }
        else
        {
            /* These 5 cases are intentionally combined. */
            /* R_TSIP_TLS_RSA_WITH_AES_128_CBC_SHA256 */
            /* R_TSIP_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 */
            /* R_TSIP_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256 */
            /* R_TSIP_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 */
            /* R_TSIP_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256 */
            if ((R_TSIP_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 != select_cipher_suite) &&
                (R_TSIP_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256 != select_cipher_suite))
            {
                /* These 3 cases are intentionally combined. */
                /* R_TSIP_TLS_RSA_WITH_AES_128_CBC_SHA256 */
                /* R_TSIP_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 */
                /* R_TSIP_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256 */
                client_mac_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CLIENT_HMAC_SHA256_FOR_CLIENT;
                server_mac_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_SERVER_HMAC_SHA256_FOR_CLIENT;
            }
            client_crypto_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CLIENT_AES128_FOR_CLIENT;
            server_crypto_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_SERVER_AES128_FOR_CLIENT;
        }
    }
    else
    {
        client_mac_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        server_mac_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        client_crypto_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        server_crypto_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_TlsGenerateSessionKey
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsGenerateVerifyData
*******************************************************************************************************************/ /**
* @details       Generate verify data.
* @param[in]     select_verify_data Select Client/Server data.
* @param[in]     tsip_master_secret master secret data.
* @param[in]     hand_shake_hash TLS hand shake message SHA256 HASH value.
* @param[out]    verify_data verify data.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TlsGenerateVerifyDataSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsGenerateVerifyData(uint32_t select_verify_data, uint32_t *tsip_master_secret,
        uint8_t *hand_shake_hash, uint8_t *verify_data)
{
    e_tsip_err_t error_code;
    uint32_t     tmp_select_verify_data = 0;

    tmp_select_verify_data = change_endian_long(select_verify_data);

    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_TlsGenerateVerifyDataSub(
    /* uint32_t *InData_Sel_VerifyData, */&tmp_select_verify_data,
    /* uint32_t *InData_MasterSecret,   */tsip_master_secret,
    /* uint32_t *InData_HandShakeHash,  */(uint32_t *) (hand_shake_hash),
    /* uint32_t *OutData_VerifyData     */(uint32_t *) (verify_data));

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsGenerateVerifyData
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsServersEphemeralEcdhPublicKeyRetrieves
*******************************************************************************************************************/ /**
* @details       Retrives ECDH public key.
* @param[in]     public_key_type key type
* @param[in]     client_random random value reported ClientHello.
* @param[in]     server_random random value reported ServerHello.
* @param[in]     server_ephemeral_ecdh_public_key Ephemeral ECDH public key from Server.
* @param[in]     server_key_exchange_sigunature Server Key Exchange sigunature.
* @param[in]     encrypted_public_key encrypted public key.
* @param[out]    encrypted_ephemeral_ecdh_public_key encrypted Ephemeral ECDH public key.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TlsServersEphemeralEcdhPublicKeyRetrievesSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsServersEphemeralEcdhPublicKeyRetrieves(uint32_t public_key_type, uint8_t *client_random,
        uint8_t *server_random, uint8_t *server_ephemeral_ecdh_public_key, uint8_t *server_key_exchange_signature,
        uint32_t *encrypted_public_key, uint32_t *encrypted_ephemeral_ecdh_public_key)
{
    e_tsip_err_t error_code;
    uint32_t     tmp_public_key_type = 0;
    uint32_t     tmp_compless_type = 0;

    tmp_public_key_type = change_endian_long(public_key_type);

    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_TlsServersEphemeralEcdhPublicKeyRetrievesSub(
    /* uint32_t *InData_Sel_PubKeyType,   */&tmp_public_key_type,
    /* uint32_t *InData_ClientRandom,     */(uint32_t*)client_random,
    /* uint32_t *InData_ServerRandom,     */(uint32_t*)server_random,
    /* uint32_t *InData_Sel_CompressType, */&tmp_compless_type,
    /* uint32_t *InData_SKE_Message,      */(uint32_t*)server_ephemeral_ecdh_public_key,
    /* uint32_t *InData_SKE_Signature,    */(uint32_t*)server_key_exchange_signature,
    /* uint32_t *InData_PubKey,           */encrypted_public_key,
    /* uint32_t *OutData_EphemeralPubKey  */encrypted_ephemeral_ecdh_public_key
    );

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsServersEphemeralEcdhPublicKeyRetrieves
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsGeneratePreMasterSecretWithEccP256Key
*******************************************************************************************************************/ /**
* @details       Generate encrypted pre-master secret.
* @param[in]     encrypted_public_key encrypted public key
* @param[in]     tls_p256_ecc_key_index P-256 ECC key index.
* @param[out]    tsip_pre_master_secret encrypted pre-master secret value for TSIP.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TlsGeneratePreMasterSecretWithEccP256KeySub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsGeneratePreMasterSecretWithEccP256Key(uint32_t *encrypted_public_key,
    tsip_tls_p256_ecc_key_index_t *tls_p256_ecc_key_index, uint32_t *tsip_pre_master_secret)
{
    e_tsip_err_t error_code;

    error_code = R_TSIP_TlsGeneratePreMasterSecretWithEccP256KeySub(
    /* uint32_t *InData_PubKey,               */encrypted_public_key,
    /* uint32_t *InData_KeyIndex,             */tls_p256_ecc_key_index->value,
    /* uint32_t *OutData_PreMasterSecretIndex */tsip_pre_master_secret
    );

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsGeneratePreMasterSecretWithEccP256Key
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsGenerateExtendedMasterSecret
*******************************************************************************************************************/ /**
* @details       Generate extendeded master secret.
* @param[in]     select_cipher_suite Key suite information number.
* @param[in]     tsip_pre_master_secret encrypted pre-master secret value for TSIP.
* @param[in]     digest P-256 ECC key index.
* @param[out]    extended_master_secret encrypted extended master secret value for TSIP.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TlsGenerateExtendedMasterSecretSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsGenerateExtendedMasterSecret(uint32_t select_cipher_suite, uint32_t *tsip_pre_master_secret,
        uint8_t *digest, uint32_t *extended_master_secret)
{
    e_tsip_err_t error_code;
    uint32_t cipher_suite = 0;

    cipher_suite = change_endian_long(select_cipher_suite);

    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_TlsGenerateExtendedMasterSecretSub(
    /* uint32_t *InData_Sel_CipherSuite,    */&cipher_suite,
    /* uint32_t *InData_PreMasterSecret,    */tsip_pre_master_secret,
    /* uint32_t *InData_MsgHash,            */(uint32_t *)digest,
    /* uint32_t *OutData_ExMasterSecret     */extended_master_secret);

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsGenerateExtendedMasterSecret
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateTlsSVRsaPublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate TLS RSA Public key index data for server.
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information (141 words) of RSA2048 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateTlsSVRsaInstallDataSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateTlsSVRsaPublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_tls_ca_certification_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateTlsSVRsaInstallDataSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CA_CERTIFICATION_PUBLIC_KEY;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateTlsSVRsaPublicKeyIndex
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateTlsSVRsaPublicKeyIndex
*******************************************************************************************************************/ /**
* @details       The API for updating TLS RSA public key index for server.
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information (141 words) of RSA2048 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateTlsSVRsaDataSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateTlsSVRsaPublicKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_tls_ca_certification_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_UpdateTlsSVRsaDataSub((uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CA_CERTIFICATION_PUBLIC_KEY;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/************************************************
 End of function R_TSIP_UpdateTlsSVRsaPublicKeyIndex
 ************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsRegisterCaCertificationPublicKeyIndex
*******************************************************************************************************************/ /**
* @details       The API for registering TLS RSA public key index.
* @param[in]     mode The entity for the public key index.
* @param[out]    key_index public key for RSA 2048bit.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @note          None
*/
e_tsip_err_t R_TSIP_TlsRegisterCaCertificationPublicKeyIndex(e_tsip_tls_mode_t mode,
        tsip_tls_ca_certification_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;

    if (TSIP_KEY_INDEX_TYPE_TLS_CA_CERTIFICATION_PUBLIC_KEY == key_index->type)
    {
        if (TSIP_TLS_MODE_CLIENT == mode)
        {
            memcpy(S_INST, &key_index->value, (sizeof(uint32_t)*R_TSIP_SINST_DATA_WORD_SIZE));
        }
        else /* TSIP_TLS_MODE_SERVER == mode */
        {
            memcpy(&S_INST[R_TSIP_SINST_WORD_SIZE/2], &key_index->value,
                (sizeof(uint32_t)*R_TSIP_SINST_DATA_WORD_SIZE));
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsRegisterCaCertificationPublicKeyIndex
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateTlsSVP256EccKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate TLS ECC key pair for server.
* @param[out]    tls_p256_ecc_key_index P256 ECC key index for TLS
* @param[out]    ephemeral_ecdh_public_key ephemeral ECDH public key
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateTlsSVP256EccKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateTlsSVP256EccKeyIndex(tsip_tls_p256_ecc_key_index_t *tls_p256_ecc_key_index,
        uint8_t *ephemeral_ecdh_public_key)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    error_code = R_TSIP_GenerateTlsSVP256EccKeyIndexSub(tls_p256_ecc_key_index->value,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)ephemeral_ecdh_public_key);
    if (TSIP_SUCCESS == error_code)
    {
        tls_p256_ecc_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_P256_ECC_KEY;
    }
    else
    {
        tls_p256_ecc_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateTlsSVP256EccKeyIndex
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsSVRootCertificateVerification
*******************************************************************************************************************/ /**
* @details       Verify root CA certificate for server.
* @param[in]     public_key_type key type
* @param[in]     certificate certificates.
* @param[in]     certificate_length byte size of certificates.
* @param[in]     public_key_n_start_position start position of public key n.
* @param[in]     public_key_n_end_position end position of public key n.
* @param[in]     public_key_e_start_position start position of public key e.
* @param[in]     public_key_e_end_position end position of public key e.
* @param[in]     signature signature for certificates.
* @param[out]    encrypted_root_public_key public key for RSA 2048bit.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TlsSVRootCertificateVerificationSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsSVRootCertificateVerification(uint32_t public_key_type, uint8_t *certificate,
        uint32_t certificate_length, uint32_t public_key_n_start_position, uint32_t public_key_n_end_position,
        uint32_t public_key_e_start_position, uint32_t public_key_e_end_position, uint8_t *signature,
        uint32_t *encrypted_root_public_key)
{
    e_tsip_err_t error_code;
    uint32_t     information[4] =
    {
        0
    };
    uint32_t     certificate_length_sub = 0;
    uint32_t     public_key_type_sub = 0;

    public_key_type_sub = change_endian_long(public_key_type);
    certificate_length_sub = change_endian_long(certificate_length);

    information[0] = change_endian_long(public_key_n_start_position);
    information[1] = change_endian_long(public_key_n_end_position);
    information[2] = change_endian_long(public_key_e_start_position);
    information[3] = change_endian_long(public_key_e_end_position);

    error_code = R_TSIP_TlsSVRootCertificateVerificationSub(
    /* Casting uint32_t pointer is used for address. */
    /* uint32_t *InData_Sel_PubKeyType,       */&public_key_type_sub,
    /* uint32_t *InData_Certificates,         */(uint32_t *) (certificate),
    /* uint32_t *InData_CertificatesLength,   */&certificate_length_sub,
    /* uint32_t *InData_Signature,            */(uint32_t *) (signature),
    /* uint32_t *InData_CertificatesInfo,     */information,
    /* uint32_t *OutData_PubKey,              */encrypted_root_public_key);

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsSVRootCertificateVerification
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsSVCertificateVerification
*******************************************************************************************************************/ /**
* @details       Verify server certificate and intermediate certificate for server.
* @param[in]     public_key_type key type
* @param[in]     encrypted_input_public_key encrypted public key.
* @param[in]     certificate certificates.
* @param[in]     certificate_length byte size of certificates.
* @param[in]     signature signature for certificates.
* @param[in]     public_key_n_start_position start position of public key n.
* @param[in]     public_key_n_end_position end position of public key n.
* @param[in]     public_key_e_start_position start position of public key e.
* @param[in]     public_key_e_end_position end position of public key e.
* @param[out]    encrypted_output_public_key public key for RSA 2048bit.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TlsSVCertificateVerificationSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsSVCertificateVerification(uint32_t public_key_type, uint32_t *encrypted_input_public_key,
        uint8_t *certificate, uint32_t certificate_length, uint8_t *signature, uint32_t public_key_n_start_position,
        uint32_t public_key_n_end_position, uint32_t public_key_e_start_position, uint32_t public_key_e_end_position,
        uint32_t *encrypted_output_public_key)
{
    e_tsip_err_t error_code;
    uint32_t     information[4] =
    {
        0
    };
    uint32_t     certificate_length_sub = 0;
    uint32_t     tmp_public_key_type = 0;
    uint32_t     tmp_public_key_output_type = 0;

    tmp_public_key_type = change_endian_long(public_key_type);
    certificate_length_sub = change_endian_long(certificate_length);

    /* InData_Sel_OutDataPubKeyType value is same to InData_Sel_InDataPubKeyType. */
    tmp_public_key_output_type = change_endian_long(public_key_type);

    information[0] = change_endian_long(public_key_n_start_position);
    information[1] = change_endian_long(public_key_n_end_position);
    information[2] = change_endian_long(public_key_e_start_position);
    information[3] = change_endian_long(public_key_e_end_position);

    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_TlsSVCertificateVerificationSub(
    /* uint32_t *InData_Sel_InDataPubKeyType, */&tmp_public_key_type,
    /* uint32_t *InData_Sel_OutDataPubKeyType,*/&tmp_public_key_output_type,
    /* uint32_t *InData_PubKey,               */encrypted_input_public_key,
    /* uint32_t *InData_TBSCertificate,       */(uint32_t *) certificate,
    /* uint32_t *InData_TBSCertificateLength, */&certificate_length_sub,
    /* uint32_t *InData_Signature,            */(uint32_t *) signature,
    /* uint32_t *InData_TBSCertificatesInfo,  */information,
    /* uint32_t *OutData_PubKey               */encrypted_output_public_key);

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsSVCertificateVerification
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsSVCertificateVerificationExtension
*******************************************************************************************************************/ /**
* @details       Verify server certificate and intermediate certificate with extension facility for server.
* @param[in]     public_key_type key type
* @param[in]     public_key_output_type output key type
* @param[in]     encrypted_input_public_key encrypted public key.
* @param[in]     certificate certificates.
* @param[in]     certificate_length byte size of certificates.
* @param[in]     signature signature for certificates.
* @param[in]     public_key_n_start_position start position of public key n.
* @param[in]     public_key_n_end_position end position of public key n.
* @param[in]     public_key_e_start_position start position of public key e.
* @param[in]     public_key_e_end_position end position of public key e.
* @param[out]    encrypted_output_public_key public key for RSA 2048bit.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TlsSVCertificateVerificationSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsSVCertificateVerificationExtension(uint32_t public_key_type, uint32_t public_key_output_type,
        uint32_t *encrypted_input_public_key,
        uint8_t *certificate, uint32_t certificate_length, uint8_t *signature, uint32_t public_key_n_start_position,
        uint32_t public_key_n_end_position, uint32_t public_key_e_start_position, uint32_t public_key_e_end_position,
        uint32_t *encrypted_output_public_key)
{
    e_tsip_err_t error_code;
    uint32_t     information[4] =
    {
        0
    };
    uint32_t     certificate_length_sub = 0;
    uint32_t     tmp_public_key_type = 0;
    uint32_t     tmp_public_key_output_type = 0;

    tmp_public_key_type = change_endian_long(public_key_type);
    tmp_public_key_output_type = change_endian_long(public_key_output_type);
    certificate_length_sub = change_endian_long(certificate_length);

    information[0] = change_endian_long(public_key_n_start_position);
    information[1] = change_endian_long(public_key_n_end_position);
    information[2] = change_endian_long(public_key_e_start_position);
    information[3] = change_endian_long(public_key_e_end_position);

    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_TlsSVCertificateVerificationSub(
    /* uint32_t *InData_Sel_InDataPubKeyType, */&tmp_public_key_type,
    /* uint32_t *InData_Sel_OutDataPubKeyType,*/&tmp_public_key_output_type,
    /* uint32_t *InData_PubKey,               */encrypted_input_public_key,
    /* uint32_t *InData_TBSCertificate,       */(uint32_t *) certificate,
    /* uint32_t *InData_TBSCertificateLength, */&certificate_length_sub,
    /* uint32_t *InData_Signature,            */(uint32_t *) signature,
    /* uint32_t *InData_TBSCertificatesInfo,  */information,
    /* uint32_t *OutData_PubKey               */encrypted_output_public_key);

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsSVCertificateVerificationExtension
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsSVGenerateServerRandom
*******************************************************************************************************************/ /**
* @details       The API for generateing random value in ServerHello.
* @param[in]     client_tls_version protocol version of TLS client.
* @param[in]     server_tls_version protocol version of TLS server.
* @param[in]     gmt_unix_time The current time and date in standard UNIX 32-bit format.
* @param[out]    server_random random value in ServerHello and its MAC.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           R_TSIP_TlsSVGenerateServerRandomSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsSVGenerateServerRandom(e_tsip_tls_version_t client_tls_version,
        e_tsip_tls_version_t server_tls_version, uint32_t gmt_unix_time, tsip_tls_sv_random_t *server_random)
{
    e_tsip_err_t    error_code = TSIP_SUCCESS;
    uint32_t        indata_type;
    uint32_t        indata_time;

    if ((TSIP_TLS_VERSION_13 == client_tls_version) && (TSIP_TLS_VERSION_12 == server_tls_version))
    {
        error_code = TSIP_ERR_PARAMETER;
    }
    else
    {
        if ((TSIP_TLS_VERSION_12 == server_tls_version) && (TSIP_TLS_VERSION_12 == client_tls_version))
        {
            indata_type = change_endian_long(0);
        }
        else if ((TSIP_TLS_VERSION_13 == server_tls_version) && (TSIP_TLS_VERSION_12 == client_tls_version))
        {
            indata_type = change_endian_long(1);
        }
        else /* (TSIP_TLS_VERSION_13 == server_tls_version) && (TSIP_TLS_VERSION_13 == client_tls_version) */
        {
            indata_type = change_endian_long(2);
        }
        indata_time = change_endian_long(gmt_unix_time);

        /* Casting uint32_t pointer is used for address. */
        error_code = R_TSIP_TlsSVGenerateServerRandomSub(
        /* uint32_t *InData_Type,   */&indata_type,
        /* uint32_t *InData_Time,   */&indata_time,
        /* uint32_t *OutData_Text,  */(uint32_t *)server_random->random,
        /* uint32_t *OutData_MAC    */server_random->mac);
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsSVGenerateServerRandom
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsSVDecryptPreMasterSecretWithRsa2048PrivateKey
*******************************************************************************************************************/ /**
* @details       Decrypt pre-master secret with RSA 2048bit and output encrypted pre-master secret for server.
* @param[in]     private_key_index key index to decrypt.
* @param[in]     encrypted_pre_master_secret the value encrypted pre-master secret.
* @param[out]    tsip_pre_master_secret pre-master secret value for TSIP.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_TlsSVDecryptPreMasterSecretSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsSVDecryptPreMasterSecretWithRsa2048PrivateKey(
        tsip_rsa2048_private_key_index_t *private_key_index, uint8_t *encrypted_pre_master_secret,
        uint32_t *tsip_pre_master_secret)
{
    e_tsip_err_t error_code;

    if (TSIP_KEY_INDEX_TYPE_RSA2048_PRIVATE == private_key_index->type)
    {
        /* Casting uint32_t pointer is used for address. */
        error_code = R_TSIP_TlsSVDecryptPreMasterSecretSub(
        /* uint32_t *InData_SecretKey,          */private_key_index->value,
        /* uint32_t *InData_PreMasterSecret,    */(uint32_t *)encrypted_pre_master_secret,
        /* uint32_t *OutData_PreMasterSecret    */tsip_pre_master_secret);
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsSVDecryptPreMasterSecretWithRsa2048PrivateKey
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsSVGenerateMasterSecret
*******************************************************************************************************************/ /**
* @details       Generate encrypted master secret for server.
* @param[in]     select_cipher_suite cipher suite type
* @param[in]     tsip_pre_master_secret pre-master secret value for TSIP.
* @param[in]     client_random random value reported ClientHello.
* @param[in]     server_random random value reported ServerHello.
* @param[out]    tsip_master_secret master secret value for TSIP.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TlsSVGenerateMasterSecretSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsSVGenerateMasterSecret(uint32_t select_cipher_suite, uint32_t *tsip_pre_master_secret,
        uint8_t *client_random, tsip_tls_sv_random_t *server_random, uint32_t *tsip_master_secret)
{
    e_tsip_err_t error_code;
    uint32_t     cipher_suite_sub = 0;

    cipher_suite_sub = change_endian_long(select_cipher_suite);

    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_TlsSVGenerateMasterSecretSub(
    /* uint32_t *InData_Sel_CipherSuite,    */&cipher_suite_sub,
    /* uint32_t *InData_PreMasterSecret,    */tsip_pre_master_secret,
    /* uint32_t *InData_ClientRandom,       */(uint32_t *)client_random,
    /* uint32_t *InData_ServerRandom,       */(uint32_t *)server_random->random,
    /* uint32_t *InData_MAC,                */server_random->mac,
    /* uint32_t *OutData_MasterSecret       */tsip_master_secret);

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsSVGenerateMasterSecret
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsSVGenerateSessionKey
*******************************************************************************************************************/ /**
* @details       Output various key information for server.
* @param[in]     select_cipher_suite Key suite information number.
* @param[in]     tsip_master_secret master secret value.
* @param[in]     client_random random value reported ClientHello.
* @param[in]     server_random random value reported ServerHello.
* @param[in]     nonce_explicit nonce value
* @param[out]    client_mac_key_index the mac key during communication from client to server.
* @param[out]    server_mac_key_index the mac key during communication from server to client.
* @param[out]    client_crypto_key_index the crypto key during communication from client to server.
* @param[out]    server_crypto_key_index the crypto key during communication from server to client.
* @param[out]    client_iv not use.
* @param[out]    server_iv not use.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TlsSVGenerateSessionKeySub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsSVGenerateSessionKey(uint32_t select_cipher_suite, uint32_t *tsip_master_secret,
        uint8_t *client_random, tsip_tls_sv_random_t *server_random, uint8_t* nonce_explicit,
        tsip_hmac_sha_key_index_t *client_mac_key_index, tsip_hmac_sha_key_index_t *server_mac_key_index,
        tsip_aes_key_index_t *client_crypto_key_index, tsip_aes_key_index_t *server_crypto_key_index,
        uint8_t *client_iv, uint8_t *server_iv)
{
    e_tsip_err_t error_code;
    uint32_t     cipher_suite = 0;

    cipher_suite = change_endian_long(select_cipher_suite);

    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_TlsSVGenerateSessionKeySub(
    /* uint32_t *InData_Sel_CipherSuite,            */&cipher_suite,
    /* uint32_t *InData_MasterSecret,               */tsip_master_secret,
    /* uint32_t *InData_ClientRandom,               */(uint32_t *) (client_random),
    /* uint32_t *InData_ServerRandom,               */(uint32_t *) (server_random->random),
    /* uint32_t *InData_MAC,                        */server_random->mac,
    /* uint32_t *InData_NonceExplicit,              */(uint32_t *) (nonce_explicit),
    /* uint32_t *OutData_ClientMACKeyOperationCode, */(uint32_t *) client_mac_key_index->value,
    /* uint32_t *OutData_ServerMACKeyOperationCode, */(uint32_t *) server_mac_key_index->value,
    /* uint32_t *OutData_ClientEncKeyOperationCode, */(uint32_t *) client_crypto_key_index->value,
    /* uint32_t *OutData_ServerEncKeyOperationCode, */(uint32_t *) server_crypto_key_index->value,
    /* uint32_t *OutData_ClientIV,                  */(uint32_t *)client_iv,
    /* uint32_t *OutData_ServerIV                   */(uint32_t *)server_iv
    );

    if (TSIP_SUCCESS == error_code)
    {
        if (R_TSIP_TLS_RSA_WITH_AES_128_CBC_SHA == select_cipher_suite)
        {
            client_mac_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CLIENT_HMAC_SHA1_FOR_SERVER;
            server_mac_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_SERVER_HMAC_SHA1_FOR_SERVER;
            client_crypto_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CLIENT_AES128_FOR_SERVER;
            server_crypto_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_SERVER_AES128_FOR_SERVER;
        }
        else if (R_TSIP_TLS_RSA_WITH_AES_256_CBC_SHA == select_cipher_suite)
        {
            client_mac_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CLIENT_HMAC_SHA1_FOR_SERVER;
            server_mac_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_SERVER_HMAC_SHA1_FOR_SERVER;
            client_crypto_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CLIENT_AES256_FOR_SERVER;
            server_crypto_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_SERVER_AES256_FOR_SERVER;
        }
        else if (R_TSIP_TLS_RSA_WITH_AES_256_CBC_SHA256 == select_cipher_suite)
        {
            client_mac_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CLIENT_HMAC_SHA256_FOR_SERVER;
            server_mac_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_SERVER_HMAC_SHA256_FOR_SERVER;
            client_crypto_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CLIENT_AES256_FOR_SERVER;
            server_crypto_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_SERVER_AES256_FOR_SERVER;
        }
        else
        {
            /* These 5 cases are intentionally combined. */
            /* R_TSIP_TLS_RSA_WITH_AES_128_CBC_SHA256 */
            /* R_TSIP_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 */
            /* R_TSIP_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256 */
            /* R_TSIP_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 */
            /* R_TSIP_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256 */
            if ((R_TSIP_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 != select_cipher_suite) &&
                (R_TSIP_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256 != select_cipher_suite))
            {
                /* These 3 cases are intentionally combined. */
                /* R_TSIP_TLS_RSA_WITH_AES_128_CBC_SHA256 */
                /* R_TSIP_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 */
                /* R_TSIP_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256 */
                client_mac_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CLIENT_HMAC_SHA256_FOR_SERVER;
                server_mac_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_SERVER_HMAC_SHA256_FOR_SERVER;
            }
            client_crypto_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_CLIENT_AES128_FOR_SERVER;
            server_crypto_key_index->type = TSIP_KEY_INDEX_TYPE_TLS_SERVER_AES128_FOR_SERVER;
        }
    }
    else
    {
        client_mac_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        server_mac_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        client_crypto_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        server_crypto_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_TlsSVGenerateSessionKey
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsSVGenerateVerifyData
*******************************************************************************************************************/ /**
* @details       Generate verify data for server.
* @param[in]     select_verify_data Select Client/Server data.
* @param[in]     tsip_master_secret master secret data.
* @param[in]     hand_shake_hash TLS hand shake message SHA256 HASH value.
* @param[out]    verify_data verify data.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TlsSVGenerateVerifyDataSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsSVGenerateVerifyData(uint32_t select_verify_data, uint32_t *tsip_master_secret,
        uint8_t *hand_shake_hash, uint8_t *verify_data)
{
    e_tsip_err_t error_code;
    uint32_t     tmp_select_verify_data = 0;

    tmp_select_verify_data = change_endian_long(select_verify_data);

    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_TlsSVGenerateVerifyDataSub(
    /* uint32_t *InData_Sel_VerifyData, */&tmp_select_verify_data,
    /* uint32_t *InData_MasterSecret,   */tsip_master_secret,
    /* uint32_t *InData_HandShakeHash,  */(uint32_t *) (hand_shake_hash),
    /* uint32_t *OutData_VerifyData     */(uint32_t *) (verify_data));

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsSVGenerateVerifyData
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsSVGeneratePreMasterSecretWithEccP256Key
*******************************************************************************************************************/ /**
* @details       Generate encrypted pre-master secret for server.
* @param[in]     ecdh_public_key encrypted public key
* @param[in]     tls_p256_ecc_key_index P-256 ECC key index.
* @param[out]    tsip_pre_master_secret encrypted pre-master secret value for TSIP.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_TlsSVGeneratePreMasterSecretWithEccP256KeySub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsSVGeneratePreMasterSecretWithEccP256Key(uint32_t *ecdh_public_key,
    tsip_tls_p256_ecc_key_index_t *tls_p256_ecc_key_index, uint32_t *tsip_pre_master_secret)
{
    e_tsip_err_t error_code;

    error_code = R_TSIP_TlsSVGeneratePreMasterSecretWithEccP256KeySub(
    /* uint32_t *InData_PubKey,               */ecdh_public_key,
    /* uint32_t *InData_KeyIndex,             */tls_p256_ecc_key_index->value,
    /* uint32_t *OutData_PreMasterSecretIndex */tsip_pre_master_secret
    );

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsSVGeneratePreMasterSecretWithEccP256Key
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsSVGenerateExtendedMasterSecret
*******************************************************************************************************************/ /**
* @details       Generate extendeded master secret for server.
* @param[in]     select_cipher_suite Key suite information number.
* @param[in]     tsip_pre_master_secret encrypted pre-master secret value for TSIP.
* @param[in]     digest P-256 ECC key index.
* @param[out]    extended_master_secret encrypted extended master secret value for TSIP.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TlsSVGenerateExtendedMasterSecretSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsSVGenerateExtendedMasterSecret(uint32_t select_cipher_suite, uint32_t *tsip_pre_master_secret,
        uint8_t *digest, uint32_t *extended_master_secret)
{
    e_tsip_err_t error_code;
    uint32_t cipher_suite = 0;

    cipher_suite = change_endian_long(select_cipher_suite);

    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_TlsSVGenerateExtendedMasterSecretSub(
    /* uint32_t *InData_Sel_CipherSuite,    */&cipher_suite,
    /* uint32_t *InData_PreMasterSecret,    */tsip_pre_master_secret,
    /* uint32_t *InData_MsgHash,            */(uint32_t *)digest,
    /* uint32_t *OutData_ExMasterSecret     */extended_master_secret);

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsSVGenerateExtendedMasterSecret
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TlsSVCertificateVerifyVerification
*******************************************************************************************************************/ /**
* @details       Verify Certificate Verify data to help TLS1.2 application for server.
* @param[in]     key_index encrypted public key.
* @param[in]     signature_scheme signature algorithm.
* @param[in]     handshake_message handshake message.
* @param[in]     handshake_message_len length of handshake message.
* @param[in]     certificate_verify Certificate Verify data.
* @param[in]     certificate_verify_len length of Certificate Verify data.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           R_TSIP_TlsCertificateVerificationSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TlsSVCertificateVerifyVerification(uint32_t *key_index,
        e_tsip_tls_signature_scheme_type_t signature_scheme, uint8_t *handshake_message, uint32_t handshake_message_len,
        uint8_t *certificate_verify, uint32_t certificate_verify_len)
{
    e_tsip_err_t            error_code                                                 = TSIP_SUCCESS;
    uint8_t                 signature_data[TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE * 2] =
    {
        0
    };
    uint8_t                 input_index                = 0;
    uint8_t                 output_index               = 0;
    uint8_t                 length_index               = 0;
    uint8_t                 copy_length                = 0;
    uint32_t                certificate_length_sub     = 0;
    uint32_t                tmp_public_key_type        = 0;
    uint32_t                tmp_public_key_output_type = 0;
    uint32_t              * p_tmp_signature_data;

    if (TSIP_TLS_SIGNATURE_SCHEME_RSA_PKCS1_SHA256 == signature_scheme)
    {
        if ((TSIP_TLS_SIG_ALG_HASH_SHA256 != certificate_verify[0])
            || (TSIP_TLS_SIG_ALG_SIG_RSA_PKCS1 != certificate_verify[1])) /* Signature Algorithm field */
        {
            error_code = TSIP_ERR_PARAMETER;
        }
        else if (((certificate_verify[2] << 8) + certificate_verify[3] + 4)
                != certificate_verify_len) /* Header field */
        {
            error_code = TSIP_ERR_PARAMETER;
        }
        else
        {
            error_code = TSIP_SUCCESS;
        }

        if (0x0100 == ((certificate_verify[2] << 8) + certificate_verify[3]))
        {
            tmp_public_key_type = 0;
        }
        else if(0x0200 == ((certificate_verify[2] << 8) + certificate_verify[3]))
        {
            tmp_public_key_type = change_endian_long(0x00000001);
        }
        else
        {
            error_code = TSIP_ERR_PARAMETER;
        }

        tmp_public_key_output_type = change_endian_long(0x00000005);

        /* Casting uint32_t pointer is used to input. */
        p_tmp_signature_data = (uint32_t *) &certificate_verify[4];
    }
    else if (TSIP_TLS_SIGNATURE_SCHEME_ECDSA_SECP256R1_SHA256 == signature_scheme)
    {
        if ((TSIP_TLS_SIG_ALG_HASH_SHA256 != certificate_verify[0])
            || (TSIP_TLS_SIG_ALG_SIG_ECDSA != certificate_verify[1])) /* Signature Algorithm field */
        {
            error_code = TSIP_ERR_PARAMETER;
        }
        else if ((((certificate_verify[3] + 4) != certificate_verify_len))
                || ((certificate_verify[5] + 6) != certificate_verify_len)) /* Header field */
        {
            error_code = TSIP_ERR_PARAMETER;
        }
        else
        {
            error_code = TSIP_SUCCESS;
        }

        /* Integer r field */
        if (TSIP_SUCCESS == error_code)
        {
            length_index = 7;

            if ((TSIP_TLS_CERTIFICATEVERIFY_SIG_DATA_SIZE + 1) == certificate_verify[length_index])
            {
                input_index  = length_index + 2;
                output_index = 0;
                copy_length  = TSIP_TLS_CERTIFICATEVERIFY_SIG_DATA_SIZE;
            }
            else if (TSIP_TLS_CERTIFICATEVERIFY_SIG_DATA_SIZE >= certificate_verify[length_index])
            {
                input_index  = length_index + 1;
                output_index = TSIP_TLS_CERTIFICATEVERIFY_SIG_DATA_SIZE - certificate_verify[length_index];
                copy_length  = certificate_verify[length_index];
            }
            else
            {
                error_code = TSIP_ERR_PARAMETER;
            }

            if (TSIP_SUCCESS == error_code)
            {
                memcpy(&signature_data[output_index], &certificate_verify[input_index], copy_length);
            }
        }

        /* Integer s field */
        if (TSIP_SUCCESS == error_code)
        {
            length_index = input_index + copy_length + 1;

            if ((TSIP_TLS_CERTIFICATEVERIFY_SIG_DATA_SIZE + 1) == certificate_verify[length_index])
            {
                input_index  = length_index + 2;
                output_index = TSIP_TLS_CERTIFICATEVERIFY_SIG_DATA_SIZE;
                copy_length  = TSIP_TLS_CERTIFICATEVERIFY_SIG_DATA_SIZE;
            }
            else if (TSIP_TLS_CERTIFICATEVERIFY_SIG_DATA_SIZE >= certificate_verify[length_index])
            {
                input_index  = length_index + 1;
                output_index = (TSIP_TLS_CERTIFICATEVERIFY_SIG_DATA_SIZE *2) - certificate_verify[length_index];
                copy_length  = certificate_verify[length_index];
            }
            else
            {
                error_code = TSIP_ERR_PARAMETER;
            }

            if (TSIP_SUCCESS == error_code)
            {
                memcpy(&signature_data[output_index], &certificate_verify[input_index], copy_length);
            }
        }

        tmp_public_key_type        = change_endian_long(0x00000002);
        tmp_public_key_output_type = change_endian_long(0x00000005);

        /* Casting uint32_t pointer is used to input. */
        p_tmp_signature_data = (uint32_t *)signature_data;
    }
    else if (TSIP_TLS_SIGNATURE_SCHEME_RSA_PSS_RSAE_SHA256 == signature_scheme)
    {
        if ((TSIP_TLS_SIG_ALG_SIG_RSA_PSS != certificate_verify[0])
            || (TSIP_TLS_SIG_ALG_HASH_SHA256 != certificate_verify[1])) /* Signature Algorithm field */
        {
            error_code = TSIP_ERR_PARAMETER;
        }
        else if ((((certificate_verify[2] << 8) + certificate_verify[3] + 4) != certificate_verify_len)
            || (0x0100 != ((certificate_verify[2] << 8) + certificate_verify[3]))) /* Header field */
        {
            error_code = TSIP_ERR_PARAMETER;
        }
        else
        {
            error_code = TSIP_SUCCESS;
        }

        tmp_public_key_type        = change_endian_long(0x00000003);
        tmp_public_key_output_type = change_endian_long(0x00000005);

        /* Casting uint32_t pointer is used to input. */
        p_tmp_signature_data = (uint32_t *) &certificate_verify[4];
    }
    else
    {
        error_code = TSIP_ERR_PARAMETER;
    }

    if (TSIP_SUCCESS == error_code)
    {
        certificate_length_sub = change_endian_long(handshake_message_len);

        /* Casting uint32_t pointer is used for address. */
        error_code = R_TSIP_TlsSVCertificateVerificationSub(
        /* uint32_t *InData_Sel_InDataPubKeyType, */&tmp_public_key_type,
        /* uint32_t *InData_Sel_OutDataPubKeyType,*/&tmp_public_key_output_type,
        /* uint32_t *InData_PubKey,               */key_index,
        /* uint32_t *InData_TBSCertificate,       */(uint32_t *)handshake_message,
        /* uint32_t *InData_TBSCertificateLength, */&certificate_length_sub,
        /* uint32_t *InData_Signature,            */p_tmp_signature_data,
        /* uint32_t *InData_TBSCertificatesInfo,  */NULL,
        /* uint32_t *OutData_PubKey               */NULL);
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_TlsSVCertificateVerifyVerification
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateTls13P256EccKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate P-256 ECC key index.
* @param[in]     handle handler for TLS1.3(work area).
* @param[in]     mode handshake protocol.
* @param[out]    key_index ephemeral ECC key index.
* @param[out]    ephemeral_ecdh_public_key ephemeral ECDH public key.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateTls13P256EccKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateTls13P256EccKeyIndex(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls_p256_ecc_key_index_t *key_index, uint8_t *ephemeral_ecdh_public_key)
{
    e_tsip_err_t error_code;
    uint32_t     indata_cmd_sub;

    /* Casting uint32_t is used to input. */
    indata_cmd_sub = change_endian_long((uint32_t)mode);

    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_GenerateTls13P256EccKeyIndexSub(
    /* uint32_t *InData_Cmd,        */&indata_cmd_sub,
    /* uint32_t *InData_Handle,     */handle->session_handle,
    /* uint32_t *OutData_KeyIndex,  */key_index->value,
    /* uint32_t *OutData_PubKey     */(uint32_t*)ephemeral_ecdh_public_key
    );

    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_TLS_P256_ECC_KEY;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateTls13P256EccKeyIndex
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13GenerateEcdheSharedSecret
*******************************************************************************************************************/ /**
* @details       Generate ECDHE shared secret.
* @param[in]     mode handshake protocol.
* @param[in]     server_public_key public key from server.
* @param[in]     key_index ephemeral ECC key index.
* @param[out]    shared_secret_key_index ephemeral shared secret key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Tls13GenerateEcdheSharedSecretSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13GenerateEcdheSharedSecret(e_tsip_tls13_mode_t mode, uint8_t *server_public_key,
        tsip_tls_p256_ecc_key_index_t *key_index,
        tsip_tls13_ephemeral_shared_secret_key_index_t *shared_secret_key_index)
{
    e_tsip_err_t error_code;
    uint32_t     indata_cmd_sub;

    if (TSIP_KEY_INDEX_TYPE_TLS_P256_ECC_KEY == key_index->type)
    {
        /* Casting uint32_t is used to input. */
        indata_cmd_sub = change_endian_long((uint32_t)mode);

        /* Casting uint32_t pointer is used for address. */
        error_code = R_TSIP_Tls13GenerateEcdheSharedSecretSub(
        /* uint32_t *InData_Cmd,                    */&indata_cmd_sub,
        /* uint32_t *InData_PubKey,                 */(uint32_t*)server_public_key,
        /* uint32_t *InData_KeyIndex,               */key_index->value,
        /* uint32_t *OutData_SharedSecretKeyIndex   */shared_secret_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            shared_secret_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_SHARED_SECRET_FOR_CLIENT;
        }
        else
        {
            shared_secret_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13GenerateEcdheSharedSecret
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13GenerateHandshakeSecret
*******************************************************************************************************************/ /**
* @details       Generate handshake secret.
* @param[in]     shared_secret_key_index ephemeral shared secret key index.
* @param[out]    handshake_secret_key_index ephemeral handshake secret key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Tls13GenerateHandshakeSecretSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13GenerateHandshakeSecret(
        tsip_tls13_ephemeral_shared_secret_key_index_t *shared_secret_key_index,
        tsip_tls13_ephemeral_handshake_secret_key_index_t *handshake_secret_key_index)
{
    e_tsip_err_t error_code;

    if (TSIP_KEY_INDEX_TYPE_TLS13_SHARED_SECRET_FOR_CLIENT == shared_secret_key_index->type)
    {
        error_code = R_TSIP_Tls13GenerateHandshakeSecretSub(
        /* uint32_t *InData_SharedSecretKeyIndex,       */shared_secret_key_index->value,
        /* uint32_t *OutData_Handshake_SecretKeyIndex   */handshake_secret_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            handshake_secret_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_HANDSHAKE_SECRET_FOR_CLIENT;
        }
        else
        {
            handshake_secret_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13GenerateHandshakeSecret
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13GenerateServerHandshakeTrafficKey
*******************************************************************************************************************/ /**
* @details       Generate server handshake traffic key index.
* @param[out]    handle handler for TLS1.3(work area).
* @param[in]     mode handshake protocol.
* @param[in]     handshake_secret_key_index ephemeral handshake secret key index.
* @param[in]     digest message hash calculated with SHA256
* @param[out]    server_write_key_index ephemeral server write key index.
* @param[out]    server_finished_key_index ephemeral server finished key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Tls13GenerateServerHandshakeTrafficKeySub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13GenerateServerHandshakeTrafficKey(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_handshake_secret_key_index_t *handshake_secret_key_index, uint8_t *digest,
        tsip_aes_key_index_t *server_write_key_index,
        tsip_tls13_ephemeral_server_finished_key_index_t *server_finished_key_index)
{
    e_tsip_err_t error_code;
    uint32_t     indata_cmd_sub;

    if (TSIP_KEY_INDEX_TYPE_TLS13_HANDSHAKE_SECRET_FOR_CLIENT == handshake_secret_key_index->type)
    {
        /* Casting uint32_t is used to input. */
        indata_cmd_sub = change_endian_long((uint32_t)mode);

        /* Casting uint32_t pointer is used for address. */
        error_code =  R_TSIP_Tls13GenerateServerHandshakeTrafficKeySub(
        /* uint32_t *InData_Cmd,                        */&indata_cmd_sub,
        /* uint32_t *InData_Handshake_SecretKeyIndex,   */handshake_secret_key_index->value,
        /* uint32_t *InData_MsgHash,                    */(uint32_t*)digest,
        /* uint32_t *OutData_ServerWriteKeyIndex,       */server_write_key_index->value,
        /* uint32_t *OutData_ServerFinishedKeyIndex     */server_finished_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            server_write_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_CLIENT;
            server_finished_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_SERVER_FINISHED_FOR_CLIENT;

            /* Reset seqnum for the 1st data reception from server. */
            handle->flag_handshake_rx = R_TSIP_TLS13_RESET_SEQNUM;
        }
        else
        {
            server_write_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
            server_finished_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13GenerateServerHandshakeTrafficKey
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13ServerHandshakeVerification
*******************************************************************************************************************/ /**
* @details       Verify finished data provided from server.
* @param[in]     mode handshake protocol.
* @param[in]     server_finished_key_index ephemeral finished write key index.
* @param[in]     digest message hash calculated with SHA256
* @param[in]     server_finished finished data provided from server.
* @param[out]    verify_data_index verification data of server handshake.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_VERIFICATION_FAIL: Verification NG
* @see           R_TSIP_Tls13ServerHandshakeVerificationSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13ServerHandshakeVerification(e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_server_finished_key_index_t *server_finished_key_index, uint8_t *digest,
        uint8_t *server_finished, uint32_t *verify_data_index)
{
    e_tsip_err_t error_code;
    uint32_t     indata_cmd_sub;

    if (TSIP_KEY_INDEX_TYPE_TLS13_SERVER_FINISHED_FOR_CLIENT == server_finished_key_index->type)
    {
        /* Casting uint32_t is used to input. */
        indata_cmd_sub = change_endian_long((uint32_t)mode);

        /* Casting uint32_t pointer is used for address. */
        error_code = R_TSIP_Tls13ServerHandshakeVerificationSub(
        /* uint32_t *InData_Cmd,                    */&indata_cmd_sub,
        /* uint32_t *InData_ServerFinishedKeyIndex, */server_finished_key_index->value,
        /* uint32_t *InData_MsgHash,                */(uint32_t *)digest,
        /* uint32_t *InData_ServerFinished,         */(uint32_t *)server_finished,
        /* uint32_t *OutData_VerifyDataIndex        */verify_data_index
        );
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13ServerHandshakeVerification
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13GenerateClientHandshakeTrafficKey
*******************************************************************************************************************/ /**
* @details       Generate client write key index and client finished key index.
* @param[out]    handle handler for TLS1.3(work area).
* @param[in]     mode handshake protocol.
* @param[in]     handshake_secret_key_index ephemeral handshake secret key index.
* @param[in]     digest message hash calculated with SHA256
* @param[out]    client_write_key_index ephemeral client write key index.
* @param[out]    client_finished_key_index ephemeral finished write key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Tls13GenerateClientHandshakeTrafficKeySub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13GenerateClientHandshakeTrafficKey(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_handshake_secret_key_index_t *handshake_secret_key_index, uint8_t *digest,
        tsip_aes_key_index_t *client_write_key_index, tsip_hmac_sha_key_index_t *client_finished_key_index)
{
    e_tsip_err_t error_code;
    uint32_t     indata_cmd_sub;

    if (TSIP_KEY_INDEX_TYPE_TLS13_HANDSHAKE_SECRET_FOR_CLIENT == handshake_secret_key_index->type)
    {
        /* Casting uint32_t is used to input. */
        indata_cmd_sub = change_endian_long((uint32_t)mode);

        /* Casting uint32_t pointer is used for address. */
        error_code = R_TSIP_Tls13GenerateClientHandshakeTrafficKeySub(
        /* uint32_t *InData_Cmd,                        */&indata_cmd_sub,
        /* uint32_t *InData_Handshake_SecretKeyIndex,   */handshake_secret_key_index->value,
        /* uint32_t *InData_MsgHash,                    */(uint32_t*)digest,
        /* uint32_t *OutData_ClientWriteKeyIndex,       */client_write_key_index->value,
        /* uint32_t *OutData_ClientFinishedKeyIndex     */client_finished_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            client_write_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_CLIENT;
            client_finished_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_FINISHED_FOR_CLIENT;
            /* Casting uint32_t is used to convert. */
            client_finished_key_index->tls13_mode = (uint32_t)mode;

            /* Reset seqnum for the 1st data transmission to server. */
            handle->flag_handshake_tx = R_TSIP_TLS13_RESET_SEQNUM;
        }
        else
        {
            client_write_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
            client_finished_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13GenerateClientHandshakeTrafficKey
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13GenerateMasterSecret
*******************************************************************************************************************/ /**
* @details       Generate master secret.
* @param[out]    handle handler for TLS1.3(work area).
* @param[in]     mode handshake protocol.
* @param[in]     handshake_secret_key_index ephemeral handshake secret key index.
* @param[in]     verify_data_index verification data of server handshake.
* @param[out]    master_secret_key_index ephemeral master secret key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Tls13GenerateMasterSecretSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13GenerateMasterSecret(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_handshake_secret_key_index_t *handshake_secret_key_index, uint32_t *verify_data_index,
        tsip_tls13_ephemeral_master_secret_key_index_t *master_secret_key_index)
{
    e_tsip_err_t error_code;
    uint32_t     indata_cmd_sub;

    if (TSIP_KEY_INDEX_TYPE_TLS13_HANDSHAKE_SECRET_FOR_CLIENT == handshake_secret_key_index->type)
    {
        /* Casting uint32_t is used to input. */
        indata_cmd_sub = change_endian_long((uint32_t)mode);

        error_code = R_TSIP_Tls13GenerateMasterSecretSub(
        /* uint32_t *InData_Cmd,                        */&indata_cmd_sub,
        /* uint32_t *InData_Handshake_SecretKeyIndex,   */handshake_secret_key_index->value,
        /* uint32_t *InData_VerifyDataIndex,            */verify_data_index,
        /* uint32_t *OutData_Handle,                    */handle->session_handle,
        /* uint32_t *OutData_Master_SecretKeyIndex      */master_secret_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            master_secret_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_MASTER_SECRET_FOR_CLIENT;
        }
        else
        {
            master_secret_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13GenerateMasterSecret
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13GenerateApplicationTrafficKey
*******************************************************************************************************************/ /**
* @details       Generate application traffic key index and application secret.
* @param[in,out] handle handler for TLS1.3(work area).
* @param[in]     mode handshake protocol.
* @param[in]     master_secret_key_index ephemeral master secret key index.
* @param[in]     digest message hash calculated with SHA256
* @param[out]    server_app_secret_key_index ephemeral server application secret key index.
* @param[out]    client_app_secret_key_index ephemeral client application secret key index.
* @param[out]    server_write_key_index ephemeral server write key index.
* @param[out]    client_write_key_index ephemeral client write key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Tls13GenerateApplicationTrafficKeySub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13GenerateApplicationTrafficKey(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_master_secret_key_index_t *master_secret_key_index, uint8_t *digest,
        tsip_tls13_ephemeral_app_secret_key_index_t *server_app_secret_key_index,
        tsip_tls13_ephemeral_app_secret_key_index_t *client_app_secret_key_index,
        tsip_aes_key_index_t *server_write_key_index, tsip_aes_key_index_t *client_write_key_index)
{
    e_tsip_err_t error_code;
    uint32_t     indata_cmd_sub;

    if (TSIP_KEY_INDEX_TYPE_TLS13_MASTER_SECRET_FOR_CLIENT == master_secret_key_index->type)
    {
        /* Casting uint32_t is used to input. */
        indata_cmd_sub = change_endian_long((uint32_t)mode);

        /* Casting uint32_t pointer is used for address. */
        error_code = R_TSIP_Tls13GenerateApplicationTrafficKeySub(
        /* uint32_t *InData_Cmd,                                */&indata_cmd_sub,
        /* uint32_t *InData_Handle,                             */handle->session_handle,
        /* uint32_t *InData_Master_SecretKeyIndex,              */master_secret_key_index->value,
        /* uint32_t *InData_MsgHash,                            */(uint32_t*)digest,
        /* uint32_t *OutData_ServerApplicationSecretKeyIndex,   */server_app_secret_key_index->value,
        /* uint32_t *OutData_ClientApplicationSecretKeyIndex,   */client_app_secret_key_index->value,
        /* uint32_t *OutData_ServerWriteKeyIndex,               */server_write_key_index->value,
        /* uint32_t *OutData_ClientWriteKeyIndex                */client_write_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            server_app_secret_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_APP_TRAFFIC_SECRET_FOR_CLIENT;
            client_app_secret_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_APP_TRAFFIC_SECRET_FOR_CLIENT;
            server_write_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_CLIENT;
            client_write_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_CLIENT;

            /* Reset seqnum for the 1st data reception from server. */
            handle->flag_application_rx = R_TSIP_TLS13_RESET_SEQNUM;
            /* Reset seqnum for the 1st data transmission to server. */
            handle->flag_application_tx = R_TSIP_TLS13_RESET_SEQNUM;
        }
        else
        {
            server_app_secret_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
            client_app_secret_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
            server_write_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
            client_write_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13GenerateApplicationTrafficKey
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13UpdateApplicationTrafficKey
*******************************************************************************************************************/ /**
* @details       Update application traffic key index and application secret.
* @param[in,out] handle handler for TLS1.3(work area).
* @param[in]     mode handshake protocol.
* @param[in]     key_type key type to update application traffic key.
* @param[in]     input_app_secret_key_index updating ephemeral server/client application secret key index.
* @param[out]    output_app_secret_key_index updated ephemeral server/client application secret key index.
* @param[out]    app_write_key_index ephemeral server/client write key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           R_TSIP_Tls13UpdateApplicationTrafficKeySub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13UpdateApplicationTrafficKey(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        e_tsip_tls13_update_key_type_t key_type,
        tsip_tls13_ephemeral_app_secret_key_index_t *input_app_secret_key_index,
        tsip_tls13_ephemeral_app_secret_key_index_t *output_app_secret_key_index,
        tsip_aes_key_index_t *app_write_key_index)
{
    e_tsip_err_t error_code;
    uint32_t     indata_cmd_sub;
    uint32_t     indata_key_type_sub;

    if (TSIP_KEY_INDEX_TYPE_TLS13_APP_TRAFFIC_SECRET_FOR_CLIENT == input_app_secret_key_index->type)
    {
        /* Casting uint32_t is used to input. */
        indata_cmd_sub = change_endian_long((uint32_t)mode);
        /* Casting uint32_t is used to input. */
        indata_key_type_sub = change_endian_long((uint32_t)key_type);

        error_code = R_TSIP_Tls13UpdateApplicationTrafficKeySub(
        /* uint32_t *InData_Cmd,                        */&indata_cmd_sub,
        /* uint32_t *InData_UpdateKeyType,              */&indata_key_type_sub,
        /* uint32_t *InData_Handle,                     */handle->session_handle,
        /* uint32_t *InData_ApplicationSecretKeyIndex,  */input_app_secret_key_index->value,
        /* uint32_t *OutData_ApplicationSecretKeyIndex, */output_app_secret_key_index->value,
        /* uint32_t *OutData_ApplicationWriteKeyIndex   */app_write_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            output_app_secret_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_APP_TRAFFIC_SECRET_FOR_CLIENT;
            if (TSIP_TLS13_UPDATE_SERVER_KEY == key_type)
            {
                app_write_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_CLIENT;

                /* Reset seqnum for the 1st data reception from server. */
                handle->flag_application_rx = R_TSIP_TLS13_RESET_SEQNUM;
            }
            else
            {
                app_write_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_CLIENT;

                /* Wait for 1st data transmission to server. */
                handle->flag_application_tx = R_TSIP_TLS13_RESET_SEQNUM;
            }
        }
        else
        {
            output_app_secret_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
            app_write_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }

    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13UpdateApplicationTrafficKey
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13EncryptInit
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES GCM/CCM algorithm with AES 128 key.
* @param[out]    handle Handler for TLS1.3(work area).
* @param[in]     phase handshake phase.
* @param[in]     mode handshake protocol.
* @param[in]     cipher_suite cipher suite.
* @param[in]     key_index ephemeral key index to encrypt.
* @param[in]     payload_length payload length.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128GcmEncryptInitPrivate()
* @see           R_TSIP_Aes128CcmEncryptInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13EncryptInit(tsip_tls13_handle_t *handle, e_tsip_tls13_phase_t phase, e_tsip_tls13_mode_t mode,
        e_tsip_tls13_cipher_suite_t cipher_suite, tsip_aes_key_index_t *key_index, uint32_t payload_length)
{
    e_tsip_err_t error_code;
    uint32_t     indata_header[TSIP_TLS13_DATA_A_INPUT_SIZE] =
    {
        0
    };

    if ((TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_CLIENT == key_index->type)
        || (TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_SERVER == key_index->type))
    {
        handle->plain_length = payload_length;

        if (TSIP_TLS13_CIPHER_SUITE_AES_128_GCM_SHA256 == cipher_suite)
        {
            memset(&handle->gcm_handle_tx, 0, sizeof(tsip_gcm_handle_t));
            handle->gcm_handle_tx.flag_call_init         = CALL_ONLY_UPDATE_FINAL;
            g_private_id_counter++;
            g_aes128gcmenc_private_id                    = g_private_id_counter;
            handle->gcm_handle_tx.id                     = g_private_id_counter;
            handle->gcm_handle_tx.flag_update_input_data = GCM_INPUT_DATA_AAD;

            error_code = R_TSIP_Aes128GcmEncryptInitPrivate(key_index, phase, mode, handle, NULL);
        }
        else /* TSIP_TLS13_CIPHER_SUITE_AES_128_CCM_SHA256 */
        {
            memset(&handle->ccm_handle_tx, 0, sizeof(tsip_ccm_handle_t));
            handle->ccm_handle_tx.flag_call_init = CALL_ONLY_UPDATE_FINAL;
            g_private_id_counter++;
            g_aes128ccmenc_private_id            = g_private_id_counter;
            handle->ccm_handle_tx.id             = g_private_id_counter;
            handle->ccm_handle_tx.formatted_data[0] = (((TSIP_TLS13_TAG_SIZE - 2) / 2) << 3) | (3 - 1);

            indata_header[0] = change_endian_long(
                TSIP_TLS13_DATA_A_PREFIX | (((payload_length + TSIP_TLS13_TAG_SIZE) & 0x0000FF00) >> 8));
            indata_header[1] = change_endian_long(((payload_length + TSIP_TLS13_TAG_SIZE) & 0x000000FF) << 24);

            error_code = R_TSIP_Aes128CcmEncryptInitPrivate(key_index, phase, mode, handle, payload_length,
                NULL, indata_header, TSIP_TLS13_DATA_A_INPUT_SIZE);
        }

        handle->cipher_suite = cipher_suite;
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13EncryptInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13EncryptUpdate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES GCM/CCM mode algorithm with AES 128 key.
* @param[in,out] handle Handler for TLS1.3(work area).
* @param[in]     plain Input plain text.
* @param[out]    cipher Output cipher text.
* @param[in]     plain_length Input byte size of plain.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128GcmEncryptUpdate()
* @see           R_TSIP_Aes128CcmEncryptUpdate()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13EncryptUpdate(tsip_tls13_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_length)
{
    e_tsip_err_t error_code;
    uint8_t      indata_aad[TSIP_TLS13_DATA_A_BYTE_SIZE] =
    {
        0
    };
    uint32_t     indata_aad_len = 0;

    if (TSIP_TLS13_CIPHER_SUITE_AES_128_GCM_SHA256 == handle->cipher_suite)
    {
        if (GCM_INPUT_DATA_AAD == handle->gcm_handle_tx.flag_update_input_data)
        {
            indata_aad[0]  = TSIP_TLS13_DATA_A_TYPE;
            indata_aad[1]  = TSIP_TLS13_DATA_A_VERSION_1ST;
            indata_aad[2]  = TSIP_TLS13_DATA_A_VERSION_2ND;
            /* Casting uint8_t is used to input. */
            indata_aad[3]  = (uint8_t)(((handle->plain_length + TSIP_TLS13_TAG_SIZE) & 0x0000FF00) >> 8);
            /* Casting uint8_t is used to input. */
            indata_aad[4]  = (uint8_t)((handle->plain_length + TSIP_TLS13_TAG_SIZE) & 0x000000FF);
            indata_aad_len = TSIP_TLS13_DATA_A_BYTE_SIZE;
        }

        error_code = R_TSIP_Aes128GcmEncryptUpdate(&handle->gcm_handle_tx, plain, cipher, plain_length, indata_aad,
            indata_aad_len);
    }
    else /* TSIP_TLS13_CIPHER_SUITE_AES_128_CCM_SHA256 */
    {
        error_code = R_TSIP_Aes128CcmEncryptUpdate(&handle->ccm_handle_tx, plain, cipher, plain_length);
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13EncryptUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13EncryptFinal
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES GCM/CCM mode algorithm with AES 128 key.
* @param[in,out] handle Handler for TLS1.3(work area).
* @param[out]    cipher Output cipher text.
* @param[out]    cipher_length Output byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128GcmEncryptFinal()
* @see           R_TSIP_Aes128CcmEncryptFinal()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13EncryptFinal(tsip_tls13_handle_t *handle, uint8_t *cipher, uint32_t *cipher_length)
{
    e_tsip_err_t error_code;

    if (TSIP_TLS13_CIPHER_SUITE_AES_128_GCM_SHA256 == handle->cipher_suite)
    {
        error_code = R_TSIP_Aes128GcmEncryptFinal(&handle->gcm_handle_tx, cipher, cipher_length,
            &cipher[handle->plain_length % R_TSIP_AES_BLOCK_BYTE_SIZE]);
    }
    else /* TSIP_TLS13_CIPHER_SUITE_AES_128_CCM_SHA256 */
    {
        error_code = R_TSIP_Aes128CcmEncryptFinal(&handle->ccm_handle_tx, cipher, cipher_length,
            &cipher[handle->plain_length % R_TSIP_AES_BLOCK_BYTE_SIZE], TSIP_TLS13_TAG_SIZE);
    }

    if (TSIP_SUCCESS == error_code)
    {
        (*cipher_length) += TSIP_TLS13_TAG_SIZE;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13EncryptFinal
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13DecryptInit
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using AES GCM/CCM algorithm with AES 128 key.
* @param[out]    handle Handler for TLS1.3(work area).
* @param[in]     phase handshake phase.
* @param[in]     mode handshake protocol.
* @param[in]     cipher_suite cipher suite.
* @param[in]     key_index ephemeral key index to decrypt.
* @param[in]     payload_length payload length.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128GcmDecryptInitPrivate()
* @see           R_TSIP_Aes128CcmDecryptInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13DecryptInit(tsip_tls13_handle_t *handle, e_tsip_tls13_phase_t phase, e_tsip_tls13_mode_t mode,
        e_tsip_tls13_cipher_suite_t cipher_suite, tsip_aes_key_index_t *key_index, uint32_t payload_length)
{
    e_tsip_err_t error_code;
    uint32_t     indata_header[TSIP_TLS13_DATA_A_INPUT_SIZE] =
    {
        0
    };
    uint32_t     cipher_length = 0;

    if ((TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_CLIENT == key_index->type)
        || (TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_SERVER == key_index->type))
    {
        handle->cipher_length = payload_length;

        if (TSIP_TLS13_CIPHER_SUITE_AES_128_GCM_SHA256 == cipher_suite)
        {
            memset(&handle->gcm_handle_rx, 0, sizeof(tsip_gcm_handle_t));
            handle->gcm_handle_rx.flag_call_init         = CALL_ONLY_UPDATE_FINAL;
            g_private_id_counter++;
            g_aes128gcmdec_private_id                    = g_private_id_counter;
            handle->gcm_handle_rx.id                     = g_private_id_counter;
            handle->gcm_handle_rx.flag_update_input_data = GCM_INPUT_DATA_AAD;

            error_code = R_TSIP_Aes128GcmDecryptInitPrivate(key_index, phase, mode, handle, NULL);
        }
        else /* TSIP_TLS13_CIPHER_SUITE_AES_128_CCM_SHA256 */
        {
            memset(&handle->ccm_handle_rx, 0, sizeof(tsip_ccm_handle_t));
            handle->ccm_handle_rx.flag_call_init = CALL_ONLY_UPDATE_FINAL;
            g_private_id_counter++;
            g_aes128ccmdec_private_id            = g_private_id_counter;
            handle->ccm_handle_rx.id             = g_private_id_counter;
            handle->ccm_handle_rx.formatted_data[0] = (((TSIP_TLS13_TAG_SIZE - 2) / 2) << 3) | (3 - 1);

            cipher_length    = payload_length - TSIP_TLS13_TAG_SIZE;
            indata_header[0] = change_endian_long(TSIP_TLS13_DATA_A_PREFIX | ((payload_length & 0x0000FF00) >> 8));
            indata_header[1] = change_endian_long((payload_length & 0x000000FF) << 24);

            error_code = R_TSIP_Aes128CcmDecryptInitPrivate(key_index, phase, mode, handle, cipher_length,
                0, NULL, indata_header, TSIP_TLS13_DATA_A_INPUT_SIZE);
        }

        handle->cipher_suite     = cipher_suite;
        handle->cipher_tag_index = 0;
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13DecryptInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13DecryptUpdate
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using AES GCM/CCM mode algorithm with AES 128 key.
* @param[in,out] handle Handler for TLS1.3(work area).
* @param[in]     cipher Input cipher text.
* @param[out]    plain Output plain text.
* @param[in]     cipher_length Input byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Aes128GcmDecryptUpdate()
* @see           R_TSIP_Aes128CcmDecryptUpdate()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13DecryptUpdate(tsip_tls13_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_length)
{
    e_tsip_err_t error_code      = TSIP_SUCCESS;
    uint32_t     indata_cipher_length = 0;
    uint32_t     tag_part_length = 0;
    uint8_t      indata_aad[TSIP_TLS13_DATA_A_BYTE_SIZE] =
    {
        0
    };
    uint32_t     indata_aad_len  = 0;

    if (TSIP_TLS13_CIPHER_SUITE_AES_128_GCM_SHA256 == handle->cipher_suite)
    {
        if ((handle->cipher_length - TSIP_TLS13_TAG_SIZE)
                <= (handle->gcm_handle_rx.all_received_length + cipher_length))
        {
            indata_cipher_length = cipher_length
                - ((handle->gcm_handle_rx.all_received_length + cipher_length)
                - (handle->cipher_length - TSIP_TLS13_TAG_SIZE));
            tag_part_length      = cipher_length - indata_cipher_length;
        }
        else
        {
            indata_cipher_length = cipher_length;
        }

        if (GCM_INPUT_DATA_AAD == handle->gcm_handle_rx.flag_update_input_data)
        {
            indata_aad[0]  = TSIP_TLS13_DATA_A_TYPE;
            indata_aad[1]  = TSIP_TLS13_DATA_A_VERSION_1ST;
            indata_aad[2]  = TSIP_TLS13_DATA_A_VERSION_2ND;
            /* Casting uint8_t is used to input. */
            indata_aad[3]  = (uint8_t)((handle->cipher_length & 0x0000FF00) >> 8);
            /* Casting uint8_t is used to input. */
            indata_aad[4]  = (uint8_t)(handle->cipher_length & 0x000000FF);
            indata_aad_len = TSIP_TLS13_DATA_A_BYTE_SIZE;
        }

        error_code = R_TSIP_Aes128GcmDecryptUpdate(&handle->gcm_handle_rx, cipher, plain, indata_cipher_length,
            indata_aad, indata_aad_len);
    }
    else /* TSIP_TLS13_CIPHER_SUITE_AES_128_CCM_SHA256 */
    {
        if ((handle->cipher_length - TSIP_TLS13_TAG_SIZE)
                <= (handle->ccm_handle_rx.all_received_length + cipher_length))
        {
            indata_cipher_length = cipher_length
                - ((handle->ccm_handle_rx.all_received_length + cipher_length)
                - (handle->cipher_length - TSIP_TLS13_TAG_SIZE));
            tag_part_length      = cipher_length - indata_cipher_length;
        }
        else
        {
            indata_cipher_length = cipher_length;
        }

        if (0 != indata_cipher_length)
        {
            error_code = R_TSIP_Aes128CcmDecryptUpdate(&handle->ccm_handle_rx, cipher, plain, indata_cipher_length);
        }
    }

    if ((TSIP_SUCCESS == error_code) && (0 != tag_part_length))
    {
        if ((handle->cipher_tag_index + tag_part_length) <= TSIP_TLS13_TAG_SIZE)
        {
            memcpy(&handle->cipher_tag[handle->cipher_tag_index], &cipher[cipher_length - tag_part_length],
                tag_part_length);
            handle->cipher_tag_index += tag_part_length;
        }
        else
        {
            error_code = TSIP_ERR_PARAMETER;
        }
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13DecryptUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13DecryptFinal
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using AES GCM/CCM mode algorithm with AES 128 key.
* @param[in,out] handle Handler for TLS1.3(work area).
* @param[out]    plain Output plain text.
* @param[out]    plain_length Output byte size of plain.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @retval        TSIP_ERR_AUTHENTICATION: Authentication NG
* @see           R_TSIP_Aes128GcmDecryptFinal()
* @see           R_TSIP_Aes128CcmDecryptFinal()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13DecryptFinal(tsip_tls13_handle_t *handle, uint8_t *plain, uint32_t *plain_length)
{
    e_tsip_err_t error_code;

    if (TSIP_TLS13_CIPHER_SUITE_AES_128_GCM_SHA256 == handle->cipher_suite)
    {
        error_code = R_TSIP_Aes128GcmDecryptFinal(&handle->gcm_handle_rx, plain, plain_length, handle->cipher_tag,
            TSIP_TLS13_TAG_SIZE);
    }
    else /* TSIP_TLS13_CIPHER_SUITE_AES_128_CCM_SHA256 */
    {
        error_code = R_TSIP_Aes128CcmDecryptFinal(&handle->ccm_handle_rx, plain, plain_length, handle->cipher_tag,
            TSIP_TLS13_TAG_SIZE);
    }

    /* Avoid receiving unacceptable early data */
    if (((TSIP_ERR_AUTHENTICATION == error_code) || (TSIP_ERR_FAIL == error_code))
        && (0 == handle->seqnum_handshake_rx))
    {
        handle->flag_handshake_rx = R_TSIP_TLS13_RESET_SEQNUM;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13DecryptFinal
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13GenerateResumptionMasterSecret
*******************************************************************************************************************/ /**
* @details       Generate resumption master secret.
* @param[in]     handle handler for TLS1.3(work area).
* @param[in]     mode handshake protocol.
* @param[in]     master_secret_key_index ephemeral master secret key index.
* @param[in]     digest message hash calculated with SHA256
* @param[out]    res_master_secret_key_index ephemeral resumption master secret key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Tls13GenerateResumptionMasterSecretSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13GenerateResumptionMasterSecret(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_master_secret_key_index_t *master_secret_key_index, uint8_t *digest,
        tsip_tls13_ephemeral_res_master_secret_key_index_t *res_master_secret_key_index)
{
    e_tsip_err_t error_code;
    uint32_t     indata_cmd_sub;

    if (TSIP_KEY_INDEX_TYPE_TLS13_MASTER_SECRET_FOR_CLIENT == master_secret_key_index->type)
    {
        /* Casting uint32_t is used to input. */
        indata_cmd_sub          = change_endian_long((uint32_t)mode);

        /* Casting uint32_t pointer is used for address. */
        error_code = R_TSIP_Tls13GenerateResumptionMasterSecretSub(
        /* uint32_t *InData_Cmd,                    */&indata_cmd_sub,
        /* uint32_t *InData_Handle,                 */handle->session_handle,
        /* uint32_t *InData_Master_SecretKeyIndex,  */master_secret_key_index->value,
        /* uint32_t *InData_MsgHash,                */(uint32_t*)digest,
        /* uint32_t *OutData_RMaster_SecretKeyIndex */res_master_secret_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            res_master_secret_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_RESUMPTION_MASTER_SECRET_FOR_CLIENT;
        }
        else
        {
            res_master_secret_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13GenerateResumptionMasterSecret
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13GeneratePreSharedKey
*******************************************************************************************************************/ /**
* @details       Generate pre shared key index.
* @param[in]     handle handler for TLS1.3(work area).
* @param[in]     mode handshake protocol which the master secret generated.
* @param[in]     res_master_secret_key_index ephemeral resumption master secret key index.
* @param[in]     ticket_nonce ticket nonce.
* @param[in]     ticket_nonce_len lencth of ticket nonce.
* @param[out]    pre_shared_key_index ephemeral pre shared key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Tls13GeneratePreSharedKeySub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13GeneratePreSharedKey(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_res_master_secret_key_index_t *res_master_secret_key_index, uint8_t *ticket_nonce,
        uint32_t ticket_nonce_len, tsip_tls13_ephemeral_pre_shared_key_index_t * pre_shared_key_index)
{
    e_tsip_err_t error_code;
    uint32_t     indata_cmd_sub;
    uint32_t     indata_ticket_nonce_len;

    if (TSIP_KEY_INDEX_TYPE_TLS13_RESUMPTION_MASTER_SECRET_FOR_CLIENT == res_master_secret_key_index->type)
    {
        /* Casting uint32_t is used to input. */
        indata_cmd_sub          = change_endian_long((uint32_t)mode);
        indata_ticket_nonce_len = change_endian_long(ticket_nonce_len);

        /* Casting uint32_t pointer is used for address. */
        error_code = R_TSIP_Tls13GeneratePreSharedKeySub(
        /* uint32_t *InData_Cmd,                    */&indata_cmd_sub,
        /* uint32_t *InData_Handle,                 */handle->session_handle,
        /* uint32_t *InData_RMaster_SecretKeyIndex, */res_master_secret_key_index->value,
        /* uint32_t *InData_TicketNonceLen,         */&indata_ticket_nonce_len,
        /* uint32_t *InData_TicketNonce,            */(uint32_t*)ticket_nonce,
        /* uint32_t *OutData_PreSharedKeyIndex      */pre_shared_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            pre_shared_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_PRE_SHARED_KEY_FOR_CLIENT;
        }
        else
        {
            pre_shared_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13GeneratePreSharedKey
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13GeneratePskBinderKey
*******************************************************************************************************************/ /**
* @details       Generate psk binder key index.
* @param[in]     handle handler for TLS1.3(work area).
* @param[in]     pre_shared_key_index ephemeral pre shared key index.
* @param[out]    psk_binder_key_index ephemeral psk binder key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Tls13GeneratePskBinderKeySub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13GeneratePskBinderKey(tsip_tls13_handle_t *handle,
        tsip_tls13_ephemeral_pre_shared_key_index_t * pre_shared_key_index,
        tsip_hmac_sha_key_index_t * psk_binder_key_index)
{
    e_tsip_err_t error_code;

    if (TSIP_KEY_INDEX_TYPE_TLS13_PRE_SHARED_KEY_FOR_CLIENT == pre_shared_key_index->type)
    {
        error_code = R_TSIP_Tls13GeneratePskBinderKeySub(
        /* uint32_t *InData_Handle,             */handle->session_handle,
        /* uint32_t *InData_PreSharedKeyIndex,  */pre_shared_key_index->value,
        /* uint32_t *OutData_BinderKeyIndex     */psk_binder_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            psk_binder_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_PSK_BINDER_KEY_FOR_CLIENT;
        }
        else
        {
            psk_binder_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13GeneratePskBinderKey
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13Generate0RttApplicationWriteKey
*******************************************************************************************************************/ /**
* @details       Generate client application traffic key index to use in 0-RTT.
* @param[in,out] handle handler for TLS1.3(work area).
* @param[in]     pre_shared_key_index ephemeral pre shared key index.
* @param[in]     digest message hash calculated with SHA256
* @param[out]    client_write_key_index ephemeral client write key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Tls13Generate0RttApplicationWriteKeySub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13Generate0RttApplicationWriteKey(tsip_tls13_handle_t *handle,
        tsip_tls13_ephemeral_pre_shared_key_index_t * pre_shared_key_index,
        uint8_t *digest, tsip_aes_key_index_t *client_write_key_index)
{
    e_tsip_err_t error_code;

    if (TSIP_KEY_INDEX_TYPE_TLS13_PRE_SHARED_KEY_FOR_CLIENT == pre_shared_key_index->type)
    {
        error_code = R_TSIP_Tls13Generate0RttApplicationWriteKeySub(
        /* uint32_t *InData_Handle,                 */handle->session_handle,
        /* uint32_t *InData_PreSharedKeyIndex,      */pre_shared_key_index->value,
        /* uint32_t *InData_MsgHash,                */(uint32_t*)digest,
        /* uint32_t *OutData_ClientWriteKeyIndex    */client_write_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            client_write_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_CLIENT;

            /* Wait for 1st data transmission to server. */
            handle->flag_application_tx = R_TSIP_TLS13_RESET_SEQNUM;
        }
        else
        {
            client_write_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13Generate0RttApplicationWriteKey
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13GenerateResumptionHandshakeSecret
*******************************************************************************************************************/ /**
* @details       Generate resumption handshake secret.
* @param[in]     handle handler for TLS1.3(work area).
* @param[in]     mode handshake protocol which the shared secret generated.
* @param[in]     pre_shared_key_index ephemeral pre shared key index.
* @param[in]     shared_secret_key_index ephemeral shared secret key index.
* @param[out]    handshake_secret_key_index ephemeral handshake secret key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Tls13GenerateResumptionHandshakeSecretSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13GenerateResumptionHandshakeSecret(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_pre_shared_key_index_t * pre_shared_key_index,
        tsip_tls13_ephemeral_shared_secret_key_index_t *shared_secret_key_index,
        tsip_tls13_ephemeral_handshake_secret_key_index_t *handshake_secret_key_index)
{
    e_tsip_err_t error_code;
    uint32_t     indata_cmd_sub;

    if ((TSIP_KEY_INDEX_TYPE_TLS13_PRE_SHARED_KEY_FOR_CLIENT == pre_shared_key_index->type)
        && (TSIP_KEY_INDEX_TYPE_TLS13_SHARED_SECRET_FOR_CLIENT == shared_secret_key_index->type))
    {
        /* Casting uint32_t is used to input. */
        indata_cmd_sub = change_endian_long((uint32_t)mode);

        error_code = R_TSIP_Tls13GenerateResumptionHandshakeSecretSub(
        /* uint32_t *InData_Handle,                     */handle->session_handle,
        /* uint32_t *InData_SharedSecretKeyIndexType,   */&indata_cmd_sub,
        /* uint32_t *InData_PreSharedKeyIndex,          */pre_shared_key_index->value,
        /* uint32_t *InData_SharedSecretKeyIndex,       */shared_secret_key_index->value,
        /* uint32_t *OutData_Handshake_SecretKeyIndex   */handshake_secret_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            handshake_secret_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_HANDSHAKE_SECRET_FOR_CLIENT;
        }
        else
        {
            handshake_secret_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13GenerateResumptionHandshakeSecret
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13CertificateVerifyGenerate
*******************************************************************************************************************/ /**
* @details       Generate Certificate Verify data to help TLS1.3 application.
* @param[in]     key_index user secret key index.
* @param[in]     signature_scheme signature algorithm.
* @param[in]     digest message hash calculated with SHA256
* @param[out]    certificate_verify Certificate Verify data.
* @param[out]    certificate_verify_len length of Certificate Verify data.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           gen_sign_ecdsa()
* @see           gen_sign_rsa_pss()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13CertificateVerifyGenerate(uint32_t *key_index,
        e_tsip_tls13_signature_scheme_type_t signature_scheme, uint8_t *digest, uint8_t *certificate_verify,
        uint32_t *certificate_verify_len)
{
    e_tsip_err_t            error_code                                                 = TSIP_SUCCESS;
    uint8_t                 indata_msg_dgst[TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE];
    uint8_t                 signature_data[TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE * 2] =
    {
        0
    };
    uint32_t                signature_len;
    uint8_t                 out_signature_index = 0;
    uint8_t                 output_index        = 0;
    uint8_t                 r_field_length      = 0;
    uint8_t                 s_field_length      = 0;

    if (TSIP_TLS13_SIGNATURE_SCHEME_ECDSA_SECP256R1_SHA256 == signature_scheme)
    {
        memcpy(indata_msg_dgst, s_certificate_verify_content, TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE - 32);
        memcpy(&indata_msg_dgst[73], s_certificate_verify_content_client, 6);
        memcpy(&indata_msg_dgst[TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE-R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE], digest,
            R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE);

        /* Casting tsip_ecc_public_key_index_t pointer is used to adjust key type. */
        error_code = gen_sign_ecdsa((tsip_ecc_private_key_index_t *)key_index, indata_msg_dgst,
            TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE, signature_data, &signature_len);
        if (TSIP_SUCCESS == error_code)
        {
            /* Signature Algorithm field */
            certificate_verify[0] = TSIP_TLS13_SIG_ALG_HASH_SHA256;
            certificate_verify[1] = TSIP_TLS13_SIG_ALG_SIG_ECDSA;

            /* Integer r field */
            output_index   = 8; /* position of r data */
            r_field_length = TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE;
            for (out_signature_index = 0; out_signature_index < TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE;
                    out_signature_index++)
            {
                if (0x00 == signature_data[out_signature_index])
                {
                    r_field_length--;
                }
                else
                {
                    break;
                }
            }
            if (0x80 == (signature_data[out_signature_index] & 0x80))
            {
                certificate_verify[output_index] = 0x00;
                output_index++;
                r_field_length += 1;
            }
            memcpy(&certificate_verify[output_index], &signature_data[out_signature_index],
                (TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE - out_signature_index));
            certificate_verify[6] = TSIP_TLS13_ASN1_TAG_INTEGER;
            certificate_verify[7] = r_field_length;

            /* Integer s field */
            output_index   = 10 + r_field_length; /* position of s data */
            s_field_length = TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE;
            for (out_signature_index = TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE;
                    out_signature_index < (TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE *2); out_signature_index++)
            {
                if (0x00 == signature_data[out_signature_index])
                {
                    s_field_length--;
                }
                else
                {
                    break;
                }
            }
            if (0x80 == (signature_data[out_signature_index] & 0x80))
            {
                certificate_verify[output_index] = 0x00;
                output_index++;
                s_field_length += 1;
            }
            memcpy(&certificate_verify[output_index], &signature_data[out_signature_index],
                (TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE * 2 - out_signature_index));
            certificate_verify[8 + r_field_length] = TSIP_TLS13_ASN1_TAG_INTEGER;
            certificate_verify[9 + r_field_length] = s_field_length;

            /* Header field */
            certificate_verify[2] = 0x00;
            certificate_verify[3] = 6 + r_field_length + s_field_length;
            certificate_verify[4] = TSIP_TLS13_ASN1_TAG_SEQUENCE;
            certificate_verify[5] = 4 + r_field_length + s_field_length;

            *certificate_verify_len = 10 + r_field_length + s_field_length;
        }
    }
    else if (TSIP_TLS13_SIGNATURE_SCHEME_RSA_PSS_RSAE_SHA256 == signature_scheme)
    {
        /* Signature Algorithm field */
        certificate_verify[0] = TSIP_TLS13_SIG_ALG_SIG_RSA_PSS;
        certificate_verify[1] = TSIP_TLS13_SIG_ALG_HASH_SHA256;

        /* length field */
        certificate_verify[2] = 0x01;
        certificate_verify[3] = 0x00;

        memcpy(indata_msg_dgst, s_certificate_verify_content, TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE - 32);
        memcpy(&indata_msg_dgst[73], s_certificate_verify_content_client, 6);
        memcpy(&indata_msg_dgst[TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE-R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE], digest,
            R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE);

        /* Casting tsip_rsa2048_private_key_index_t pointer is used to input. */
        error_code = gen_sign_rsa_pss((tsip_rsa2048_private_key_index_t *)key_index, indata_msg_dgst,
            TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE, &certificate_verify[4], certificate_verify_len);

        if (TSIP_SUCCESS == error_code)
        {
            (*certificate_verify_len) += 4;
        }
    }
    else
    {
        error_code = TSIP_ERR_PARAMETER;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13CertificateVerifyGenerate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13CertificateVerifyVerification
*******************************************************************************************************************/ /**
* @details       Verify Certificate Verify data to help TLS1.3 application.
* @param[in]     key_index encrypted public key.
* @param[in]     signature_scheme signature algorithm.
* @param[in]     digest message hash calculated with SHA256
* @param[in]     certificate_verify Certificate Verify data.
* @param[in]     certificate_verify_len length of Certificate Verify data.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           R_TSIP_TlsCertificateVerificationSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13CertificateVerifyVerification(uint32_t *key_index,
        e_tsip_tls13_signature_scheme_type_t signature_scheme, uint8_t *digest, uint8_t *certificate_verify,
        uint32_t certificate_verify_len)
{
    e_tsip_err_t            error_code                                                 = TSIP_SUCCESS;
    uint8_t                 indata_msg_dgst[TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE];
    uint8_t                 signature_data[TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE * 2] =
    {
        0
    };
    uint8_t                 input_index                = 0;
    uint8_t                 output_index               = 0;
    uint8_t                 length_index               = 0;
    uint8_t                 copy_length                = 0;
    uint32_t                certificate_length_sub     = 0;
    uint32_t                tmp_public_key_type        = 0;
    uint32_t                tmp_public_key_output_type = 0;
    uint32_t              * p_tmp_signature_data;

    if (TSIP_TLS13_SIGNATURE_SCHEME_ECDSA_SECP256R1_SHA256 == signature_scheme)
    {
        if ((TSIP_TLS13_SIG_ALG_HASH_SHA256 != certificate_verify[0])
            || (TSIP_TLS13_SIG_ALG_SIG_ECDSA != certificate_verify[1])) /* Signature Algorithm field */
        {
            error_code = TSIP_ERR_PARAMETER;
        }
        else if ((((certificate_verify[3] + 4) != certificate_verify_len))
                || ((certificate_verify[5] + 6) != certificate_verify_len)) /* Header field */
        {
            error_code = TSIP_ERR_PARAMETER;
        }
        else
        {
            error_code = TSIP_SUCCESS;
        }

        /* Integer r field */
        if (TSIP_SUCCESS == error_code)
        {
            length_index = 7;

            if ((TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE + 1) == certificate_verify[length_index])
            {
                input_index  = length_index + 2;
                output_index = 0;
                copy_length  = TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE;
            }
            else if (TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE >= certificate_verify[length_index])
            {
                input_index  = length_index + 1;
                output_index = TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE - certificate_verify[length_index];
                copy_length  = certificate_verify[length_index];
            }
            else
            {
                error_code = TSIP_ERR_PARAMETER;
            }

            if (TSIP_SUCCESS == error_code)
            {
                memcpy(&signature_data[output_index], &certificate_verify[input_index], copy_length);
            }
        }

        /* Integer s field */
        if (TSIP_SUCCESS == error_code)
        {
            length_index = input_index + copy_length + 1;

            if ((TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE + 1) == certificate_verify[length_index])
            {
                input_index  = length_index + 2;
                output_index = TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE;
                copy_length  = TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE;
            }
            else if (TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE >= certificate_verify[length_index])
            {
                input_index  = length_index + 1;
                output_index = (TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE *2) - certificate_verify[length_index];
                copy_length  = certificate_verify[length_index];
            }
            else
            {
                error_code = TSIP_ERR_PARAMETER;
            }

            if (TSIP_SUCCESS == error_code)
            {
                memcpy(&signature_data[output_index], &certificate_verify[input_index], copy_length);
            }
        }

        tmp_public_key_type        = change_endian_long(0x00000002);
        tmp_public_key_output_type = change_endian_long(0x00000005);

        /* Casting uint32_t pointer is used to input. */
        p_tmp_signature_data = (uint32_t *)signature_data;
    }
    else if (TSIP_TLS13_SIGNATURE_SCHEME_RSA_PSS_RSAE_SHA256 == signature_scheme)
    {
        if ((TSIP_TLS13_SIG_ALG_SIG_RSA_PSS != certificate_verify[0])
            || (TSIP_TLS13_SIG_ALG_HASH_SHA256 != certificate_verify[1])) /* Signature Algorithm field */
        {
            error_code = TSIP_ERR_PARAMETER;
        }
        else if ((((certificate_verify[2] << 8) + certificate_verify[3] + 4) != certificate_verify_len)
            || (0x0100 != ((certificate_verify[2] << 8) + certificate_verify[3]))) /* Header field */
        {
            error_code = TSIP_ERR_PARAMETER;
        }
        else
        {
            error_code = TSIP_SUCCESS;
        }

        tmp_public_key_type        = change_endian_long(0x00000003);
        tmp_public_key_output_type = change_endian_long(0x00000005);

        /* Casting uint32_t pointer is used to input. */
        p_tmp_signature_data = (uint32_t *) &certificate_verify[4];
    }
    else
    {
        error_code = TSIP_ERR_PARAMETER;
    }

    if (TSIP_SUCCESS == error_code)
    {
        memcpy(indata_msg_dgst, s_certificate_verify_content, TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE - 32);
        memcpy(&indata_msg_dgst[73], s_certificate_verify_content_server, 6);
        memcpy(&indata_msg_dgst[TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE-R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE], digest,
            R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE);
        certificate_length_sub = change_endian_long(TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE);

        /* Casting uint32_t pointer is used for address. */
        error_code = R_TSIP_TlsCertificateVerificationSub(
        /* uint32_t *InData_Sel_InDataPubKeyType, */&tmp_public_key_type,
        /* uint32_t *InData_Sel_OutDataPubKeyType,*/&tmp_public_key_output_type,
        /* uint32_t *InData_PubKey,               */key_index,
        /* uint32_t *InData_TBSCertificate,       */(uint32_t *)indata_msg_dgst,
        /* uint32_t *InData_TBSCertificateLength, */&certificate_length_sub,
        /* uint32_t *InData_Signature,            */p_tmp_signature_data,
        /* uint32_t *InData_TBSCertificatesInfo,  */NULL,
        /* uint32_t *OutData_PubKey               */NULL);
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13CertificateVerifyVerification
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateTls13SVP256EccKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate P-256 ECC key index for server.
* @param[in]     handle handler for TLS1.3(work area).
* @param[in]     mode handshake protocol.
* @param[out]    key_index ephemeral ECC key index.
* @param[out]    ephemeral_ecdh_public_key ephemeral ECDH public key.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateTls13SVP256EccKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateTls13SVP256EccKeyIndex(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls_p256_ecc_key_index_t *key_index, uint8_t *ephemeral_ecdh_public_key)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t     indata_cmd_sub;

    /* Casting uint32_t is used to input. */
    indata_cmd_sub = change_endian_long((uint32_t)mode);

    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_GenerateTls13SVP256EccKeyIndexSub(
    /* uint32_t *InData_Cmd,        */&indata_cmd_sub,
    /* uint32_t *InData_Handle,     */handle->session_handle,
    /* uint32_t *OutData_KeyIndex,  */key_index->value,
    /* uint32_t *OutData_PubKey     */(uint32_t*)ephemeral_ecdh_public_key
    );

    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_TLS_P256_ECC_KEY;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateTls13SVP256EccKeyIndex
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13SVGenerateEcdheSharedSecret
*******************************************************************************************************************/ /**
* @details       Generate ECDHE shared secret for server.
* @param[in]     mode handshake protocol.
* @param[in]     client_public_key public key from client.
* @param[in]     key_index ephemeral ECC key index.
* @param[out]    shared_secret_key_index ephemeral shared secret key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Tls13SVGenerateEcdheSharedSecretSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13SVGenerateEcdheSharedSecret(e_tsip_tls13_mode_t mode, uint8_t *client_public_key,
        tsip_tls_p256_ecc_key_index_t *key_index,
        tsip_tls13_ephemeral_shared_secret_key_index_t *shared_secret_key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t     indata_cmd_sub;

    if (TSIP_KEY_INDEX_TYPE_TLS_P256_ECC_KEY == key_index->type)
    {
        /* Casting uint32_t is used to input. */
        indata_cmd_sub = change_endian_long((uint32_t)mode);

        /* Casting uint32_t pointer is used for address. */
        error_code = R_TSIP_Tls13SVGenerateEcdheSharedSecretSub(
        /* uint32_t *InData_Cmd,                    */&indata_cmd_sub,
        /* uint32_t *InData_PubKey,                 */(uint32_t*)client_public_key,
        /* uint32_t *InData_KeyIndex,               */key_index->value,
        /* uint32_t *OutData_SharedSecretKeyIndex   */shared_secret_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            shared_secret_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_SHARED_SECRET_FOR_SERVER;
        }
        else
        {
            shared_secret_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13SVGenerateEcdheSharedSecret
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13SVGenerateHandshakeSecret
*******************************************************************************************************************/ /**
* @details       Generate handshake secret for server.
* @param[in]     shared_secret_key_index ephemeral shared secret key index.
* @param[out]    handshake_secret_key_index ephemeral handshake secret key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Tls13SVGenerateHandshakeSecretSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13SVGenerateHandshakeSecret(
        tsip_tls13_ephemeral_shared_secret_key_index_t *shared_secret_key_index,
        tsip_tls13_ephemeral_handshake_secret_key_index_t *handshake_secret_key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;

    if (TSIP_KEY_INDEX_TYPE_TLS13_SHARED_SECRET_FOR_SERVER == shared_secret_key_index->type)
    {
        error_code = R_TSIP_Tls13SVGenerateHandshakeSecretSub(
        /* uint32_t *InData_SharedSecretKeyIndex,       */shared_secret_key_index->value,
        /* uint32_t *OutData_Handshake_SecretKeyIndex   */handshake_secret_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            handshake_secret_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_HANDSHAKE_SECRET_FOR_SERVER;
        }
        else
        {
            handshake_secret_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13SVGenerateHandshakeSecret
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13SVGenerateServerHandshakeTrafficKey
*******************************************************************************************************************/ /**
* @details       Generate server handshake traffic key index for server.
* @param[out]    handle handler for TLS1.3(work area).
* @param[in]     mode handshake protocol.
* @param[in]     handshake_secret_key_index ephemeral handshake secret key index.
* @param[in]     digest message hash calculated with SHA256
* @param[out]    server_write_key_index ephemeral server write key index.
* @param[out]    server_finished_key_index ephemeral server finished key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Tls13SVGenerateServerHandshakeTrafficKeySub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13SVGenerateServerHandshakeTrafficKey(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_handshake_secret_key_index_t *handshake_secret_key_index, uint8_t *digest,
        tsip_aes_key_index_t *server_write_key_index, tsip_hmac_sha_key_index_t *server_finished_key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t     indata_cmd_sub;

    if (TSIP_KEY_INDEX_TYPE_TLS13_HANDSHAKE_SECRET_FOR_SERVER == handshake_secret_key_index->type)
    {
        /* Casting uint32_t is used to input. */
        indata_cmd_sub = change_endian_long((uint32_t)mode);

        /* Casting uint32_t pointer is used for address. */
        error_code =  R_TSIP_Tls13SVGenerateServerHandshakeTrafficKeySub(
        /* uint32_t *InData_Cmd,                        */&indata_cmd_sub,
        /* uint32_t *InData_Handshake_SecretKeyIndex,   */handshake_secret_key_index->value,
        /* uint32_t *InData_MsgHash,                    */(uint32_t*)digest,
        /* uint32_t *OutData_ServerWriteKeyIndex,       */server_write_key_index->value,
        /* uint32_t *OutData_ServerFinishedKeyIndex     */server_finished_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            server_write_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_SERVER;
            server_finished_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_SERVER_FINISHED_FOR_SERVER;
            /* Casting uint32_t is used to convert. */
            server_finished_key_index->tls13_mode = (uint32_t)mode;

            /* Reset seqnum for the 1st data transmission to client. */
            handle->flag_handshake_tx = R_TSIP_TLS13_RESET_SEQNUM;
        }
        else
        {
            server_write_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
            server_finished_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13SVGenerateServerHandshakeTrafficKey
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13SVClientHandshakeVerification
*******************************************************************************************************************/ /**
* @details       Verify finished data provided from client for server.
* @param[in]     mode handshake protocol.
* @param[in]     client_finished_key_index ephemeral finished write key index.
* @param[in]     digest message hash calculated with SHA256
* @param[in]     client_finished finished data provided from client.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_VERIFICATION_FAIL: Verification NG
* @see           R_TSIP_Tls13SVServerHandshakeVerificationSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13SVClientHandshakeVerification(e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_client_finished_key_index_t *client_finished_key_index, uint8_t *digest,
        uint8_t *client_finished)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t     indata_cmd_sub;

    if (TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_FINISHED_FOR_SERVER == client_finished_key_index->type)
    {
        /* Casting uint32_t is used to input. */
        indata_cmd_sub = change_endian_long((uint32_t)mode);

        /* Casting uint32_t pointer is used for address. */
        error_code = R_TSIP_Tls13SVClientHandshakeVerificationSub(
        /* uint32_t *InData_Cmd,                    */&indata_cmd_sub,
        /* uint32_t *InData_ClientFinishedKeyIndex, */client_finished_key_index->value,
        /* uint32_t *InData_MsgHash,                */(uint32_t *)digest,
        /* uint32_t *InData_ClientFinished,         */(uint32_t *)client_finished
        );
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13SVClientHandshakeVerification
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13SVGenerateClientHandshakeTrafficKey
*******************************************************************************************************************/ /**
* @details       Generate client write key index and client finished key index for server.
* @param[out]    handle handler for TLS1.3(work area).
* @param[in]     mode handshake protocol.
* @param[in]     handshake_secret_key_index ephemeral handshake secret key index.
* @param[in]     digest message hash calculated with SHA256
* @param[out]    client_write_key_index ephemeral client write key index.
* @param[out]    client_finished_key_index ephemeral finished write key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Tls13SVGenerateClientHandshakeTrafficKeySub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13SVGenerateClientHandshakeTrafficKey(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_handshake_secret_key_index_t *handshake_secret_key_index, uint8_t *digest,
        tsip_aes_key_index_t *client_write_key_index,
        tsip_tls13_ephemeral_client_finished_key_index_t *client_finished_key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t     indata_cmd_sub;

    if (TSIP_KEY_INDEX_TYPE_TLS13_HANDSHAKE_SECRET_FOR_SERVER == handshake_secret_key_index->type)
    {
        /* Casting uint32_t is used to input. */
        indata_cmd_sub = change_endian_long((uint32_t)mode);

        /* Casting uint32_t pointer is used for address. */
        error_code = R_TSIP_Tls13SVGenerateClientHandshakeTrafficKeySub(
        /* uint32_t *InData_Cmd,                        */&indata_cmd_sub,
        /* uint32_t *InData_Handshake_SecretKeyIndex,   */handshake_secret_key_index->value,
        /* uint32_t *InData_MsgHash,                    */(uint32_t*)digest,
        /* uint32_t *OutData_ClientWriteKeyIndex,       */client_write_key_index->value,
        /* uint32_t *OutData_ClientFinishedKeyIndex     */client_finished_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            client_write_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_SERVER;
            client_finished_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_FINISHED_FOR_SERVER;

            /* Reset seqnum for the 1st data reception from client. */
            handle->flag_handshake_rx = R_TSIP_TLS13_RESET_SEQNUM;
        }
        else
        {
            client_write_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
            client_finished_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13SVGenerateClientHandshakeTrafficKey
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13SVGenerateMasterSecret
*******************************************************************************************************************/ /**
* @details       Generate master secret for server.
* @param[out]    handle handler for TLS1.3(work area).
* @param[in]     mode handshake protocol.
* @param[in]     handshake_secret_key_index ephemeral handshake secret key index.
* @param[out]    master_secret_key_index ephemeral master secret key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Tls13SVGenerateMasterSecretSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13SVGenerateMasterSecret(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_handshake_secret_key_index_t *handshake_secret_key_index,
        tsip_tls13_ephemeral_master_secret_key_index_t *master_secret_key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t     indata_cmd_sub;

    if (TSIP_KEY_INDEX_TYPE_TLS13_HANDSHAKE_SECRET_FOR_SERVER == handshake_secret_key_index->type)
    {
        /* Casting uint32_t is used to input. */
        indata_cmd_sub = change_endian_long((uint32_t)mode);

        error_code = R_TSIP_Tls13SVGenerateMasterSecretSub(
        /* uint32_t *InData_Cmd,                        */&indata_cmd_sub,
        /* uint32_t *InData_Handshake_SecretKeyIndex,   */handshake_secret_key_index->value,
        /* uint32_t *OutData_Handle,                    */handle->session_handle,
        /* uint32_t *OutData_Master_SecretKeyIndex      */master_secret_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            master_secret_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_MASTER_SECRET_FOR_SERVER;
        }
        else
        {
            master_secret_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13SVGenerateMasterSecret
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13SVGenerateApplicationTrafficKey
*******************************************************************************************************************/ /**
* @details       Generate application traffic key index and application secret for server.
* @param[in,out] handle handler for TLS1.3(work area).
* @param[in]     mode handshake protocol.
* @param[in]     master_secret_key_index ephemeral master secret key index.
* @param[in]     digest message hash calculated with SHA256
* @param[out]    server_app_secret_key_index ephemeral server application secret key index.
* @param[out]    client_app_secret_key_index ephemeral client application secret key index.
* @param[out]    server_write_key_index ephemeral server write key index.
* @param[out]    client_write_key_index ephemeral client write key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Tls13SVGenerateApplicationTrafficKeySub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13SVGenerateApplicationTrafficKey(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_master_secret_key_index_t *master_secret_key_index, uint8_t *digest,
        tsip_tls13_ephemeral_app_secret_key_index_t *server_app_secret_key_index,
        tsip_tls13_ephemeral_app_secret_key_index_t *client_app_secret_key_index,
        tsip_aes_key_index_t *server_write_key_index, tsip_aes_key_index_t *client_write_key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t     indata_cmd_sub;

    if (TSIP_KEY_INDEX_TYPE_TLS13_MASTER_SECRET_FOR_SERVER == master_secret_key_index->type)
    {
        /* Casting uint32_t is used to input. */
        indata_cmd_sub = change_endian_long((uint32_t)mode);

        /* Casting uint32_t pointer is used for address. */
        error_code = R_TSIP_Tls13SVGenerateApplicationTrafficKeySub(
        /* uint32_t *InData_Cmd,                                */&indata_cmd_sub,
        /* uint32_t *InData_Handle,                             */handle->session_handle,
        /* uint32_t *InData_Master_SecretKeyIndex,              */master_secret_key_index->value,
        /* uint32_t *InData_MsgHash,                            */(uint32_t*)digest,
        /* uint32_t *OutData_ServerApplicationSecretKeyIndex,   */server_app_secret_key_index->value,
        /* uint32_t *OutData_ClientApplicationSecretKeyIndex,   */client_app_secret_key_index->value,
        /* uint32_t *OutData_ServerWriteKeyIndex,               */server_write_key_index->value,
        /* uint32_t *OutData_ClientWriteKeyIndex                */client_write_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            server_app_secret_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_APP_TRAFFIC_SECRET_FOR_SERVER;
            client_app_secret_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_APP_TRAFFIC_SECRET_FOR_SERVER;
            server_write_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_SERVER;
            client_write_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_SERVER;

            /* Reset seqnum for the 1st data transmission to client. */
            handle->flag_application_tx = R_TSIP_TLS13_RESET_SEQNUM;
            /* Reset seqnum for the 1st data reception from client. */
            handle->flag_application_rx = R_TSIP_TLS13_RESET_SEQNUM;
        }
        else
        {
            server_app_secret_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
            client_app_secret_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
            server_write_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
            client_write_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13SVGenerateApplicationTrafficKey
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13SVUpdateApplicationTrafficKey
*******************************************************************************************************************/ /**
* @details       Update application traffic key index and application secret for server.
* @param[in,out] handle handler for TLS1.3(work area).
* @param[in]     mode handshake protocol.
* @param[in]     key_type key type to update application traffic key.
* @param[in]     input_app_secret_key_index updating ephemeral server/client application secret key index.
* @param[out]    output_app_secret_key_index updated ephemeral server/client application secret key index.
* @param[out]    app_write_key_index ephemeral server/client write key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           R_TSIP_Tls13SVUpdateApplicationTrafficKeySub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13SVUpdateApplicationTrafficKey(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        e_tsip_tls13_update_key_type_t key_type,
        tsip_tls13_ephemeral_app_secret_key_index_t *input_app_secret_key_index,
        tsip_tls13_ephemeral_app_secret_key_index_t *output_app_secret_key_index,
        tsip_aes_key_index_t *app_write_key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t     indata_cmd_sub;
    uint32_t     indata_key_type_sub;

    if (TSIP_KEY_INDEX_TYPE_TLS13_APP_TRAFFIC_SECRET_FOR_SERVER == input_app_secret_key_index->type)
    {
        /* Casting uint32_t is used to input. */
        indata_cmd_sub = change_endian_long((uint32_t)mode);
        /* Casting uint32_t is used to input. */
        indata_key_type_sub = change_endian_long((uint32_t)key_type);

        error_code = R_TSIP_Tls13SVUpdateApplicationTrafficKeySub(
        /* uint32_t *InData_Cmd,                        */&indata_cmd_sub,
        /* uint32_t *InData_UpdateKeyType,              */&indata_key_type_sub,
        /* uint32_t *InData_Handle,                     */handle->session_handle,
        /* uint32_t *InData_ApplicationSecretKeyIndex,  */input_app_secret_key_index->value,
        /* uint32_t *OutData_ApplicationSecretKeyIndex, */output_app_secret_key_index->value,
        /* uint32_t *OutData_ApplicationWriteKeyIndex   */app_write_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            output_app_secret_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_APP_TRAFFIC_SECRET_FOR_SERVER;
            if (TSIP_TLS13_UPDATE_SERVER_KEY == key_type)
            {
                app_write_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_SERVER;

                /* Wait for 1st data transmission to client. */
                handle->flag_application_tx = R_TSIP_TLS13_RESET_SEQNUM;
            }
            else
            {
                app_write_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_SERVER;

                /* Reset seqnum for the 1st data reception from client. */
                handle->flag_application_rx = R_TSIP_TLS13_RESET_SEQNUM;
            }
        }
        else
        {
            output_app_secret_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
            app_write_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }

    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13SVUpdateApplicationTrafficKey
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13SVGenerateResumptionMasterSecret
*******************************************************************************************************************/ /**
* @details       Generate resumption master secret for server.
* @param[in]     handle handler for TLS1.3(work area).
* @param[in]     mode handshake protocol.
* @param[in]     master_secret_key_index ephemeral master secret key index.
* @param[in]     digest message hash calculated with SHA256
* @param[out]    res_master_secret_key_index ephemeral resumption master secret key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Tls13SVGenerateResumptionMasterSecretSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13SVGenerateResumptionMasterSecret(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_master_secret_key_index_t *master_secret_key_index, uint8_t *digest,
        tsip_tls13_ephemeral_res_master_secret_key_index_t *res_master_secret_key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t     indata_cmd_sub;

    if (TSIP_KEY_INDEX_TYPE_TLS13_MASTER_SECRET_FOR_SERVER == master_secret_key_index->type)
    {
        /* Casting uint32_t is used to input. */
        indata_cmd_sub          = change_endian_long((uint32_t)mode);

        /* Casting uint32_t pointer is used for address. */
        error_code = R_TSIP_Tls13SVGenerateResumptionMasterSecretSub(
        /* uint32_t *InData_Cmd,                    */&indata_cmd_sub,
        /* uint32_t *InData_Handle,                 */handle->session_handle,
        /* uint32_t *InData_Master_SecretKeyIndex,  */master_secret_key_index->value,
        /* uint32_t *InData_MsgHash,                */(uint32_t*)digest,
        /* uint32_t *OutData_RMaster_SecretKeyIndex */res_master_secret_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            res_master_secret_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_RESUMPTION_MASTER_SECRET_FOR_SERVER;
        }
        else
        {
            res_master_secret_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13SVGenerateResumptionMasterSecret
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13SVGeneratePreSharedKey
*******************************************************************************************************************/ /**
* @details       Generate pre shared key index for server.
* @param[in]     handle handler for TLS1.3(work area).
* @param[in]     mode handshake protocol which the master secret generated.
* @param[in]     res_master_secret_key_index ephemeral resumption master secret key index.
* @param[in]     ticket_nonce ticket nonce.
* @param[in]     ticket_nonce_len lencth of ticket nonce.
* @param[out]    pre_shared_key_index ephemeral pre shared key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Tls13SVGeneratePreSharedKeySub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13SVGeneratePreSharedKey(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_res_master_secret_key_index_t *res_master_secret_key_index, uint8_t *ticket_nonce,
        uint32_t ticket_nonce_len, tsip_tls13_ephemeral_pre_shared_key_index_t * pre_shared_key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t     indata_cmd_sub;
    uint32_t     indata_ticket_nonce_len;

    if (TSIP_KEY_INDEX_TYPE_TLS13_RESUMPTION_MASTER_SECRET_FOR_SERVER == res_master_secret_key_index->type)
    {
        /* Casting uint32_t is used to input. */
        indata_cmd_sub          = change_endian_long((uint32_t)mode);
        indata_ticket_nonce_len = change_endian_long(ticket_nonce_len);

        /* Casting uint32_t pointer is used for address. */
        error_code = R_TSIP_Tls13SVGeneratePreSharedKeySub(
        /* uint32_t *InData_Cmd,                    */&indata_cmd_sub,
        /* uint32_t *InData_Handle,                 */handle->session_handle,
        /* uint32_t *InData_RMaster_SecretKeyIndex, */res_master_secret_key_index->value,
        /* uint32_t *InData_TicketNonceLen,         */&indata_ticket_nonce_len,
        /* uint32_t *InData_TicketNonce,            */(uint32_t*)ticket_nonce,
        /* uint32_t *OutData_PreSharedKeyIndex      */pre_shared_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            pre_shared_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_PRE_SHARED_KEY_FOR_SERVER;
        }
        else
        {
            pre_shared_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13SVGeneratePreSharedKey
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13SVGeneratePskBinderKey
*******************************************************************************************************************/ /**
* @details       Generate psk binder key index for server.
* @param[in]     handle handler for TLS1.3(work area).
* @param[in]     pre_shared_key_index ephemeral pre shared key index.
* @param[out]    psk_binder_key_index ephemeral psk binder key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Tls13SVGeneratePskBinderKeySub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13SVGeneratePskBinderKey(tsip_tls13_handle_t *handle,
        tsip_tls13_ephemeral_pre_shared_key_index_t * pre_shared_key_index,
        tsip_hmac_sha_key_index_t * psk_binder_key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;

    if (TSIP_KEY_INDEX_TYPE_TLS13_PRE_SHARED_KEY_FOR_SERVER == pre_shared_key_index->type)
    {
        error_code = R_TSIP_Tls13SVGeneratePskBinderKeySub(
        /* uint32_t *InData_Handle,             */handle->session_handle,
        /* uint32_t *InData_PreSharedKeyIndex,  */pre_shared_key_index->value,
        /* uint32_t *OutData_BinderKeyIndex     */psk_binder_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            psk_binder_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_PSK_BINDER_KEY_FOR_SERVER;
        }
        else
        {
            psk_binder_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13SVGeneratePskBinderKey
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13SVGenerate0RttApplicationWriteKey
*******************************************************************************************************************/ /**
* @details       Generate client application traffic key index to use in 0-RTT for server.
* @param[in,out] handle handler for TLS1.3(work area).
* @param[in]     pre_shared_key_index ephemeral pre shared key index.
* @param[in]     digest message hash calculated with SHA256
* @param[out]    client_write_key_index ephemeral client write key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Tls13SVGenerate0RttApplicationWriteKeySub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13SVGenerate0RttApplicationWriteKey(tsip_tls13_handle_t *handle,
        tsip_tls13_ephemeral_pre_shared_key_index_t * pre_shared_key_index,
        uint8_t *digest, tsip_aes_key_index_t *client_write_key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;

    if (TSIP_KEY_INDEX_TYPE_TLS13_PRE_SHARED_KEY_FOR_SERVER == pre_shared_key_index->type)
    {
        error_code = R_TSIP_Tls13SVGenerate0RttApplicationWriteKeySub(
        /* uint32_t *InData_Handle,                 */handle->session_handle,
        /* uint32_t *InData_PreSharedKeyIndex,      */pre_shared_key_index->value,
        /* uint32_t *InData_MsgHash,                */(uint32_t*)digest,
        /* uint32_t *OutData_ClientWriteKeyIndex    */client_write_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            client_write_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_SERVER;

            /* Wait for 1st data reception from client. */
            handle->flag_application_rx = R_TSIP_TLS13_RESET_SEQNUM;
        }
        else
        {
            client_write_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13SVGenerate0RttApplicationWriteKey
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13SVGenerateResumptionHandshakeSecret
*******************************************************************************************************************/ /**
* @details       Generate resumption handshake secret for server.
* @param[in]     handle handler for TLS1.3(work area).
* @param[in]     mode handshake protocol which the shared secret generated.
* @param[in]     pre_shared_key_index ephemeral pre shared key index.
* @param[in]     shared_secret_key_index ephemeral shared secret key index.
* @param[out]    handshake_secret_key_index ephemeral handshake secret key index.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Tls13SVGenerateResumptionHandshakeSecretSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13SVGenerateResumptionHandshakeSecret(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_pre_shared_key_index_t * pre_shared_key_index,
        tsip_tls13_ephemeral_shared_secret_key_index_t *shared_secret_key_index,
        tsip_tls13_ephemeral_handshake_secret_key_index_t *handshake_secret_key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t     indata_cmd_sub;

    if ((TSIP_KEY_INDEX_TYPE_TLS13_PRE_SHARED_KEY_FOR_SERVER == pre_shared_key_index->type)
        && (TSIP_KEY_INDEX_TYPE_TLS13_SHARED_SECRET_FOR_SERVER == shared_secret_key_index->type))
    {
        /* Casting uint32_t is used to input. */
        indata_cmd_sub = change_endian_long((uint32_t)mode);

        error_code = R_TSIP_Tls13SVGenerateResumptionHandshakeSecretSub(
        /* uint32_t *InData_Handle,                     */handle->session_handle,
        /* uint32_t *InData_SharedSecretKeyIndexType,   */&indata_cmd_sub,
        /* uint32_t *InData_PreSharedKeyIndex,          */pre_shared_key_index->value,
        /* uint32_t *InData_SharedSecretKeyIndex,       */shared_secret_key_index->value,
        /* uint32_t *OutData_Handshake_SecretKeyIndex   */handshake_secret_key_index->value
        );

        if (TSIP_SUCCESS == error_code)
        {
            handshake_secret_key_index->type = TSIP_KEY_INDEX_TYPE_TLS13_HANDSHAKE_SECRET_FOR_SERVER;
        }
        else
        {
            handshake_secret_key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    else
    {
        error_code = TSIP_ERR_KEY_SET;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13SVGenerateResumptionHandshakeSecret
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13SVCertificateVerifyGenerate
*******************************************************************************************************************/ /**
* @details       Generate Certificate Verify data to help TLS1.3 application for server.
* @param[in]     key_index user secret key index.
* @param[in]     signature_scheme signature algorithm.
* @param[in]     digest message hash calculated with SHA256
* @param[out]    certificate_verify Certificate Verify data.
* @param[out]    certificate_verify_len length of Certificate Verify data.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           gen_sign_ecdsa()
* @see           gen_sign_rsa_pss()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13SVCertificateVerifyGenerate(uint32_t *key_index,
        e_tsip_tls13_signature_scheme_type_t signature_scheme, uint8_t *digest, uint8_t *certificate_verify,
        uint32_t *certificate_verify_len)
{
    e_tsip_err_t            error_code                                                 = TSIP_SUCCESS;
    uint8_t                 indata_msg_dgst[TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE];
    uint8_t                 signature_data[TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE * 2] =
    {
        0
    };
    uint32_t                signature_len;
    uint8_t                 out_signature_index = 0;
    uint8_t                 output_index        = 0;
    uint8_t                 r_field_length      = 0;
    uint8_t                 s_field_length      = 0;

    if (TSIP_TLS13_SIGNATURE_SCHEME_ECDSA_SECP256R1_SHA256 == signature_scheme)
    {
        memcpy(indata_msg_dgst, s_certificate_verify_content, TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE - 32);
        memcpy(&indata_msg_dgst[73], s_certificate_verify_content_server, 6);
        memcpy(&indata_msg_dgst[TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE-R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE], digest,
            R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE);

        /* Casting tsip_ecc_public_key_index_t pointer is used to adjust key type. */
        error_code = gen_sign_ecdsa((tsip_ecc_private_key_index_t *)key_index, indata_msg_dgst,
            TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE, signature_data, &signature_len);
        if (TSIP_SUCCESS == error_code)
        {
            /* Signature Algorithm field */
            certificate_verify[0] = TSIP_TLS13_SIG_ALG_HASH_SHA256;
            certificate_verify[1] = TSIP_TLS13_SIG_ALG_SIG_ECDSA;

            /* Integer r field */
            output_index   = 8; /* position of r data */
            r_field_length = TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE;
            for (out_signature_index = 0; out_signature_index < TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE;
                    out_signature_index++)
            {
                if (0x00 == signature_data[out_signature_index])
                {
                    r_field_length--;
                }
                else
                {
                    break;
                }
            }
            if (0x80 == (signature_data[out_signature_index] & 0x80))
            {
                certificate_verify[output_index] = 0x00;
                output_index++;
                r_field_length += 1;
            }
            memcpy(&certificate_verify[output_index], &signature_data[out_signature_index],
                (TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE - out_signature_index));
            certificate_verify[6] = TSIP_TLS13_ASN1_TAG_INTEGER;
            certificate_verify[7] = r_field_length;

            /* Integer s field */
            output_index   = 10 + r_field_length; /* position of s data */
            s_field_length = TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE;
            for (out_signature_index = TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE;
                    out_signature_index < (TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE *2); out_signature_index++)
            {
                if (0x00 == signature_data[out_signature_index])
                {
                    s_field_length--;
                }
                else
                {
                    break;
                }
            }
            if (0x80 == (signature_data[out_signature_index] & 0x80))
            {
                certificate_verify[output_index] = 0x00;
                output_index++;
                s_field_length += 1;
            }
            memcpy(&certificate_verify[output_index], &signature_data[out_signature_index],
                (TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE * 2 - out_signature_index));
            certificate_verify[8 + r_field_length] = TSIP_TLS13_ASN1_TAG_INTEGER;
            certificate_verify[9 + r_field_length] = s_field_length;

            /* Header field */
            certificate_verify[2] = 0x00;
            certificate_verify[3] = 6 + r_field_length + s_field_length;
            certificate_verify[4] = TSIP_TLS13_ASN1_TAG_SEQUENCE;
            certificate_verify[5] = 4 + r_field_length + s_field_length;

            *certificate_verify_len = 10 + r_field_length + s_field_length;
        }
    }
    else if (TSIP_TLS13_SIGNATURE_SCHEME_RSA_PSS_RSAE_SHA256 == signature_scheme)
    {
        /* Signature Algorithm field */
        certificate_verify[0] = TSIP_TLS13_SIG_ALG_SIG_RSA_PSS;
        certificate_verify[1] = TSIP_TLS13_SIG_ALG_HASH_SHA256;

        /* length field */
        certificate_verify[2] = 0x01;
        certificate_verify[3] = 0x00;

        memcpy(indata_msg_dgst, s_certificate_verify_content, TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE - 32);
        memcpy(&indata_msg_dgst[73], s_certificate_verify_content_server, 6);
        memcpy(&indata_msg_dgst[TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE-R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE], digest,
            R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE);

        /* Casting tsip_rsa2048_private_key_index_t pointer is used to input. */
        error_code = gen_sign_rsa_pss((tsip_rsa2048_private_key_index_t *)key_index, indata_msg_dgst,
            TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE, &certificate_verify[4], certificate_verify_len);

        if (TSIP_SUCCESS == error_code)
        {
            (*certificate_verify_len) += 4;
        }
    }
    else
    {
        error_code = TSIP_ERR_PARAMETER;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13SVCertificateVerifyGenerate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Tls13SVCertificateVerifyVerification
*******************************************************************************************************************/ /**
* @details       Verify Certificate Verify data to help TLS1.3 application for server.
* @param[in]     key_index encrypted public key.
* @param[in]     signature_scheme signature algorithm.
* @param[in]     digest message hash calculated with SHA256
* @param[in]     certificate_verify Certificate Verify data.
* @param[in]     certificate_verify_len length of Certificate Verify data.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           R_TSIP_TlsCertificateVerificationSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13SVCertificateVerifyVerification(uint32_t *key_index,
        e_tsip_tls13_signature_scheme_type_t signature_scheme, uint8_t *digest, uint8_t *certificate_verify,
        uint32_t certificate_verify_len)
{
    e_tsip_err_t            error_code                                                 = TSIP_SUCCESS;
    uint8_t                 indata_msg_dgst[TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE];
    uint8_t                 signature_data[TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE * 2] =
    {
        0
    };
    uint8_t                 input_index                = 0;
    uint8_t                 output_index               = 0;
    uint8_t                 length_index               = 0;
    uint8_t                 copy_length                = 0;
    uint32_t                certificate_length_sub     = 0;
    uint32_t                tmp_public_key_type        = 0;
    uint32_t                tmp_public_key_output_type = 0;
    uint32_t              * p_tmp_signature_data;

    if (TSIP_TLS13_SIGNATURE_SCHEME_ECDSA_SECP256R1_SHA256 == signature_scheme)
    {
        if ((TSIP_TLS13_SIG_ALG_HASH_SHA256 != certificate_verify[0])
            || (TSIP_TLS13_SIG_ALG_SIG_ECDSA != certificate_verify[1])) /* Signature Algorithm field */
        {
            error_code = TSIP_ERR_PARAMETER;
        }
        else if ((((certificate_verify[3] + 4) != certificate_verify_len))
                || ((certificate_verify[5] + 6) != certificate_verify_len)) /* Header field */
        {
            error_code = TSIP_ERR_PARAMETER;
        }
        else
        {
            error_code = TSIP_SUCCESS;
        }

        /* Integer r field */
        if (TSIP_SUCCESS == error_code)
        {
            length_index = 7;

            if ((TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE + 1) == certificate_verify[length_index])
            {
                input_index  = length_index + 2;
                output_index = 0;
                copy_length  = TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE;
            }
            else if (TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE >= certificate_verify[length_index])
            {
                input_index  = length_index + 1;
                output_index = TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE - certificate_verify[length_index];
                copy_length  = certificate_verify[length_index];
            }
            else
            {
                error_code = TSIP_ERR_PARAMETER;
            }

            if (TSIP_SUCCESS == error_code)
            {
                memcpy(&signature_data[output_index], &certificate_verify[input_index], copy_length);
            }
        }

        /* Integer s field */
        if (TSIP_SUCCESS == error_code)
        {
            length_index = input_index + copy_length + 1;

            if ((TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE + 1) == certificate_verify[length_index])
            {
                input_index  = length_index + 2;
                output_index = TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE;
                copy_length  = TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE;
            }
            else if (TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE >= certificate_verify[length_index])
            {
                input_index  = length_index + 1;
                output_index = (TSIP_TLS13_CERTIFICATEVERIFY_SIG_DATA_SIZE *2) - certificate_verify[length_index];
                copy_length  = certificate_verify[length_index];
            }
            else
            {
                error_code = TSIP_ERR_PARAMETER;
            }

            if (TSIP_SUCCESS == error_code)
            {
                memcpy(&signature_data[output_index], &certificate_verify[input_index], copy_length);
            }
        }

        tmp_public_key_type        = change_endian_long(0x00000002);
        tmp_public_key_output_type = change_endian_long(0x00000005);

        /* Casting uint32_t pointer is used to input. */
        p_tmp_signature_data = (uint32_t *)signature_data;
    }
    else if (TSIP_TLS13_SIGNATURE_SCHEME_RSA_PSS_RSAE_SHA256 == signature_scheme)
    {
        if ((TSIP_TLS13_SIG_ALG_SIG_RSA_PSS != certificate_verify[0])
            || (TSIP_TLS13_SIG_ALG_HASH_SHA256 != certificate_verify[1])) /* Signature Algorithm field */
        {
            error_code = TSIP_ERR_PARAMETER;
        }
        else if ((((certificate_verify[2] << 8) + certificate_verify[3] + 4) != certificate_verify_len)
            || (0x0100 != ((certificate_verify[2] << 8) + certificate_verify[3]))) /* Header field */
        {
            error_code = TSIP_ERR_PARAMETER;
        }
        else
        {
            error_code = TSIP_SUCCESS;
        }

        tmp_public_key_type        = change_endian_long(0x00000003);
        tmp_public_key_output_type = change_endian_long(0x00000005);

        /* Casting uint32_t pointer is used to input. */
        p_tmp_signature_data = (uint32_t *) &certificate_verify[4];
    }
    else
    {
        error_code = TSIP_ERR_PARAMETER;
    }

    if (TSIP_SUCCESS == error_code)
    {
        memcpy(indata_msg_dgst, s_certificate_verify_content, TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE - 32);
        memcpy(&indata_msg_dgst[73], s_certificate_verify_content_client, 6);
        memcpy(&indata_msg_dgst[TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE-R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE], digest,
            R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE);
        certificate_length_sub = change_endian_long(TSIP_TLS13_CERTIFICATEVERIFY_CONTEXT_SIZE);

        /* Casting uint32_t pointer is used for address. */
        error_code = R_TSIP_TlsSVCertificateVerificationSub(
        /* uint32_t *InData_Sel_InDataPubKeyType, */&tmp_public_key_type,
        /* uint32_t *InData_Sel_OutDataPubKeyType,*/&tmp_public_key_output_type,
        /* uint32_t *InData_PubKey,               */key_index,
        /* uint32_t *InData_TBSCertificate,       */(uint32_t *)indata_msg_dgst,
        /* uint32_t *InData_TBSCertificateLength, */&certificate_length_sub,
        /* uint32_t *InData_Signature,            */p_tmp_signature_data,
        /* uint32_t *InData_TBSCertificatesInfo,  */NULL,
        /* uint32_t *OutData_PubKey               */NULL);
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_Tls13SVCertificateVerifyVerification
 *******************************/
#endif /* TSIP_TLS == 1 */

