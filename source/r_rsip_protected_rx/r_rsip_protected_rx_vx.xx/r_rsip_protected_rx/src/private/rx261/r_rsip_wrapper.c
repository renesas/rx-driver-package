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
* File Name    : r_rsip_wrapper.c
* Description  : Interface definition for the RSIP wrapper.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 15.10.2024 1.00     First Release.
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_rsip_private.h"
#include "../../primitive/rx261/r_rsip_primitive.h"
#include "r_rsip_wrapper.h"
#include "r_rsip_public.h"
#include "r_rsip_protected_rx_config.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* For AES-ECB/CBC/CTR */
#define RSIP_PRV_CMD_AES_CIPHER_ECB_ENCRYPT                 (0U)
#define RSIP_PRV_CMD_AES_CIPHER_ECB_DECRYPT                 (1U)
#define RSIP_PRV_CMD_AES_CIPHER_CBC_ENCRYPT                 (2U)
#define RSIP_PRV_CMD_AES_CIPHER_CBC_DECRYPT                 (3U)
#define RSIP_PRV_CMD_AES_CIPHER_CTR_CRYPT                   (4U)

/* For AES-CMAC. These commands do not require byte swapping */
#define RSIP_PRV_CMD_AES_CMAC_GENERATE_WITHOUT_REMAINDER    (0U)
#define RSIP_PRV_CMD_AES_CMAC_GENERATE_WITH_REMAINDER       (1U)
#define RSIP_PRV_CMD_AES_CMAC_VERIFY_WITHOUT_REMAINDER      (2U)
#define RSIP_PRV_CMD_AES_CMAC_VERIFY_WITH_REMAINDER         (3U)

/* For RFC3394 Key Wrap */
#define RSIP_PRV_WORD_SIZE_RFC3394_WRAPPED_KEY_AES_128      (6U)
#define RSIP_PRV_WORD_SIZE_RFC3394_WRAPPED_KEY_AES_256      (10U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
static const uint32_t gs_cmd_aes_mode[] =
{
    [RSIP_AES_MODE_EXTENDED_ECB_ENCRYPT] = RSIP_PRV_CMD_AES_CIPHER_ECB_ENCRYPT,
    [RSIP_AES_MODE_EXTENDED_ECB_DECRYPT] = RSIP_PRV_CMD_AES_CIPHER_ECB_DECRYPT,
    [RSIP_AES_MODE_EXTENDED_CBC_ENCRYPT] = RSIP_PRV_CMD_AES_CIPHER_CBC_ENCRYPT,
    [RSIP_AES_MODE_EXTENDED_CBC_DECRYPT] = RSIP_PRV_CMD_AES_CIPHER_CBC_DECRYPT,
    [RSIP_AES_MODE_EXTENDED_CTR_CRYPT]   = RSIP_PRV_CMD_AES_CIPHER_CTR_CRYPT
};

typedef enum e_rsip_rfc3394_key_wrap_type
{
    RSIP_RFC3394_KEY_WRAP_TYPE_AES128,
    RSIP_RFC3394_KEY_WRAP_TYPE_AES256 = 0x02,
} rsip_rfc3394_key_wrap_type_t;

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

static rsip_err_t select_rfc3394_key_wrap_mode (const rsip_key_type_t key_type,
                                                uint32_t              WrappedKeyType[]);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

rsip_err_t r_rsip_wrapper_pf0_secp256r1(const uint32_t InData_KeyIndex[],
                                        const uint32_t InData_MsgDgst[],
                                        uint32_t       OutData_Signature[])
{
    const uint32_t * InData_DomainParam = DomainParam_NIST_P256;
    uint32_t         cv_type            = change_endian_long(RSIP_ECC_CURVE_TYPE_NIST);

    return r_rsip_pf0(&cv_type, InData_KeyIndex, InData_MsgDgst, InData_DomainParam, OutData_Signature);
}

rsip_err_t r_rsip_wrapper_pf0_brainpoolp256r1(const uint32_t InData_KeyIndex[],
                                        const uint32_t InData_MsgDgst[],
                                        uint32_t       OutData_Signature[])
{
    const uint32_t * InData_DomainParam = DomainParam_Brainpool_256r1;
    uint32_t         cv_type            = change_endian_long(RSIP_ECC_CURVE_TYPE_Brainpool);

    return r_rsip_pf0(&cv_type, InData_KeyIndex, InData_MsgDgst, InData_DomainParam, OutData_Signature);
}

rsip_err_t r_rsip_wrapper_pf0_secp256k1(const uint32_t InData_KeyIndex[],
                                        const uint32_t InData_MsgDgst[],
                                        uint32_t       OutData_Signature[])
{
    const uint32_t * InData_DomainParam = DomainParam_Koblitz_secp256k1;
    uint32_t         cv_type            = change_endian_long(RSIP_ECC_CURVE_TYPE_Koblitz);

    return r_rsip_pf0(&cv_type, InData_KeyIndex, InData_MsgDgst, InData_DomainParam, OutData_Signature);
}

rsip_err_t r_rsip_wrapper_pf1_secp256r1(const uint32_t InData_KeyIndex[],
                                        const uint32_t InData_MsgDgst[],
                                        const uint32_t InData_Signature[])
{
    const uint32_t * InData_DomainParam = DomainParam_NIST_P256;
    uint32_t         cv_type            = change_endian_long(RSIP_ECC_CURVE_TYPE_NIST);

    return r_rsip_pf1(&cv_type, InData_KeyIndex, InData_MsgDgst, InData_Signature, InData_DomainParam);
}

rsip_err_t r_rsip_wrapper_pf1_brainpoolp256r1(const uint32_t InData_KeyIndex[],
                                        const uint32_t InData_MsgDgst[],
                                        const uint32_t InData_Signature[])
{
    const uint32_t * InData_DomainParam = DomainParam_Brainpool_256r1;
    uint32_t         cv_type            = change_endian_long(RSIP_ECC_CURVE_TYPE_Brainpool);

    return r_rsip_pf1(&cv_type, InData_KeyIndex, InData_MsgDgst, InData_Signature, InData_DomainParam);
}

rsip_err_t r_rsip_wrapper_pf1_secp256k1(const uint32_t InData_KeyIndex[],
                                        const uint32_t InData_MsgDgst[],
                                        const uint32_t InData_Signature[])
{
    const uint32_t * InData_DomainParam = DomainParam_Koblitz_secp256k1;
    uint32_t         cv_type            = change_endian_long(RSIP_ECC_CURVE_TYPE_Koblitz);

    return r_rsip_pf1(&cv_type, InData_KeyIndex, InData_MsgDgst, InData_Signature, InData_DomainParam);
}

rsip_err_t r_rsip_wrapper_pf4_secp256r1(uint32_t OutData_PubKeyIndex[], uint32_t OutData_PrivKeyIndex[])
{
    uint32_t         InData_CurveType    = change_endian_long(RSIP_ECC_CURVE_TYPE_NIST);
    uint32_t const * p_domain_param      = DomainParam_NIST_P256;

    return r_rsip_pf4(&InData_CurveType, p_domain_param, OutData_PubKeyIndex, OutData_PrivKeyIndex);
}

rsip_err_t r_rsip_wrapper_pf4_brainpoolp256r1(uint32_t OutData_PubKeyIndex[], uint32_t OutData_PrivKeyIndex[])
{
    uint32_t         InData_CurveType    = change_endian_long(RSIP_ECC_CURVE_TYPE_Brainpool);
    uint32_t const * p_domain_param      = DomainParam_Brainpool_256r1;

    return r_rsip_pf4(&InData_CurveType, p_domain_param, OutData_PubKeyIndex, OutData_PrivKeyIndex);
}

rsip_err_t r_rsip_wrapper_pf4_secp256k1(uint32_t OutData_PubKeyIndex[], uint32_t OutData_PrivKeyIndex[])
{
    uint32_t         InData_CurveType    = change_endian_long(RSIP_ECC_CURVE_TYPE_Koblitz);
    uint32_t const * p_domain_param      = DomainParam_Koblitz_secp256k1;

    return r_rsip_pf4(&InData_CurveType, p_domain_param, OutData_PubKeyIndex, OutData_PrivKeyIndex);
}

RSIP_SEC_P_SECURE_BOOT_ERASE
rsip_err_t r_rsip_p6e_aes128(const uint32_t InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_AES128;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_AES128_INST_DATA_WORD;
    uint32_t InData_SharedKeyIndex = RSIP_INSTALL_KEY_RING_INDEX;

    return r_rsip_p6e_r1(&LC, &CMD, &InData_SharedKeyIndex, InData_SessionKey, InData_IV,
                        InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}

RSIP_SEC_P_SECURE_BOOT_ERASE
rsip_err_t r_rsip_p6e_aes256(const uint32_t InData_SessionKey[],
                            const uint32_t InData_IV[],
                            const uint32_t InData_InstData[],
                            uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_AES256;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_AES256_INST_DATA_WORD;
    uint32_t InData_SharedKeyIndex = RSIP_INSTALL_KEY_RING_INDEX;

    return r_rsip_p6e_r1(&LC, &CMD, &InData_SharedKeyIndex, InData_SessionKey, InData_IV,
                        InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}

RSIP_SEC_P_SECURE_BOOT_ERASE
rsip_err_t r_rsip_p6e_eccp256_public(const uint32_t InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_ECC_SECP256R1_PUBLIC;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_ECC_SECP256R1_PUBLIC_KEY_INST_DATA_WORD;
    uint32_t InData_SharedKeyIndex = RSIP_INSTALL_KEY_RING_INDEX;

    return r_rsip_p6e_r1(&LC, &CMD, &InData_SharedKeyIndex, InData_SessionKey, InData_IV,
                        InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}

RSIP_SEC_P_SECURE_BOOT_ERASE
rsip_err_t r_rsip_p6e_eccp256_private(const uint32_t InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_ECC_SECP256R1_PRIVATE;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_ECC_SECP256R1_PRIVATE_KEY_INST_DATA_WORD;
    uint32_t InData_SharedKeyIndex = RSIP_INSTALL_KEY_RING_INDEX;

    return r_rsip_p6e_r1(&LC, &CMD, &InData_SharedKeyIndex, InData_SessionKey, InData_IV,
                        InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}

RSIP_SEC_P_SECURE_BOOT_ERASE
rsip_err_t r_rsip_p6e_eccp256r1_public(const uint32_t InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_ECC_BRAINPOOLP256R1_PUBLIC;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_ECC_SECP256R1_PUBLIC_KEY_INST_DATA_WORD;
    uint32_t InData_SharedKeyIndex = RSIP_INSTALL_KEY_RING_INDEX;

    return r_rsip_p6e_r1(&LC, &CMD, &InData_SharedKeyIndex, InData_SessionKey, InData_IV,
                        InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}

RSIP_SEC_P_SECURE_BOOT_ERASE
rsip_err_t r_rsip_p6e_eccp256r1_private(const uint32_t InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_ECC_BRAINPOOLP256R1_PRIVATE;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_ECC_SECP256R1_PRIVATE_KEY_INST_DATA_WORD;
    uint32_t InData_SharedKeyIndex = RSIP_INSTALL_KEY_RING_INDEX;

    return r_rsip_p6e_r1(&LC, &CMD, &InData_SharedKeyIndex, InData_SessionKey, InData_IV,
                        InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}

RSIP_SEC_P_SECURE_BOOT_ERASE
rsip_err_t r_rsip_p6e_eccsecp256k1_public(const uint32_t InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_ECC_SECP256K1_PUBLIC;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_ECC_SECP256R1_PUBLIC_KEY_INST_DATA_WORD;
    uint32_t InData_SharedKeyIndex = RSIP_INSTALL_KEY_RING_INDEX;

    return r_rsip_p6e_r1(&LC, &CMD, &InData_SharedKeyIndex, InData_SessionKey, InData_IV,
                        InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}

RSIP_SEC_P_SECURE_BOOT_ERASE
rsip_err_t r_rsip_p6e_eccsecp256k1_private(const uint32_t InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_ECC_SECP256K1_PRIVATE;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_ECC_SECP256R1_PRIVATE_KEY_INST_DATA_WORD;
    uint32_t InData_SharedKeyIndex = RSIP_INSTALL_KEY_RING_INDEX;

    return r_rsip_p6e_r1(&LC, &CMD, &InData_SharedKeyIndex, InData_SessionKey, InData_IV,
                        InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}

RSIP_SEC_P_SECURE_BOOT_ERASE
rsip_err_t r_rsip_p6e_hmacsha224(const uint32_t InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_HMAC_SHA224;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_HMAC_SHA256_KEY_INST_DATA_WORD;
    uint32_t InData_SharedKeyIndex = RSIP_INSTALL_KEY_RING_INDEX;

    return r_rsip_p6e_r1(&LC, &CMD, &InData_SharedKeyIndex, InData_SessionKey, InData_IV,
                        InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}

RSIP_SEC_P_SECURE_BOOT_ERASE
rsip_err_t r_rsip_p6e_hmacsha256(const uint32_t InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_HMAC_SHA256;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_HMAC_SHA256_KEY_INST_DATA_WORD;
    uint32_t InData_SharedKeyIndex = RSIP_INSTALL_KEY_RING_INDEX;

    return r_rsip_p6e_r1(&LC, &CMD, &InData_SharedKeyIndex, InData_SessionKey, InData_IV,
                        InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}

RSIP_SEC_P_SECURE_BOOT_ERASE
rsip_err_t r_rsip_p6e_aes256_KUK(const uint32_t InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_KUK;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_AES256_INST_DATA_WORD;
    uint32_t InData_SharedKeyIndex = RSIP_INSTALL_KEY_RING_INDEX;

    return r_rsip_p6e_r1(&LC, &CMD, &InData_SharedKeyIndex, InData_SessionKey, InData_IV,
                        InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}
RSIP_SEC_DEFAULT

rsip_err_t r_rsip_wrapper_p6f_aes128(const uint32_t InData_IV[],
                                    const uint32_t InData_InstData[],
                                    uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_AES128;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_AES128_INST_DATA_WORD;

    return r_rsip_p6f_r1(&LC, &CMD, InData_IV, InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}


rsip_err_t r_rsip_wrapper_p6f_aes256(const uint32_t InData_IV[],
                                    const uint32_t InData_InstData[],
                                    uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_AES256;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_AES256_INST_DATA_WORD;

    return r_rsip_p6f_r1(&LC, &CMD, InData_IV, InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}

rsip_err_t r_rsip_wrapper_p6f_secp256r1_pub(const uint32_t InData_IV[],
                                            const uint32_t InData_InstData[],
                                            uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_ECC_SECP256R1_PUBLIC;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_ECC_SECP256R1_PUBLIC_KEY_INST_DATA_WORD;

    return r_rsip_p6f_r1(&LC, &CMD, InData_IV, InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}

rsip_err_t r_rsip_wrapper_p6f_secp256r1_priv(const uint32_t InData_IV[],
                                            const uint32_t InData_InstData[],
                                            uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_ECC_SECP256R1_PRIVATE;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_ECC_SECP256R1_PRIVATE_KEY_INST_DATA_WORD;

    return r_rsip_p6f_r1(&LC, &CMD, InData_IV, InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}

rsip_err_t r_rsip_wrapper_p6f_hmacsha224(const uint32_t InData_IV[],
                                        const uint32_t InData_InstData[],
                                        uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_HMAC_SHA224;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_HMAC_SHA256_KEY_INST_DATA_WORD;

    return r_rsip_p6f_r1(&LC, &CMD, InData_IV, InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}

rsip_err_t r_rsip_wrapper_p6f_hmacsha256(const uint32_t InData_IV[],
                                        const uint32_t InData_InstData[],
                                        uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_HMAC_SHA256;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_HMAC_SHA256_KEY_INST_DATA_WORD;

    return r_rsip_p6f_r1(&LC, &CMD, InData_IV, InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}

rsip_err_t r_rsip_wrapper_p6f_brainpoolp256r1_pub(const uint32_t InData_IV[],
                                                const uint32_t InData_InstData[],
                                                uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_ECC_BRAINPOOLP256R1_PUBLIC;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_ECC_SECP256R1_PUBLIC_KEY_INST_DATA_WORD;

    return r_rsip_p6f_r1(&LC, &CMD, InData_IV, InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}

rsip_err_t r_rsip_wrapper_p6f_brainpoolp256r1_priv(const uint32_t InData_IV[],
                                                const uint32_t InData_InstData[],
                                                uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_ECC_BRAINPOOLP256R1_PRIVATE;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_ECC_SECP256R1_PRIVATE_KEY_INST_DATA_WORD;

    return r_rsip_p6f_r1(&LC, &CMD, InData_IV, InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}

rsip_err_t r_rsip_wrapper_p6f_secp256k1_pub(const uint32_t InData_IV[],
                                            const uint32_t InData_InstData[],
                                            uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_ECC_SECP256K1_PUBLIC;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_ECC_SECP256R1_PUBLIC_KEY_INST_DATA_WORD;

    return r_rsip_p6f_r1(&LC, &CMD, InData_IV, InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}

rsip_err_t r_rsip_wrapper_p6f_secp256k1_priv(const uint32_t InData_IV[],
                                            const uint32_t InData_InstData[],
                                            uint32_t       OutData_KeyIndex[])
{
    uint32_t CMD = RSIP_OEM_CMD_ECC_SECP256K1_PRIVATE;
    uint32_t LC  = RSIP_DUMMY_LC;
    INST_DATA_SIZE = RSIP_OEM_KEY_SIZE_ECC_SECP256R1_PRIVATE_KEY_INST_DATA_WORD;

    return r_rsip_p6f_r1(&LC, &CMD, InData_IV, InData_InstData, INST_DATA_SIZE, OutData_KeyIndex);
}



rsip_err_t r_rsip_wrapper_p8f_aes128(const uint32_t        InData_KeyIndex[],
                                    const rsip_key_type_t key_type,
                                    const uint32_t        InData_WrappedKeyIndex[],
                                    uint32_t              OutData_Text[])
{
    uint32_t InData_Cmd[1] = {change_endian_long(0)};
    uint32_t InData_WrappedKeyType[1];
    uint32_t key_index_size = RSIP_KEY_INDEX_SIZE_AES128_WORD;
    uint32_t wrappedkey_index_size = RSIP_WRAPPED_KEY_SIZE_AES128_WORD;

    rsip_err_t err = select_rfc3394_key_wrap_mode(key_type, InData_WrappedKeyType);
    if (RSIP_SUCCESS == err)
    {
        err = r_rsip_p8f_r1(InData_Cmd, InData_KeyIndex, InData_WrappedKeyType, InData_WrappedKeyIndex,
                            key_index_size, OutData_Text, wrappedkey_index_size);
    }

    return err;
}

rsip_err_t r_rsip_wrapper_p8f_aes256(const uint32_t        InData_KeyIndex[],
                                    const rsip_key_type_t key_type,
                                    const uint32_t        InData_WrappedKeyIndex[],
                                    uint32_t              OutData_Text[])
{
    uint32_t InData_Cmd[1] = {change_endian_long(1)};
    uint32_t InData_WrappedKeyType[1];
    uint32_t key_index_size = RSIP_KEY_INDEX_SIZE_AES256_WORD;
    uint32_t wrappedkey_index_size = RSIP_WRAPPED_KEY_SIZE_AES256_WORD;

    rsip_err_t err = select_rfc3394_key_wrap_mode(key_type, InData_WrappedKeyType);
    if (RSIP_SUCCESS == err)
    {
        err = r_rsip_p8f_r1(InData_Cmd, InData_KeyIndex, InData_WrappedKeyType, InData_WrappedKeyIndex,
                            key_index_size, OutData_Text, wrappedkey_index_size);
    }
    return err;
}

rsip_err_t r_rsip_wrapper_p90_aes128(const uint32_t        InData_KeyIndex[],
                                    const rsip_key_type_t key_type,
                                    const uint32_t        InData_Text[],
                                    uint32_t              OutData_KeyIndex[])
{
    uint32_t InData_Cmd[1] = {change_endian_long(0)};
    uint32_t InData_WrappedKeyType[1];
    uint32_t key_index_size = RSIP_KEY_INDEX_SIZE_AES128_WORD;
    uint32_t wrappedkey_index_size = RSIP_WRAPPED_KEY_SIZE_AES128_WORD;

    rsip_err_t err = select_rfc3394_key_wrap_mode(key_type, InData_WrappedKeyType);
    if (RSIP_SUCCESS == err)
    {
        err = r_rsip_p90_r1(InData_Cmd, InData_KeyIndex, InData_WrappedKeyType, InData_Text, key_index_size,
                            OutData_KeyIndex, wrappedkey_index_size);
    }

    return err;
}

rsip_err_t r_rsip_wrapper_p90_aes256(const uint32_t        InData_KeyIndex[],
                                    const rsip_key_type_t key_type,
                                    const uint32_t        InData_Text[],
                                    uint32_t              OutData_KeyIndex[])
{
    uint32_t InData_Cmd[1] = {change_endian_long(1)};
    uint32_t InData_WrappedKeyType[1];
    uint32_t key_index_size = RSIP_KEY_INDEX_SIZE_AES256_WORD;
    uint32_t wrappedkey_index_size = RSIP_WRAPPED_KEY_SIZE_AES256_WORD;

    rsip_err_t err = select_rfc3394_key_wrap_mode(key_type, InData_WrappedKeyType);
    if (RSIP_SUCCESS == err)
    {
        err = r_rsip_p90_r1(InData_Cmd, InData_KeyIndex, InData_WrappedKeyType, InData_Text, key_index_size,
                            OutData_KeyIndex, wrappedkey_index_size);
    }

    return err;
}

rsip_err_t r_rsip_wrapper_p47i_aes128ecb_encrypt(const uint32_t InData_KeyIndex[], const uint32_t InData_IV[])
{
    uint32_t cmd      = change_endian_long(gs_cmd_aes_mode[RSIP_AES_MODE_EXTENDED_ECB_ENCRYPT]);

    return r_rsip_p47i(&cmd, InData_KeyIndex, InData_IV);
}

rsip_err_t r_rsip_wrapper_p47i_aes128ecb_decrypt(const uint32_t InData_KeyIndex[], const uint32_t InData_IV[])
{
    uint32_t cmd      = change_endian_long(gs_cmd_aes_mode[RSIP_AES_MODE_EXTENDED_ECB_DECRYPT]);

    return r_rsip_p47i(&cmd, InData_KeyIndex, InData_IV);
}

rsip_err_t r_rsip_wrapper_p47i_aes128cbc_encrypt(const uint32_t InData_KeyIndex[], const uint32_t InData_IV[])
{
    uint32_t cmd      = change_endian_long(gs_cmd_aes_mode[RSIP_AES_MODE_EXTENDED_CBC_ENCRYPT]);

    return r_rsip_p47i(&cmd, InData_KeyIndex, InData_IV);
}

rsip_err_t r_rsip_wrapper_p47i_aes128cbc_decrypt(const uint32_t InData_KeyIndex[], const uint32_t InData_IV[])
{
    uint32_t cmd      = change_endian_long(gs_cmd_aes_mode[RSIP_AES_MODE_EXTENDED_CBC_DECRYPT]);

    return r_rsip_p47i(&cmd, InData_KeyIndex, InData_IV);
}

rsip_err_t r_rsip_wrapper_p47i_aes128ctr_crypt(const uint32_t InData_KeyIndex[], const uint32_t InData_IV[])
{
    uint32_t cmd      = change_endian_long(gs_cmd_aes_mode[RSIP_AES_MODE_EXTENDED_CTR_CRYPT]);

    return r_rsip_p47i(&cmd, InData_KeyIndex, InData_IV);
}

rsip_err_t r_rsip_wrapper_p50i_aes256ecb_encrypt(const uint32_t InData_KeyIndex[], const uint32_t InData_IV[])
{;
    uint32_t cmd      = change_endian_long(gs_cmd_aes_mode[RSIP_AES_MODE_EXTENDED_ECB_ENCRYPT]);

    return r_rsip_p50i(&cmd, InData_KeyIndex, InData_IV);
}

rsip_err_t r_rsip_wrapper_p50i_aes256ecb_decrypt(const uint32_t InData_KeyIndex[], const uint32_t InData_IV[])
{
    uint32_t cmd      = change_endian_long(gs_cmd_aes_mode[RSIP_AES_MODE_EXTENDED_ECB_DECRYPT]);

    return r_rsip_p50i(&cmd, InData_KeyIndex, InData_IV);
}

rsip_err_t r_rsip_wrapper_p50i_aes256cbc_encrypt(const uint32_t InData_KeyIndex[], const uint32_t InData_IV[])
{
    uint32_t cmd      = change_endian_long(gs_cmd_aes_mode[RSIP_AES_MODE_EXTENDED_CBC_ENCRYPT]);

    return r_rsip_p50i(&cmd, InData_KeyIndex, InData_IV);
}

rsip_err_t r_rsip_wrapper_p50i_aes256cbc_decrypt(const uint32_t InData_KeyIndex[], const uint32_t InData_IV[])
{
    uint32_t cmd      = change_endian_long(gs_cmd_aes_mode[RSIP_AES_MODE_EXTENDED_CBC_DECRYPT]);

    return r_rsip_p50i(&cmd, InData_KeyIndex, InData_IV);
}

rsip_err_t r_rsip_wrapper_p50i_aes256ctr_crypt(const uint32_t InData_KeyIndex[], const uint32_t InData_IV[])
{
    uint32_t cmd      = change_endian_long(gs_cmd_aes_mode[RSIP_AES_MODE_EXTENDED_CTR_CRYPT]);

    return r_rsip_p50i(&cmd, InData_KeyIndex, InData_IV);
}

rsip_err_t r_rsip_wrapper_p29i_aes128gcm_encrypt(const uint32_t * InData_KeyIndex, const uint32_t * InData_IV)
{
    return r_rsip_p29i(InData_KeyIndex, InData_IV);
}

rsip_err_t r_rsip_wrapper_p32i_aes128gcm_decrypt(const uint32_t * InData_KeyIndex, const uint32_t * InData_IV)
{
    return r_rsip_p32i(InData_KeyIndex, InData_IV);
}

rsip_err_t r_rsip_wrapper_p34i_aes256gcm_encrypt(const uint32_t * InData_KeyIndex, const uint32_t * InData_IV)
{
    return r_rsip_p34i(InData_KeyIndex, InData_IV);
}

rsip_err_t r_rsip_wrapper_p36i_aes256gcm_decrypt(const uint32_t * InData_KeyIndex, const uint32_t * InData_IV)
{
    return r_rsip_p36i(InData_KeyIndex, InData_IV);
}

rsip_err_t r_rsip_wrapper_p95i_aes128ccm_encrypt(const uint32_t * InData_KeyIndex,
                                                const uint32_t * InData_TextLen,
                                                const uint32_t * InData_IV,
                                                const uint32_t * InData_Header,
                                                uint32_t         Header_Len)
{
    (void) InData_TextLen;
    return r_rsip_p95i(InData_KeyIndex,
                        InData_IV,
                        InData_Header,
                        Header_Len);
}

rsip_err_t r_rsip_wrapper_p95f_aes128ccm_encrypt(const uint32_t    * InData_Text,
                                                    const uint32_t * InData_TextLen,
                                                    uint32_t       * OutData_Text,
                                                    uint32_t       * OutData_MAC)
{


    return r_rsip_p95f(InData_Text, InData_TextLen, OutData_Text, OutData_MAC);
}

rsip_err_t r_rsip_wrapper_p98i_aes128ccm_decrypt(const uint32_t    * InData_KeyIndex,
                                                    const uint32_t * InData_TextLen,
                                                    const uint32_t * InData_MACLength,
                                                    const uint32_t * InData_IV,
                                                    const uint32_t * InData_Header,
                                                    uint32_t         Header_Len)
{
    (void) InData_TextLen;
    (void) InData_MACLength;
    return r_rsip_p98i(InData_KeyIndex,
                        InData_IV,
                        InData_Header,
                        Header_Len);
}

rsip_err_t r_rsip_wrapper_p98f_aes128ccm_decrypt(const uint32_t    * InData_Text,
                                                    const uint32_t * InData_TextLen,
                                                    const uint32_t * InData_MAC,
                                                    const uint32_t * InData_MACLength,
                                                    uint32_t       * OutData_Text)
{
    return r_rsip_p98f(InData_Text, InData_TextLen, InData_MAC, InData_MACLength, OutData_Text);
}

rsip_err_t r_rsip_wrapper_pa1i_aes256ccm_encrypt(const uint32_t    * InData_KeyIndex,
                                                    const uint32_t * InData_TextLen,
                                                    const uint32_t * InData_IV,
                                                    const uint32_t * InData_Header,
                                                    uint32_t         Header_Len)
{
    (void) InData_TextLen;

    return r_rsip_pa1i(InData_KeyIndex, InData_IV, InData_Header, Header_Len);
}

rsip_err_t r_rsip_wrapper_pa4i_aes256ccm_decrypt(const uint32_t   * InData_KeyIndex,
                                                    const uint32_t * InData_TextLen,
                                                    const uint32_t * InData_MACLength,
                                                    const uint32_t * InData_IV,
                                                    const uint32_t * InData_Header,
                                                    uint32_t         Header_Len)
{
    (void) InData_TextLen;
    (void) InData_MACLength;

    return r_rsip_pa4i(InData_KeyIndex, InData_IV, InData_Header, Header_Len);
}


rsip_err_t r_rsip_wrapper_p41i_aes128mac(const uint32_t * InData_KeyIndex)
{
    return r_rsip_p41i(InData_KeyIndex);
}

rsip_err_t r_rsip_wrapper_p41f_aes128mac_generate(const uint32_t   * InData_Text,
                                                    uint32_t       * OutData_DataT,
                                                    const uint32_t   all_msg_len)
{
    uint32_t InData_Cmd[1] =
    {
        ((0 == (all_msg_len % 16)) &&
        (0 !=
        all_msg_len)) ? RSIP_PRV_CMD_AES_CMAC_GENERATE_WITHOUT_REMAINDER :
        RSIP_PRV_CMD_AES_CMAC_GENERATE_WITH_REMAINDER
    };
    uint32_t InData_DataT = 0;

    return r_rsip_p41f(InData_Cmd, InData_Text, &InData_DataT, (uint32_t *) &all_msg_len, OutData_DataT);
}

rsip_err_t r_rsip_wrapper_p41f_aes128mac_verify(const uint32_t * InData_Text,
                                                const uint32_t * InData_DataT,
                                                const uint32_t * InData_DataTLen,
                                                const uint32_t   all_msg_len)
{
    uint32_t InData_Cmd[1] =
    {
        ((0 == (all_msg_len % 16)) &&
        (0 !=
        all_msg_len)) ? RSIP_PRV_CMD_AES_CMAC_VERIFY_WITHOUT_REMAINDER :
        RSIP_PRV_CMD_AES_CMAC_VERIFY_WITH_REMAINDER
    };
    uint32_t OutData_DataT[4] = {0};
    (void) all_msg_len;

    return r_rsip_p41f(InData_Cmd, InData_Text, InData_DataT, InData_DataTLen, OutData_DataT);
}

rsip_err_t r_rsip_wrapper_p44i_aes256mac(const uint32_t * InData_KeyIndex)
{
    return r_rsip_p44i(InData_KeyIndex);
}

rsip_err_t r_rsip_wrapper_p44f_aes256mac_generate(const uint32_t   * InData_Text,
                                                    uint32_t       * OutData_DataT,
                                                    const uint32_t   all_msg_len)
{
    uint32_t InData_Cmd[1] =
    {
        ((0 == (all_msg_len % 16)) &&
        (0 !=
        all_msg_len)) ? RSIP_PRV_CMD_AES_CMAC_GENERATE_WITHOUT_REMAINDER :
        RSIP_PRV_CMD_AES_CMAC_GENERATE_WITH_REMAINDER
    };
    uint32_t InData_DataT = 0;

    return r_rsip_p44f(InData_Cmd, InData_Text, &InData_DataT, (uint32_t *) &all_msg_len, OutData_DataT);
}

rsip_err_t r_rsip_wrapper_p44f_aes256mac_verify(const uint32_t * InData_Text,
                                                const uint32_t * InData_DataT,
                                                const uint32_t * InData_DataTLen,
                                                const uint32_t   all_msg_len)
{
    uint32_t InData_Cmd[1] =
    {
        ((0 == (all_msg_len % 16)) &&
        (0 !=
        all_msg_len)) ? RSIP_PRV_CMD_AES_CMAC_VERIFY_WITHOUT_REMAINDER :
        RSIP_PRV_CMD_AES_CMAC_VERIFY_WITH_REMAINDER
    };
    uint32_t OutData_DataT[4] = {0};
    (void) all_msg_len;

    return r_rsip_p44f(InData_Cmd, InData_Text, InData_DataT, InData_DataTLen, OutData_DataT);
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

static rsip_err_t select_rfc3394_key_wrap_mode(const rsip_key_type_t key_type,
                                                uint32_t              WrappedKeyType[])
{
    rsip_alg_t alg     = r_rsip_key_type_to_alg(key_type);
    uint32_t   subtype = r_rsip_key_type_to_subtype(key_type);

    rsip_err_t err = RSIP_SUCCESS;
    switch (alg)
    {
        case RSIP_ALG_AES:
        {
            switch (subtype)
            {
                case RSIP_KEY_AES_128:
                {
                    WrappedKeyType[0] = change_endian_long(RSIP_RFC3394_KEY_WRAP_TYPE_AES128);
                    KEY_INDEX_SIZE    = r_rsip_byte_to_word_convert(RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_AES_128);
                    WRAPPED_KEY_SIZE  = RSIP_PRV_WORD_SIZE_RFC3394_WRAPPED_KEY_AES_128;
                    break;
                }

                case RSIP_KEY_AES_256:
                {
                    WrappedKeyType[0] = change_endian_long(RSIP_RFC3394_KEY_WRAP_TYPE_AES256);
                    KEY_INDEX_SIZE    = r_rsip_byte_to_word_convert(RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_AES_256);
                    WRAPPED_KEY_SIZE  = RSIP_PRV_WORD_SIZE_RFC3394_WRAPPED_KEY_AES_256;
                    break;
                }

                default:
                {
                    err = RSIP_ERR_KEY_SET;
                }
            }
            break;
        }

        default:
        {
            err = RSIP_ERR_KEY_SET;
        }
    }

    return err;
}
