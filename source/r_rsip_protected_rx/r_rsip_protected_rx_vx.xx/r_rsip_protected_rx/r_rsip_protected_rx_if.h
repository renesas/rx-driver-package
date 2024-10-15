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
* File Name    : r_rsip_protected_rx_if.h
* Description  : Interface definition for the r_rsip_protected_rx module.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 15.10.2024 1.00     First Release.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup RENESAS_SECURITY_INTERFACES
 * @defgroup RSIP_PROTECTED_API_Summary RSIP Interface
 * @brief Interface for Renesas Secure IP (RSIP) functions.
 *
 * @section RSIP_API_Summary Summary
 * The RSIP interface provides RSIP functionality.
 *
 * @{
 **********************************************************************************************************************/

#ifndef R_RSIP_RX_HEADER_FILE
#define R_RSIP_RX_HEADER_FILE

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"
#include "r_rsip_protected_rx_config.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#define RSIP_VERSION_MAJOR    (1u)
#define RSIP_VERSION_MINOR    (00u)

/* Wrapped Key size */
#define RSIP_CFG_BYTE_SIZE_ENCRYPTED_KEY_VALUE_AES_128          (32)
#define RSIP_CFG_BYTE_SIZE_ENCRYPTED_KEY_VALUE_AES_256          (48)

/* Calculate key type value and size */
#define RSIP_PRV_KEY_TYPE(alg, subtype)                         (((alg) << 8) + (subtype))
/* Calculate key pair type value */
#define RSIP_PRV_KEY_PAIR_TYPE(pub_alg, priv_alg, subtype)      (((pub_alg) << 16) + ((priv_alg) << 8) + (subtype))
/* Calculate key size */
#define RSIP_PRV_KEY_SIZE(key_value_size)                       (4U + (key_value_size))

/* S_RAM, S_INST2, S_HEAP size */
#define RSIP_PRV_WORD_SIZE_S_RAM                                (32U)
#define RSIP_PRV_WORD_SIZE_S_INST2                              (16U)
#define RSIP_PRV_WORD_SIZE_S_HEAP                               (940U)

/* AES */
#define RSIP_PRV_BYTE_SIZE_AES_BLOCK                            (16U)
#define RSIP_PRV_BYTE_SIZE_CCM_NONCE_MAX                        (13U)
#define RSIP_PRV_BYTE_SIZE_CCM_AAD_MAX                          (110U)

/* SHA */
#define RSIP_PRV_SHA224256_HASH_LENGTH_BYTE_SIZE                (32U)

/* FirmwareUpdate */
#define RSIP_PRV_BYTE_SIZE_FWUP_BLOCK                           (16U)

/* Secure boot section. */
#if defined RSIP_SECURE_BOOT_SECTION_ENBALE
/* Required for each variable definition with no initial value to be placed in the SECURE_BOOT section. */
#define RSIP_SEC_B_SECURE_BOOT          R_BSP_ATTRIB_SECTION_CHANGE(B, SECURE_BOOT, 4)
/* Required for each constant definition to be placed in the SECURE_BOOT section. */
#define RSIP_SEC_C_SECURE_BOOT          R_BSP_ATTRIB_SECTION_CHANGE(C, SECURE_BOOT, 4)
/* Required for each variable definition with initial value to be placed in the SECURE_BOOT section. */
#define RSIP_SEC_D_SECURE_BOOT          R_BSP_ATTRIB_SECTION_CHANGE(D, SECURE_BOOT, 4)
/* Required for each function definition to be placed in the SECURE_BOOT section. */
#define RSIP_SEC_P_SECURE_BOOT          R_BSP_ATTRIB_SECTION_CHANGE(P, SECURE_BOOT)
/* Required for each function definition to be placed in the SECURE_BOOT_ERASE section. */
#define RSIP_SEC_P_SECURE_BOOT_ERASE    R_BSP_ATTRIB_SECTION_CHANGE(P, SECURE_BOOT_ERASE)
/* Revert to default section. */
#define RSIP_SEC_DEFAULT                R_BSP_ATTRIB_SECTION_CHANGE_END
#else
/* Required for each variable definition with no initial value to be placed in the SECURE_BOOT section.(dummy) */
#define RSIP_SEC_B_SECURE_BOOT
/* Required for each constant definition to be placed in the SECURE_BOOT section.(dummy) */
#define RSIP_SEC_C_SECURE_BOOT
/* Required for each variable definition with initial value to be placed in the SECURE_BOOT section.(dummy) */
#define RSIP_SEC_D_SECURE_BOOT
/* Required for each function definition to be placed in the SECURE_BOOT section.(dummy) */
#define RSIP_SEC_P_SECURE_BOOT
/* Required for each function definition to be placed in the SECURE_BOOT_ERASE section.(dummy) */
#define RSIP_SEC_P_SECURE_BOOT_ERASE
/* Revert to default section.(dummy) */
#define RSIP_SEC_DEFAULT
#endif  /* defined RSIP_SECURE_BOOT */

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
typedef void rsip_ctrl_t;

/** Return type */
typedef enum e_rsip_err_t
{
    RSIP_SUCCESS = 0u,
    RSIP_ERR_FAIL,
    RSIP_ERR_RESOURCE_CONFLICT,
    RSIP_ERR_RETRY,
    RSIP_ERR_KEY_SET,
    RSIP_ERR_PARAMETER,
    RSIP_ERR_PROHIBIT_FUNCTION,
    RSIP_ERR_AUTHENTICATION,
} rsip_err_t;

/** Key sub type */
typedef enum e_rsip_key_subtype
{
    RSIP_KEY_INVALID = 0x00,             // Invalid key

    RSIP_KEY_AES_128 = 0x00,             // AES-128
    RSIP_KEY_AES_256 = 0x01,             // AES-256
    RSIP_KEY_AES_NUM,                    // Number of subtypes

    RSIP_KEY_ECC_SECP256R1       = 0x00, // secp256r1
    RSIP_KEY_ECC_SECP256K1       = 0x01, // secp256k1
    RSIP_KEY_ECC_BRAINPOOLP256R1 = 0x02, // brainpoolP256r1
    RSIP_KEY_ECC_NUM,                    // Number of subtypes

    RSIP_KEY_HMAC_SHA224 = 0x00,         // HMAC-SHA224
    RSIP_KEY_HMAC_SHA256 = 0x01,         // HMAC-SHA256
    RSIP_KEY_HMAC_NUM,                   // Number of subtypes
} rsip_key_subtype_t;

/** Key algorithms */
typedef enum e_rsip_alg
{
    RSIP_ALG_INVALID     = 0x00,       // Invalid
    RSIP_ALG_AES         = 0x10,       // AES
    RSIP_ALG_ECC_PUBLIC  = 0x20,       // ECC public key
    RSIP_ALG_ECC_PRIVATE = 0x21,       // ECC private key
    RSIP_ALG_HMAC        = 0x30,       // HMAC
} rsip_alg_t;

/** Key types */
typedef enum e_rsip_key_type
{
    RSIP_KEY_TYPE_INVALID                     = RSIP_PRV_KEY_TYPE(RSIP_ALG_INVALID, RSIP_KEY_INVALID),                 // Invalid key
    RSIP_KEY_TYPE_AES_128                     = RSIP_PRV_KEY_TYPE(RSIP_ALG_AES, RSIP_KEY_AES_128),                     // AES-128
    RSIP_KEY_TYPE_AES_256                     = RSIP_PRV_KEY_TYPE(RSIP_ALG_AES, RSIP_KEY_AES_256),                     // AES-256
    RSIP_KEY_TYPE_ECC_SECP256R1_PUBLIC        = RSIP_PRV_KEY_TYPE(RSIP_ALG_ECC_PUBLIC, RSIP_KEY_ECC_SECP256R1),        // secp256r1 public key (also known as NIST P-256, prime256v1)
    RSIP_KEY_TYPE_ECC_SECP256K1_PUBLIC        = RSIP_PRV_KEY_TYPE(RSIP_ALG_ECC_PUBLIC, RSIP_KEY_ECC_SECP256K1),        // secp256k1 public key
    RSIP_KEY_TYPE_ECC_BRAINPOOLP256R1_PUBLIC  = RSIP_PRV_KEY_TYPE(RSIP_ALG_ECC_PUBLIC, RSIP_KEY_ECC_BRAINPOOLP256R1),  // brainpoolP256r1 public key
    RSIP_KEY_TYPE_ECC_SECP256R1_PRIVATE       = RSIP_PRV_KEY_TYPE(RSIP_ALG_ECC_PRIVATE, RSIP_KEY_ECC_SECP256R1),       // secp256r1 private key (also known as NIST P-256, prime256v1))
    RSIP_KEY_TYPE_ECC_SECP256K1_PRIVATE       = RSIP_PRV_KEY_TYPE(RSIP_ALG_ECC_PRIVATE, RSIP_KEY_ECC_SECP256K1),       // secp256k1 private key
    RSIP_KEY_TYPE_ECC_BRAINPOOLP256R1_PRIVATE = RSIP_PRV_KEY_TYPE(RSIP_ALG_ECC_PRIVATE, RSIP_KEY_ECC_BRAINPOOLP256R1), // brainpoolP256r1 private key
    RSIP_KEY_TYPE_HMAC_SHA224                 = RSIP_PRV_KEY_TYPE(RSIP_ALG_HMAC, RSIP_KEY_HMAC_SHA224),                // HMAC-SHA224
    RSIP_KEY_TYPE_HMAC_SHA256                 = RSIP_PRV_KEY_TYPE(RSIP_ALG_HMAC, RSIP_KEY_HMAC_SHA256),                // HMAC-SHA256
} rsip_key_type_t;

/** Key pair types */
typedef enum e_rsip_key_pair_type
{
    RSIP_KEY_PAIR_TYPE_INVALID,                                                                          // Invalid key pair type
    RSIP_KEY_PAIR_TYPE_ECC_SECP256R1 =
        RSIP_PRV_KEY_PAIR_TYPE(RSIP_ALG_ECC_PUBLIC, RSIP_ALG_ECC_PRIVATE, RSIP_KEY_ECC_SECP256R1),       // secp256r1 key pair (also known as NIST P-256, prime256v1)
    RSIP_KEY_PAIR_TYPE_ECC_SECP256K1 =
        RSIP_PRV_KEY_PAIR_TYPE(RSIP_ALG_ECC_PUBLIC, RSIP_ALG_ECC_PRIVATE, RSIP_KEY_ECC_SECP256K1),       // secp256k1 key pair
    RSIP_KEY_PAIR_TYPE_ECC_BRAINPOOLP256R1 =
        RSIP_PRV_KEY_PAIR_TYPE(RSIP_ALG_ECC_PUBLIC, RSIP_ALG_ECC_PRIVATE, RSIP_KEY_ECC_BRAINPOOLP256R1), // brainpoolP256r1 key pair
} rsip_key_pair_type_t;

/** Byte size of wrapped key */
typedef enum e_rsip_byte_size_wrapped_key
{
    RSIP_BYTE_SIZE_WRAPPED_KEY_AES_128 =
        RSIP_PRV_KEY_SIZE(RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_AES_128),                     // AES-128
    RSIP_BYTE_SIZE_WRAPPED_KEY_AES_256 =
        RSIP_PRV_KEY_SIZE(RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_AES_256),                     // AES-256
    RSIP_BYTE_SIZE_WRAPPED_KEY_ECC_SECP256R1_PUBLIC =
        RSIP_PRV_KEY_SIZE(RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_SECP256R1_PUBLIC),        // secp256r1 public key
    RSIP_BYTE_SIZE_WRAPPED_KEY_ECC_SECP256K1_PUBLIC =
        RSIP_PRV_KEY_SIZE(RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_SECP256K1_PUBLIC),        // secp256k1 public key
    RSIP_BYTE_SIZE_WRAPPED_KEY_ECC_BRAINPOOLP256R1_PUBLIC =
        RSIP_PRV_KEY_SIZE(RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_BRAINPOOLP256R1_PUBLIC),  // brainpoolP256r1 public key
    RSIP_BYTE_SIZE_WRAPPED_KEY_ECC_SECP256R1_PRIVATE =
        RSIP_PRV_KEY_SIZE(RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_SECP256R1_PRIVATE),       // secp256r1 private key
    RSIP_BYTE_SIZE_WRAPPED_KEY_ECC_SECP256K1_PRIVATE =
        RSIP_PRV_KEY_SIZE(RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_SECP256K1_PRIVATE),       // secp256k1 private key
    RSIP_BYTE_SIZE_WRAPPED_KEY_ECC_BRAINPOOLP256R1_PRIVATE =
        RSIP_PRV_KEY_SIZE(RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_BRAINPOOLP256R1_PRIVATE), // brainpoolP256r1 private key
    RSIP_BYTE_SIZE_WRAPPED_KEY_HMAC_SHA224 =
        RSIP_PRV_KEY_SIZE(RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_HMAC_SHA224),                 // HMAC-SHA224 private key
    RSIP_BYTE_SIZE_WRAPPED_KEY_HMAC_SHA256 =
        RSIP_PRV_KEY_SIZE(RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_HMAC_SHA256),                 // HMAC-SHA256 private key
} rsip_byte_size_wrapped_key_t;

/* State that specifies functions that can be called next */
typedef enum e_rsip_state
{
    RSIP_STATE_MAIN,                    // Main
    RSIP_STATE_AES_CIPHER,              // AES-ECB, AES-CBC, AES-CTR
    RSIP_STATE_AES_GCM_ENC_UPDATE_AAD,  // AES-GCM encryption, AAD input
    RSIP_STATE_AES_GCM_ENC_UPDATE_TEXT, // AES-GCM encryption, plaintext/ciphertext input
    RSIP_STATE_AES_GCM_DEC_UPDATE_AAD,  // AES-GCM decryption, AAD input
    RSIP_STATE_AES_GCM_DEC_UPDATE_TEXT, // AES-GCM decryption, plaintext/ciphertext input
    RSIP_STATE_AES_CCM_ENC_SET_LENGTH,  // AES-CCM encryption, length setting
    RSIP_STATE_AES_CCM_ENC_UPDATE_AAD,  // AES-CCM encryption, AAD input
    RSIP_STATE_AES_CCM_ENC_UPDATE_TEXT, // AES-CCM encryption, plaintext/ciphertext input
    RSIP_STATE_AES_CCM_ENC_FINISH,      // AES-CCM encryption, finish
    RSIP_STATE_AES_CCM_DEC_SET_LENGTH,  // AES-CCM decryption, length setting
    RSIP_STATE_AES_CCM_DEC_UPDATE_AAD,  // AES-CCM decryption, AAD input
    RSIP_STATE_AES_CCM_DEC_UPDATE_TEXT, // AES-CCM decryption, plaintext/ciphertext input
    RSIP_STATE_AES_CCM_DEC_VERI,        // AES-CCM encryption, verification
    RSIP_STATE_AES_CMAC,                // AES-CMAC
    RSIP_STATE_SHA,                     // SHA
    RSIP_STATE_HMAC,                    // HMAC
    RSIP_STATE_FWUP,                    // FirmwareUpdate
} rsip_state_t;

/** Block cipher modes of operation for AES */
typedef enum e_rsip_aes_cipher_mode
{
    RSIP_AES_CIPHER_MODE_ECB_ENC,      // Electronic Codebook (ECB) encryption
    RSIP_AES_CIPHER_MODE_ECB_DEC,      // Electronic Codebook (ECB) decryption
    RSIP_AES_CIPHER_MODE_CBC_ENC,      // Cipher Block Chaining (CBC) mode encryption
    RSIP_AES_CIPHER_MODE_CBC_DEC,      // Cipher Block Chaining (CBC) mode decryption
    RSIP_AES_CIPHER_MODE_CTR,          // Counter (CTR) encryption and decryption
} rsip_aes_cipher_mode_t;

/** AEAD modes of operation for AES */
typedef enum e_rsip_aes_aead_mode
{
    RSIP_AES_AEAD_MODE_GCM_ENC,        // Galois/Counter Mode (GCM) encryption
    RSIP_AES_AEAD_MODE_GCM_DEC,        // Galois/Counter Mode (GCM) decryption
    RSIP_AES_AEAD_MODE_CCM_ENC,        // Counter with CBC-MAC (CCM) encryption
    RSIP_AES_AEAD_MODE_CCM_DEC,        // Counter with CBC-MAC (CCM) decryption
} rsip_aes_aead_mode_t;

/** MAC modes of operation for AES */
typedef enum e_rsip_aes_mac_mode
{
    RSIP_AES_MAC_MODE_CMAC,            // Cipher-based MAC (CMAC)
} rsip_aes_mac_mode_t;

/* Working area for AES-CMAC */
typedef struct st_rsip_aes_cmac_handle
{
    const void * p_func;                               // Pointer to primitive functions
    uint8_t      buffer[RSIP_PRV_BYTE_SIZE_AES_BLOCK]; // Buffer for message
    uint32_t     buffered_length;                      // Buffered message length
    uint32_t     total_length;                         // Total message length
} rsip_aes_cmac_handle_t;

/* Working area for AES block cipher */
typedef struct st_rsip_aes_cihper_handle
{
    const void * p_func;               // Pointer to primitive functions
} rsip_aes_cipher_handle_t;

/* Working area for AES-GCM */
typedef struct st_rsip_aes_gcm_handle
{
    const void * p_func;                               // Pointer to primitive functions
    uint8_t      buffer[RSIP_PRV_BYTE_SIZE_AES_BLOCK]; // Buffer for AAD and plaintext/ciphertext
    uint32_t     buffered_length;                      // Buffered AAD and plaintext/ciphertext length
    uint32_t     total_length;                         // Total plaintext/ciphertext length
    uint32_t     total_aad_length;                     // Total AAD length
} rsip_aes_gcm_handle_t;

/* Working area for AES-CCM */
typedef struct st_rsip_aes_ccm_handle
{
    const void * p_func;                                          // Pointer to primitive functions
    uint8_t      wrapped_key[RSIP_BYTE_SIZE_WRAPPED_KEY_AES_256]; // Stored wrapped key
    uint8_t      nonce_buffer[RSIP_PRV_BYTE_SIZE_CCM_NONCE_MAX];  // Buffer for nonce
    uint32_t     nonce_length;                                    // Nonce length
    uint8_t      buffer[RSIP_PRV_BYTE_SIZE_CCM_AAD_MAX];          // Buffer for AAD and plaintext/ciphertext
    uint32_t     buffered_length;                                 // Buffered AAD and plaintext/ciphertext length
    uint32_t     input_aad_length;                                // Input AAD length
    uint32_t     total_aad_length;                                // Total AAD length
    uint32_t     input_length;                                    // Input plaintext/ciphertext length
    uint32_t     total_length;                                    // Total plaintext/ciphertext length
    uint32_t     tag_length;                                      // Tag length
} rsip_aes_ccm_handle_t;

/** Hash type */
typedef enum e_rsip_hash_type
{
    RSIP_HASH_TYPE_SHA224,             // SHA-224
    RSIP_HASH_TYPE_SHA256,             // SHA-256
    RSIP_HASH_TYPE_NUM                 // Number of hash types
} rsip_hash_type_t;

/* State that specifies functions that can be called next. This enum is private. */
typedef enum e_rsip_user_handle_state
{
    RSIP_USER_HANDLE_STATE_INIT,       // Init function can be called.
    RSIP_USER_HANDLE_STATE_RESUME,     // Resume
    RSIP_USER_HANDLE_STATE_UPDATE      // Update
} rsip_user_handle_state_t;

/** Working area for SHA functions. DO NOT MODIFY. */
typedef struct st_rsip_sha_handle
{
    rsip_hash_type_t         type;                                                      // Hash type
    uint8_t                  buffer[RSIP_CFG_BYTE_SIZE_SHA_BLOCK_MAX];                  // Stored message
    uint32_t                 buffered_length;                                           // Buffered message length
    uint32_t                 total_length;                                              // Total message length input to primitive
    uint32_t                 block_size;                                                // Block size
    uint8_t                  current_hash[RSIP_PRV_SHA224256_HASH_LENGTH_BYTE_SIZE];    // SHA224/256(32byte) are supported
    uint32_t                 internal_state[RSIP_CFG_WORD_SIZE_SHA_INTERNAL_STATE_MAX]; // Internal state
    rsip_user_handle_state_t handle_state;                                              // Handle state
} rsip_sha_handle_t;

/** Working area for HMAC functions. DO NOT MODIFY. */
typedef struct st_rsip_hmac_handle
{
    const void              * p_func;                                                     // Pointer to primitive functions
    uint8_t                   wrapped_key[RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_HMAC_MAX];       // Stored wrapped key
    uint8_t                   buffer[RSIP_CFG_BYTE_SIZE_HMAC_BLOCK_MAX];                  // Stored message
    uint32_t                  buffered_length;                                            // Buffered message length
    uint32_t                  total_length;                                               // Total message length input to primitive
    uint32_t                  block_size;                                                 // Block size
    uint32_t                  internal_state[RSIP_CFG_WORD_SIZE_HMAC_INTERNAL_STATE_MAX]; // Internal state
    rsip_user_handle_state_t  handle_state;                                               // Handle state
} rsip_hmac_handle_t;

/* Working area for RSIP cryptographic algorithms. This union is included in private control block. */
typedef union u_rsip_handle
{
    rsip_aes_cipher_handle_t aes_cipher; // AES block cipher
    rsip_aes_gcm_handle_t    aes_gcm;    // AES-GCM
    rsip_aes_ccm_handle_t    aes_ccm;    // AES-CCM
    rsip_aes_cmac_handle_t   aes_cmac;   // AES-CMAC
    rsip_sha_handle_t        sha;        // SHA
    rsip_hmac_handle_t       hmac;       // HMAC
} rsip_handle_t;

typedef struct st_rsip_cfg
{
    void const * p_extend;
} rsip_cfg_t;


/** RSIP private control block. DO NOT MODIFY. Initialization occurs when R_RSIP_Open() is called. */
typedef struct st_rsip_instance_ctrl
{
    uint32_t           open;           // Indicates whether the open() API has been successfully
    rsip_cfg_t const * p_cfg;          // Pointer to the configuration block
    rsip_handle_t      handle;         // Handle of algorithms that cannot be suspended
    rsip_state_t       state;          // Flags to limit the next API to call
} rsip_instance_ctrl_t;


/** Wrapped key structure for all supported algorithms.
 * The struct length of each algorithm is defined by RSIP_BYTE_SIZE_WRAPPED_KEY macro. */
typedef struct st_rsip_wrapped_key
{
    uint8_t alg;                       // Key algorithm
    uint8_t subtype;                   // Key sub type
    uint8_t info[2];                   // Reserved area
    uint8_t value[];                   // Variable length array to store the key value
} rsip_wrapped_key_t;


/** Key Update Key (KUK) */
typedef struct st_rsip_key_update_key
{
    uint8_t value[RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_KEY_UPDATE_KEY];
} rsip_key_update_key_t;

/** Wrapped User Factory Programming Key (W-UFPK) */
typedef struct st_rsip_wufpk
{
   uint8_t value[RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_UFPK];
} rsip_wufpk_t;

/** RSIP driver structure.  General RSIP functions implemented at the HAL layer follow this API. */
typedef struct st_rsip_api
{
    /**
     * Enables use of Renesas Secure IP functionality.
     *
     * @param[in,out] p_ctrl Pointer to control block.
     * @param[in]     p_cfg  Pointer to configuration structure.
     */
    fsp_err_t (* open)(rsip_ctrl_t * const p_ctrl, rsip_cfg_t const * const p_cfg);

    /**
     * Disables use of Renesas Secure IP functionality.
     *
     * @param[in,out]   p_ctrl     Pointer to control block.
     */
    fsp_err_t (* close)(rsip_ctrl_t * const p_ctrl);

    /**
     * Returns the version of this module.
     *
     * @param[in,out]  none.
     */
    uint32_t (* getversion)(void);

    /**
     * Generates a 128-bit random number.
     *
     * @param[in,out] p_ctrl   Pointer to control block.
     * @param[out]    p_random 128bit random numbers.
     */
    fsp_err_t (* randomNumberGenerate)(rsip_ctrl_t * const p_ctrl, uint8_t * const p_random);

    /**
     * Generate a wrapped symmetric key from a random number.
     * In this API, user key input is unnecessary.
     * By encrypting data using the wrapped key is output by this API, dead copying of data can be prevented.
     *
     * @param[in,out] p_ctrl        Pointer to control block.
     * @param[in]     key_type      Outputs key type.
     * @param[out]    p_wrapped_key Pointer to destination of wrapped key.
     *                              The length depends on key type. Refer "Key Size Table".
     */
    fsp_err_t (* keyGenerate)(rsip_ctrl_t * const p_ctrl, rsip_key_type_t const key_type,
                                rsip_wrapped_key_t * const p_wrapped_key);

    /**
     * Generate a wrapped asymmetric key pair from a random number. In this API, user key input is unnecessary.
     * By encrypting data using the wrapped key is output by this API, dead copying of data can be prevented.
     *
     * @param[in,out] p_ctrl                Pointer to control block.
     * @param[in]     key_pair_type         Output key pair type.
     * @param[out]    p_wrapped_public_key  Key index for Public Key.
     *                                      The length depends on the key type. Refer "Key Size Table".
     * @param[out]    p_wrapped_private_key Key index for Private Key.
     *                                      The length depends on the key type. Refer "Key Size Table".
     */
    fsp_err_t (* keyPairGenerate)(rsip_ctrl_t * const p_ctrl, rsip_key_pair_type_t const key_pair_type,
                                    rsip_wrapped_key_t * const p_wrapped_public_key,
                                    rsip_wrapped_key_t * const p_wrapped_private_key);

    /**
     * Decrypt the encrypted user key with Key Update Key (KUK) and wrap it with the Hardware Unique Key (HUK).
     *
     * @param[in,out] p_ctrl           Pointer to control block.
     * @param[in]     p_key_update_key Pointer to Key Update Key.
     * @param[in]     p_initial_vector Initialization vector when generating encrypted_key.
     *                                 The length is 16 bytes.
     * @param[in]     key_type         Inputs/Outputs key type.
     * @param[in]     p_encrypted_key  Encrypted user key. The length depends on the key type. Refer "Key Size Table".
     * @param[out]    p_wrapped_key    Pointer to destination of wrapped key.
     *                                 The length depends on key type. Refer "Key Size Table".
     */
    fsp_err_t (* encryptedKeyWrap)(rsip_ctrl_t * const p_ctrl, rsip_key_update_key_t const * const p_key_update_key,
                                    uint8_t const * const p_initial_vector, rsip_key_type_t const key_type,
                                    uint8_t const * const p_encrypted_key,
                                    rsip_wrapped_key_t * const p_wrapped_key);


    /**
    * Decrypts an encrypted user key with User Factory Programming Key (UFPK)
    * and wrap it with the Hardware Unique Key (HUK).
    *
    * @param[in,out] p_ctrl                                Pointer to control block.
    * @param[in]     p_wrapped_user_factory_programming_key Wrapped User Factory Programming Key (W-UFPK).
    *                                                       The length is 32 bytes.
    * @param[in]     p_initial_vector                       Initialization vector when generating encrypted_key.
    *                                                       The length is 16 bytes.
    * @param[in]     key_type                               Inputs/Outputs key type.
    * @param[in]     p_encrypted_key                        Encrypted user key. The length depends on the key type.
    * @param[out]    p_injected_key                         Pointer to destination of injected key.
    *                                                       The length depends on the key type.
    */
    fsp_err_t (* initialKeyWrap)(rsip_ctrl_t * const p_ctrl,
                                    rsip_wufpk_t const * const p_wrapped_user_factory_programming_key,
                                    uint8_t const * const      p_initial_vector,
                                    rsip_key_type_t const      key_type,
                                    uint8_t const * const      p_encrypted_key,
                                    uint8_t * const            p_injected_key);

    /**
     * Decrypts an encrypted Key Update Key (KUK) with User Factory Programming Key (UFPK)
     * and wrap it with the Hardware Unique Key (HUK).
     *
     * @param[in,out] p_ctrl                                 Pointer to control block.
     * @param[in]     p_wrapped_user_factory_programming_key Wrapped User Factory Programming Key (W-UFPK).
     *                                                       The length is 32 bytes.
     * @param[in]     p_initial_vector                       Initialization vector when generating encrypted_key.
     *                                                       The length is 16 bytes.
     * @param[in]     p_encrypted_key                        Encrypted key update key.
     * @param[out]    p_injected_key                         Pointer to destination of injected key update key.
     */
    fsp_err_t (* initialKeyUpdateKeyWrap)(rsip_ctrl_t * const p_ctrl,
                                            rsip_wufpk_t const * const    p_wrapped_user_factory_programming_key,
                                            uint8_t const * const         p_initial_vector,
                                            uint8_t const * const         p_encrypted_key,
                                            rsip_key_update_key_t * const p_injected_key);

    /**
     * This function provides Key Wrap algorithm compliant with RFC3394.
     * Using p_wrapped_kek to wrap p_wrapped_target_key, and output the result to p_rfc3394_wrapped_target_key.
     *
     * @param[in,out] p_ctrl                       Pointer to control block.
     * @param[in]     p_wrapped_kek                Pointer to wrapped key-encryption-key used to
     *                                             RFC3394-wrap the target key.
     * @param[in]     p_wrapped_target_key         Pointer to wrapped target key to be RFC3394-wrapped.
     * @param[out]    p_rfc3394_wrapped_target_key Pointer to destination of RFC3394-wrapped target key.
     */
    fsp_err_t (* rfc3394_KeyWrap)(rsip_ctrl_t * const              p_ctrl,
                                    rsip_wrapped_key_t const * const p_wrapped_kek,
                                    rsip_wrapped_key_t const * const p_wrapped_target_key,
                                    uint8_t * const                  p_rfc3394_wrapped_target_key);

    /**
     * This function provides Key Unwrap algorithm compliant with RFC3394.
     * Using p_wrapped_kek to unwrap p_rfc3394_wrapped_target_key, and output the result to p_wrapped_target_key.
     *
     * @param[in,out] p_ctrl                       Pointer to control block.
     * @param[in]     p_wrapped_kek                Pointer to wrapped key-encryption-key used to
     *                                             RFC3394-unwrap the target key.
     * @param[in]     key_type                     Key type of p_rfc3394_wrapped_target_key.
     * @param[in]     p_rfc3394_wrapped_target_key Pointer to AES-wrapped target key to be RFC3394-unwrapped.
     * @param[out]    p_wrapped_target_key         Pointer to destination of RFC3394-unwrapped target key.
     */
    fsp_err_t (* rfc3394_KeyUnwrap)(rsip_ctrl_t * const              p_ctrl,
                                    rsip_wrapped_key_t const * const p_wrapped_kek,
                                    rsip_key_type_t const            key_type,
                                    uint8_t const * const            p_rfc3394_wrapped_target_key,
                                    rsip_wrapped_key_t * const       p_wrapped_target_key);

    /**
     * This function provides the ability to construct structure data "rsip_wrapped_key_t" from injected key data.
     * The value of injected key is not validated in this API. Refer "Key Size Table" for supported key types.
     *
     * @param[in]  key_type                  Key type of p_injected_key.
     * @param[in]  p_injected_key            Pointer to key to be injected.
     * @param[out] p_wrapped_key             Pointer to destination of wrapped key.
     * @param[in]  wrapped_key_buffer_length Length of p_wrapped_key destination.
     *                                       It must be equal to or greater than actual wrapped key.
     */
    fsp_err_t (* injectedKeyImport)(rsip_key_type_t const      key_type,
                                    uint8_t const * const      p_injected_key,
                                    rsip_wrapped_key_t * const p_wrapped_key,
                                    uint32_t const             wrapped_key_buffer_length);

    /**
     * Exports public key parameters from a wrapped key.
     *
     * @param[in]  p_wrapped_public_key Key index for Public Key.
     *                                  The length depends on the key type. Refer "Key Size Table".
     * @param[out] p_raw_public_key     Pointer to destination of raw public key. The length depends on the key length.
     */
    fsp_err_t (* publicKeyExport)(rsip_wrapped_key_t const * const p_wrapped_public_key,
                                    uint8_t * const                  p_raw_public_key);

    /**
     * Set parameters of AES cipher.
     *
     * @param[in,out] p_ctrl           Pointer to control block.
     * @param[in]     mode             Block cipher modes of operation for AES.
     * @param[in]     p_wrapped_key    Pointer to wrapped key of AES or XTS-AES key.
     * @param[in]     p_initial_vector Pointer to initialization vector (IV) or nonce. The length is 16 bytes.
     *                                 @arg [ECB] Not required
     *                                 @arg [CBC][XTS] IV
     *                                 @arg [CTR] Nonce
     */
    fsp_err_t (* aesCipherInit)(rsip_ctrl_t * const p_ctrl, rsip_aes_cipher_mode_t const mode,
                                rsip_wrapped_key_t const * const p_wrapped_key, uint8_t const * const p_initial_vector);

    /**
     * Encrypt plaintext.
     *
     * @param[in,out] p_ctrl   Pointer to control block.
     * @param[in]     p_input  Pointer to input text. The length is given as the argument.
     * @param[out]    p_output Pointer to destination of output text. The length is given as the argument.
     * @param[in]     length   Byte length of input and output.
     *                         @arg [ECB][CBC][CTR] Must be 0 or a multiple of 16.
     *                         @arg [XTS] Must be 0 or greater than or equal to 16.
     *                         After an integer not divisible by 16 is input, update can no longer be executed.
     */
    fsp_err_t (* aesCipherUpdate)(rsip_ctrl_t * const p_ctrl, uint8_t const * const p_input, uint8_t * const p_output,
                                    uint32_t const length);

    /**
     * Finalize AES operation.
     *
     * @param[in,out] p_ctrl Pointer to control block.
     */
    fsp_err_t (* aesCipherFinish)(rsip_ctrl_t * const p_ctrl);

    /**
     * Prepares an AES-AEAD function.
     *
     * @param[in,out] p_ctrl        Pointer to control block.
     * @param[in]     mode          AEAD mode of operation.
     * @param[in]     p_wrapped_key Pointer to wrapped key of AES key.
     * @param[in]     p_nonce       Pointer to nonce. The length is nonce_length.
     * @param[in]     nonce_length  Byte length of nonce. Input 1 or more.
     */
    fsp_err_t (* aesAeadInit)(rsip_ctrl_t * const p_ctrl, rsip_aes_aead_mode_t const mode,
                                rsip_wrapped_key_t const * const p_wrapped_key, uint8_t const * const p_nonce,
                                uint32_t const nonce_length);

    /**
     * Set text and tag lengths for specific mode.
     *
     * @param[in,out] p_ctrl            Pointer to control block.
     * @param[in]     total_aad_length  Total AAD length.
     * @param[in]     total_text_length Total input and output text length.
     * @param[in]     tag_length        Input or output tag length.
     */
    fsp_err_t (* aesAeadLengthsSet)(rsip_ctrl_t * const p_ctrl, uint32_t const total_aad_length,
                                    uint32_t const total_text_length, uint32_t const tag_length);

    /**
     * Inputs additional authentication data.
     *
     * @param[in,out] p_ctrl     Pointer to control block.
     * @param[in]     p_aad      Additional authentication data. The length depends on aad_length.
     * @param[in]     aad_length Byte length of additional authentication data (0 or more bytes).
     *                           After starting input of plaintext, this value must always be 0.
     */
    fsp_err_t (* aesAeadAadUpdate)(rsip_ctrl_t * const p_ctrl, uint8_t const * const p_aad, uint32_t const aad_length);

    /**
     * Inputs test and executes encryption and decryption.
     *
     * @param[in,out] p_ctrl          Pointer to control block.
     * @param[in]     p_input         Pointer to input text. The length is input_length.
     * @param[in]     input_length    Byte length of input text (0 or more bytes).
     * @param[out]    p_output        Pointer to destination of output text. The length is p_output_length.
     * @param[out]    p_output_length Pointer to destination of output text length.
     */
    fsp_err_t (* aesAeadUpdate)(rsip_ctrl_t * const p_ctrl, uint8_t const * const p_input, uint32_t const input_length,
                                uint8_t * const p_output, uint32_t * const p_output_length);

    /**
     * Finalizes an AES-GCM encryption.
     *
     * If there is 16-byte fractional data indicated by the total data length of the value of p_plain that was input by
     * R_RSIP_AES_GCM_EncryptUpdate(), this API will output the result of encrypting that fractional data to p_cipher.
     * Here, the portion that does not reach 16 bytes will be padded with zeros.
     *
     * @param[in,out] p_ctrl          Pointer to control block.
     * @param[out]    p_output        Pointer to destination of output text. The fractional block is output.
     * @param[out]    p_output_length Pointer to destination of output text length.
     * @param[out]    p_tag           Pointer to destination of tag for authentication.
     *                                GCM : The length is 16 bytes.
     *                                      *If a different tag length is required, truncate the 16-byte tag
     *                                        to the required tag length (NIST SP800-38D 7.1).
     *                                CCM : The length is the value set by the API R_RSIP_AES_AEAD_LengthsSet().
     */
    fsp_err_t (* aesAeadFinish)(rsip_ctrl_t * const p_ctrl, uint8_t * const p_output, uint32_t * const p_output_length,
                                uint8_t * const p_tag);

    /**
     * Finalizes an AES-GCM decryption.
     *
     * If there is 16-byte fractional data indicated by the total data length of the value of p_cipher that was input by
     * R_RSIP_AES_GCM_DecryptUpdate(), this API will output the result of decrypting that fractional data to p_cipher.
     * Here, the portion that does not reach 16 bytes will be padded with zeros.
     *
     * @param[in,out] p_ctrl          Pointer to control block.
     * @param[out]    p_output        Pointer to destination of decrypted data.
     * @param[out]    p_output_length Pointer to destination of decrypted data length.
     * @param[in]     p_tag           Pointer to destination of tag for authentication.The length depends on tag_length.
     * @param[in]     tag_length      Byte length of tag. Must be 1 to 16.
     */
    fsp_err_t (* aesAeadVerify)(rsip_ctrl_t * const p_ctrl, uint8_t * const p_output, uint32_t * const p_output_length,
                                uint8_t const * const p_tag, uint32_t const tag_length);

    /**
     * Prepares an AES-MAC generation and verification.
     *
     * @param[in,out] p_ctrl        Pointer to control block.
     * @param[in]     mode          MAC mode of operation
     * @param[in]     p_wrapped_key Pointer to wrapped key of AES key.
     */
    fsp_err_t (* aesMacInit)(rsip_ctrl_t * const p_ctrl, rsip_aes_mac_mode_t const mode,
                                rsip_wrapped_key_t const * const p_wrapped_key);

    /**
     * Input message.
     * Inside this function, the data that is input by the user is buffered until
     * the input value of p_message exceeds 16 bytes.
     * If the input value, p_message, is not a multiple of 16 bytes, it will be padded within the function.
     *
     * @param[in,out] p_ctrl         Pointer to control block.
     * @param[in]     p_message      Pointer to message. The length is message_length.
     * @param[in]     message_length Byte length of message (0 or more bytes).
     */
    fsp_err_t (* aesMacUpdate)(rsip_ctrl_t * const p_ctrl, uint8_t const * const p_message,
                                uint32_t const message_length);

    /**
     * Finalizes an AES-CMAC generation.
     *
     * @param[in,out] p_ctrl Pointer to control block.
     * @param[out]    p_mac  Pointer to destination of MAC. The length is 16 bytes.
     */
    fsp_err_t (* aesMacSignFinish)(rsip_ctrl_t * const p_ctrl, uint8_t * const p_mac);

    /** Finalizes an AES-CMAC verification.
     *
     * @param[in,out] p_ctrl     Pointer to control block.
     * @param[in]     p_mac      Pointer to MAC. The length depends on mac_length.
     * @param[in]     mac_length Byte length of MAC. Must be 2 to 16.
     */
    fsp_err_t (* aesMacVerifyFinish)(rsip_ctrl_t * const p_ctrl, uint8_t const * const p_mac,
                                        uint32_t const mac_length);

    /**
     * Signs a hashed message. The message hash should be generated in advance.
     *
     * @param[in,out] p_ctrl                Pointer to control block.
     * @param[in]     p_wrapped_private_key Pointer to wrapped key of ECC private key.
     * @param[in]     p_hash                Pointer to hash value. The length is as same as the key length.
     * @param[out]    p_signature           Pointer to destination of signature (r, s).
     *                                      The length is twice as long as the key length.
     */
    fsp_err_t (* ecdsaSign)(rsip_ctrl_t * const p_ctrl, rsip_wrapped_key_t const * const p_wrapped_private_key,
                            uint8_t const * const p_hash, uint8_t * const p_signature);

    /**
     * Verifies a hashed message. The message hash should be generated in advance.
     *
     * @param[in,out] p_ctrl               Pointer to control block.
     * @param[in]     p_wrapped_public_key Pointer to wrapped key of ECC public key.
     * @param[in]     p_hash               Pointer to hash value. The length is as same as the key length.
     * @param[in]     p_signature          Pointer to signature (r, s).The length is twice as long as the key length.
     */
    fsp_err_t (* ecdsaVerify)(rsip_ctrl_t * const p_ctrl, rsip_wrapped_key_t const * const p_wrapped_public_key,
                                uint8_t const * const p_hash, uint8_t const * const p_signature);

    /**
     * Prepares a SHA generation.
     *
     * @param[in,out] p_ctrl    Pointer to control block.
     * @param[in]     hash_type Generating hash type.
     */
    fsp_err_t (* shaInit)(rsip_ctrl_t * const p_ctrl, rsip_hash_type_t const hash_type);

    /**
     * Inputs message.
     *
     * @param[in,out] p_ctrl         Pointer to control block.
     * @param[in]     p_message      Pointer to message. The length is message_length.
     * @param[in]     message_length Byte length of message (0 or more bytes).
     */
    fsp_err_t (* shaUpdate)(rsip_ctrl_t * const p_ctrl, uint8_t const * const p_message, uint32_t const message_length);

    /**
     * Finalizes a SHA generation.
     *
     * @param[in,out] p_ctrl   Pointer to control block.
     * @param[out]    p_digest Pointer to destination of message digest. The length depends on hash type.
     */
    fsp_err_t (* shaFinish)(rsip_ctrl_t * const p_ctrl, uint8_t * const p_digest);

    /**
     * Suspend SHA generation.
     * This API allows you to suspend processing,
     * for example, if you are in the middle of computing digest value for successive
     * chunks of the message and need to perform another process.
     *
     * @param[in,out] p_ctrl   Pointer to control block.
     * @param[out]    p_handle Pointer to destination of SHA control block.
     */
    fsp_err_t (* shaSuspend)(rsip_ctrl_t * const p_ctrl, rsip_sha_handle_t * const p_handle);

    /**
     * Resume SHA generation.
     * This API allows you to resume a process that has been suspended by R_RSIP_SHA_Suspend() API.
     *
     * @param[in,out] p_ctrl   Pointer to control block.
     * @param[in]     p_handle Pointer to SHA control block.
     */
    fsp_err_t (* shaResume)(rsip_ctrl_t * const p_ctrl, rsip_sha_handle_t const * const p_handle);

    /**
     * Prepares a HMAC generation.
     *
     * @param[in,out] p_ctrl        Pointer to control block.
     * @param[in]     p_wrapped_key Pointer to wrapped key of HMAC key.
     */
    fsp_err_t (* hmacInit)(rsip_ctrl_t * const p_ctrl, rsip_wrapped_key_t const * const p_wrapped_key);

    /**
     * Inputs message.
     *
     * @param[in,out] p_ctrl         Pointer to control block.
     * @param[in]     p_message      Pointer to message. The length is message_length.
     * @param[in]     message_length Byte length of message (0 or more bytes).
     */
    fsp_err_t (* hmacUpdate)(rsip_ctrl_t * const p_ctrl, uint8_t const * const p_message,
                                uint32_t const message_length);

    /**
     * Finalizes a HMAC generation.
     *
     * @param[in,out] p_ctrl   Pointer to control block.
     * @param[out]    p_mac    Pointer to destination of message digest. The length depends on MAC type.
     */
    fsp_err_t (* hmacSignFinish)(rsip_ctrl_t * const p_ctrl, uint8_t * const p_mac);

    /**
     * Finalizes a HMAC verification.
     *
     * @param[in,out] p_ctrl     Pointer to control block.
     * @param[in]     p_mac      Pointer to MAC. The length depends on mac_length.
     * @param[in]     mac_length Byte length of MAC.
     */
    fsp_err_t (* hmacVerifyFinish)(rsip_ctrl_t * const p_ctrl, uint8_t const * const p_mac, uint32_t const mac_length);

    /**
     * Start Firmware Update state.
     *
     * @param[in,out] p_ctrl     Pointer to control block.
     */
    fsp_err_t (* startupdatefirmware)(rsip_ctrl_t * const p_ctrl);

    /**
     * Prepares the FWUP MAC sign.
     *
     * @param[in,out] p_ctrl                            Pointer to control block.
     * @param[in]     p_wrapped_key_encryption_key      Wrapped Key Encryption Key.
     * @param[in]     p_encrypted_image_encryption_key  Encrypted Image Encryption Key.
     * @param[in]     p_initial_vector                  Initialization vector area for decrypting encrypted firmware.
     * @param[in]     firmware_size                     The entire firmware length in bytes to be decrypted.
     */
    fsp_err_t (* firmwareMacSignInit)(rsip_ctrl_t * const p_ctrl,
                                        rsip_wufpk_t const * const p_wrapped_key_encryption_key,
                                        uint8_t const * const p_encrypted_image_encryption_key,
                                        uint8_t const * const p_initial_vector, uint32_t const firmware_size);

    /**
     * Update the FWUP MAC sign.
     *
     * @param[in,out] p_ctrl       Pointer to control block.
     * @param[in]     p_input      Input firmware area.
     * @param[in]     p_output     Output firmware area.
     * @param[in]     length       Input firmware byte length. The length must be in multiples of 16 bytes.
     */
    fsp_err_t (* firmwareMacSignUpdate)(rsip_ctrl_t * const p_ctrl, uint8_t const * const p_input,
                                        uint8_t * const p_output, uint32_t const length);

    /**
     * Finalizes the FWUP MAC sign.
     *
     * @param[in,out] p_ctrl         Pointer to control block.
     * @param[in]     p_input        Input firmware area.
     * @param[in]     p_input_mac    Input firmware mac.
     * @param[in]     length         Input firmware byte length. The length must be in multiples of 16 bytes.
     * @param[in]     p_output       Output firmware area.
     * @param[in]     p_output_mac   Output firmware mac.
     */
    fsp_err_t (* firmwareMacSignFinish)(rsip_ctrl_t * const p_ctrl, uint8_t const * const p_input,
                                        uint8_t const * const p_input_mac, uint32_t const length,
                                        uint8_t * const p_output, uint8_t * const p_output_mac);

    /**
     * Prepares the SecureBoot MAC verify.
     *
     * @param[in,out] p_ctrl                            Pointer to control block.
     */
    fsp_err_t (* securebootMacVerifyInit)(rsip_ctrl_t * const p_ctrl);

    /**
     * Update the SecureBoot MAC verify.
     *
     * @param[in,out] p_ctrl        Pointer to control block.
     * @param[in]     p_input       Input firmware area.
     * @param[in]     input_length  Input firmware byte length. The length must be in multiples of 16 bytes.
     */
    fsp_err_t (* securebootMacVerifyUpdate)(rsip_ctrl_t * const p_ctrl, uint8_t const * const p_input,
                                            uint32_t const input_length);

    /**
     * Finalizes the SecureBoot MAC verify.
     *
     * @param[in,out] p_ctrl        Pointer to control block.
     * @param[in]     p_input       Input firmware area.
     * @param[in]     p_mac         Input firmware mac.
     * @param[in]     input_length  Input firmware byte length. The length must be in multiples of 16 bytes.
     */
    fsp_err_t (* securebootMacVerifyFinish)(rsip_ctrl_t * const p_ctrl, uint8_t const * const p_input,
                                            uint8_t const * const p_mac, uint32_t const input_length);

} rsip_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_rsip_instance
{
    rsip_ctrl_t      * p_ctrl;         // Pointer to the control structure for this instance
    rsip_cfg_t const * p_cfg;          // Pointer to the configuration structure for this instance
    rsip_api_t const * p_api;          // Pointer to the API structure for this instance
} rsip_instance_t;


/*******************************************************************************************************************//**
 * @} (end addtogroup RSIP_PROTECTED_API)
 **********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
/* r_rsip.c */
fsp_err_t R_RSIP_Open (rsip_ctrl_t * const p_ctrl, rsip_cfg_t const * const p_cfg);
fsp_err_t R_RSIP_Close (rsip_ctrl_t * const p_ctrl);
uint32_t  R_RSIP_GetVersion (void);
fsp_err_t R_RSIP_RandomNumberGenerate (rsip_ctrl_t * const p_ctrl, uint8_t * const p_random);
fsp_err_t R_RSIP_KeyGenerate (rsip_ctrl_t * const        p_ctrl,
                                rsip_key_type_t const      key_type,
                                rsip_wrapped_key_t * const p_wrapped_key);
fsp_err_t R_RSIP_KeyPairGenerate (rsip_ctrl_t * const        p_ctrl,
                                    rsip_key_pair_type_t const key_pair_type,
                                    rsip_wrapped_key_t * const p_wrapped_public_key,
                                    rsip_wrapped_key_t * const p_wrapped_private_key);
fsp_err_t R_RSIP_EncryptedKeyWrap (rsip_ctrl_t * const                 p_ctrl,
                                    rsip_key_update_key_t const * const p_key_update_key,
                                    uint8_t const * const               p_initial_vector,
                                    rsip_key_type_t const               key_type,
                                    uint8_t const * const               p_encrypted_key,
                                    rsip_wrapped_key_t * const          p_wrapped_key);
fsp_err_t R_RSIP_RFC3394_KeyWrap (rsip_ctrl_t * const              p_ctrl,
                                    rsip_wrapped_key_t const * const p_wrapped_kek,
                                    rsip_wrapped_key_t const * const p_wrapped_target_key,
                                    uint8_t * const                  p_rfc3394_wrapped_target_key);
fsp_err_t R_RSIP_RFC3394_KeyUnwrap (rsip_ctrl_t * const              p_ctrl,
                                    rsip_wrapped_key_t const * const p_wrapped_kek,
                                    rsip_key_type_t const            key_type,
                                    uint8_t const * const            p_rfc3394_wrapped_target_key,
                                    rsip_wrapped_key_t * const       p_wrapped_target_key);
fsp_err_t R_RSIP_InjectedKeyImport (rsip_key_type_t const      key_type,
                                    uint8_t const * const      p_injected_key,
                                    rsip_wrapped_key_t * const p_wrapped_key,
                                    uint32_t const             wrapped_key_buffer_length);
fsp_err_t R_RSIP_PublicKeyExport (rsip_wrapped_key_t const * const p_wrapped_public_key,
                                    uint8_t * const                  p_raw_public_key);
fsp_err_t R_RSIP_InitialKeyWrap (rsip_ctrl_t * const p_ctrl,
                                rsip_wufpk_t const * const p_wrapped_user_factory_programming_key,
                                uint8_t const * const p_initial_vector, rsip_key_type_t const key_type,
                                uint8_t const * const p_encrypted_key, uint8_t * const p_injected_key);
fsp_err_t R_RSIP_InitialKeyUpdateKeyWrap (rsip_ctrl_t * const p_ctrl,
                                        rsip_wufpk_t const * const p_wrapped_user_factory_programming_key,
                                        uint8_t const * const p_initial_vector, uint8_t const * const p_encrypted_key,
                                        rsip_key_update_key_t * const p_wrapped_key);

/* r_rsip_aes.c */
fsp_err_t R_RSIP_AES_Cipher_Init (rsip_ctrl_t * const              p_ctrl,
                                    rsip_aes_cipher_mode_t const     mode,
                                    rsip_wrapped_key_t const * const p_wrapped_key,
                                    uint8_t const * const            p_initial_vector);
fsp_err_t R_RSIP_AES_Cipher_Update (rsip_ctrl_t * const   p_ctrl,
                                    uint8_t const * const p_input,
                                    uint8_t * const       p_output,
                                    uint32_t const        length);
fsp_err_t R_RSIP_AES_Cipher_Finish (rsip_ctrl_t * const p_ctrl);
fsp_err_t R_RSIP_AES_AEAD_Init (rsip_ctrl_t * const              p_ctrl,
                                rsip_aes_aead_mode_t const       mode,
                                rsip_wrapped_key_t const * const p_wrapped_key,
                                uint8_t const * const            p_nonce,
                                uint32_t const                   nonce_length);
fsp_err_t R_RSIP_AES_AEAD_LengthsSet (rsip_ctrl_t * const p_ctrl,
                                        uint32_t const      total_aad_length,
                                        uint32_t const      total_text_length,
                                        uint32_t const      tag_length);
fsp_err_t R_RSIP_AES_AEAD_AADUpdate (rsip_ctrl_t * const p_ctrl,
                                    uint8_t const * const p_aad,
                                    uint32_t const aad_length);
fsp_err_t R_RSIP_AES_AEAD_Update (rsip_ctrl_t * const   p_ctrl,
                                    uint8_t const * const p_input,
                                    uint32_t const        input_length,
                                    uint8_t * const       p_output,
                                    uint32_t * const      p_output_length);
fsp_err_t R_RSIP_AES_AEAD_Finish (rsip_ctrl_t * const p_ctrl,
                                    uint8_t * const     p_output,
                                    uint32_t * const    p_output_length,
                                    uint8_t * const     p_tag);
fsp_err_t R_RSIP_AES_AEAD_Verify (rsip_ctrl_t * const   p_ctrl,
                                    uint8_t * const       p_output,
                                    uint32_t * const      p_output_length,
                                    uint8_t const * const p_tag,
                                    uint32_t const        tag_length);

fsp_err_t R_RSIP_AES_MAC_Init (rsip_ctrl_t * const              p_ctrl,
                                rsip_aes_mac_mode_t const        mode,
                                rsip_wrapped_key_t const * const p_wrapped_key);
fsp_err_t R_RSIP_AES_MAC_Update (rsip_ctrl_t * const   p_ctrl,
                                uint8_t const * const p_message,
                                uint32_t const        message_length);
fsp_err_t R_RSIP_AES_MAC_SignFinish (rsip_ctrl_t * const p_ctrl, uint8_t * const p_mac);
fsp_err_t R_RSIP_AES_MAC_VerifyFinish (rsip_ctrl_t * const p_ctrl, uint8_t const * const p_mac,
                                        uint32_t const mac_length);

/* r_rsip_ecc.c */
fsp_err_t R_RSIP_ECDSA_Sign (rsip_ctrl_t * const              p_ctrl,
                                rsip_wrapped_key_t const * const p_wrapped_private_key,
                                uint8_t const * const            p_hash,
                                uint8_t * const                  p_signature);
fsp_err_t R_RSIP_ECDSA_Verify (rsip_ctrl_t * const              p_ctrl,
                                rsip_wrapped_key_t const * const p_wrapped_public_key,
                                uint8_t const * const            p_hash,
                                uint8_t const * const            p_signature);

/* r_rsip_sha.c */
fsp_err_t R_RSIP_SHA_Init (rsip_ctrl_t * const p_ctrl, rsip_hash_type_t const hash_type);
fsp_err_t R_RSIP_SHA_Update (rsip_ctrl_t * const p_ctrl, uint8_t const * const p_message,
                                uint32_t const message_length);
fsp_err_t R_RSIP_SHA_Finish (rsip_ctrl_t * const p_ctrl, uint8_t * const p_digest);
fsp_err_t R_RSIP_SHA_Suspend (rsip_ctrl_t * const p_ctrl, rsip_sha_handle_t * const p_handle);
fsp_err_t R_RSIP_SHA_Resume (rsip_ctrl_t * const p_ctrl, rsip_sha_handle_t const * const p_handle);

fsp_err_t R_RSIP_HMAC_Init (rsip_ctrl_t * const p_ctrl, rsip_wrapped_key_t const * const p_wrapped_key);
fsp_err_t R_RSIP_HMAC_Update (rsip_ctrl_t * const p_ctrl, uint8_t const * const p_message,
                                uint32_t const message_length);
fsp_err_t R_RSIP_HMAC_SignFinish (rsip_ctrl_t * const p_ctrl, uint8_t * const p_mac);
fsp_err_t R_RSIP_HMAC_VerifyFinish (rsip_ctrl_t * const p_ctrl, uint8_t const * const p_mac, uint32_t const mac_length);

/* r_rsip_fwup.c */
fsp_err_t R_RSIP_FWUP_StartUpdateFirmware (rsip_ctrl_t * const p_ctrl);
fsp_err_t R_RSIP_FWUP_MAC_Sign_Init (rsip_ctrl_t * const p_ctrl,
                                    rsip_wrapped_key_t const * const p_wrapped_key_encryption_key,
                                    uint8_t const * const p_encrypted_image_encryption_key,
                                    uint8_t const * const p_initial_vector, uint32_t const firmware_size);
fsp_err_t R_RSIP_FWUP_MAC_Sign_Update (rsip_ctrl_t * const p_ctrl, uint8_t const * const p_input,
                                    uint8_t * const p_output, uint32_t const length);
fsp_err_t R_RSIP_FWUP_MAC_Sign_Finish (rsip_ctrl_t * const p_ctrl, uint8_t const * const p_input,
                                    uint8_t const * const p_input_mac, uint32_t const length,
                                    uint8_t * const p_output, uint8_t * const p_output_mac);
fsp_err_t R_RSIP_SB_MAC_Verify_Init (rsip_ctrl_t * const p_ctrl);
fsp_err_t R_RSIP_SB_MAC_Verify_Update (rsip_ctrl_t * const p_ctrl, uint8_t const * const p_input,
                                        uint32_t const input_length);
fsp_err_t R_RSIP_SB_MAC_Verify_Finish (rsip_ctrl_t * const p_ctrl, uint8_t const * const p_input,
                                        uint8_t const * const p_mac, uint32_t const input_length);

#endif  /* R_RSIP_RX_HEADER_FILE */

