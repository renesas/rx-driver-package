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
 * Copyright (C) 2018-2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_tsip_arc4_rx.c
 * Version      : 1.18
 * Description  : Interface definition for the r_tsip_tdes_rx TSIP module.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 30.04.2018 1.05     First Release
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
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <string.h>
#include "r_tsip_rx_private.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
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
#if TSIP_ARC4_ENCRYPT == 1
uint32_t g_arc4_enc_private_id;
#endif
#if TSIP_ARC4_DECRYPT == 1
uint32_t g_arc4_dec_private_id;
#endif

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/

#if TSIP_PRV_USE_ARC4
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateArc4KeyIndex
*******************************************************************************************************************/ /**
* @details       The API for outputting User Key Generation Information of Arc4.
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information (73 words) of ARC4.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateArc4KeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateArc4KeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv, uint8_t *encrypted_key,
        tsip_arc4_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateArc4KeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ARC4;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateArc4KeyIndex
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateArc4KeyIndex
*******************************************************************************************************************/ /**
* @details       The API for updating Arc4 user key index
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information (73 words) of ARC4.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateArc4KeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateArc4KeyIndex(uint8_t *iv, uint8_t *encrypted_key, tsip_arc4_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_UpdateArc4KeyIndexSub((uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ARC4;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/****************************************
 End of function R_TSIP_UpdateArc4KeyIndex
 ****************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateArc4RandomKeyIndex
*******************************************************************************************************************/ /**
* @details       The API for outputting random User Key Generation Information of ARC4.
* @param[out]    key_index Output the user Key Generation Information (73 words) of ARC4.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @see           R_TSIP_GenerateArc4RandomKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateArc4RandomKeyIndex(tsip_arc4_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    error_code = R_TSIP_GenerateArc4RandomKeyIndexSub(key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ARC4;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/***************************************
 End of function R_TSIP_GenerateArc4RandomKeyIndex
 ***************************************/
#endif /* TSIP_PRV_USE_ARC4 */

#if TSIP_ARC4_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Arc4EncryptInit
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using ARC4 algorithm.
* @param[out]    handle Handler for ARC4(work area).
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Arc4EncryptInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Arc4EncryptInit(tsip_arc4_handle_t *handle, tsip_arc4_key_index_t *key_index)
{
    memset(handle, 0, sizeof(tsip_arc4_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_arc4_enc_private_id = g_private_id_counter;
    handle->id = g_arc4_enc_private_id;
    return R_TSIP_Arc4EncryptInitPrivate(key_index);
}
/*******************************
 End of function R_TSIP_Arc4EncryptInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Arc4EncryptUpdate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using ARC4 algorithm.
* @param[in,out] handle Handler for ARC4(work area).
* @param[in]     plain Input plain text.
* @param[out]    cipher Output cipher text.
* @param[in]     plain_length Input byte size of plain.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Arc4EncryptUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Arc4EncryptUpdate(tsip_arc4_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_length)
{
    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_arc4_enc_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    /* Process of fractions using last_1_block_as_fraction is not implemented. */
    /* Casting uint32_t pointer is used for address. */
    return R_TSIP_Arc4EncryptUpdatePrivate((uint32_t*)plain, (uint32_t*)cipher, plain_length >> 2);
}
/*******************************
 End of function R_TSIP_Arc4EncryptUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Arc4EncryptFinal
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using ARC4 algorithm.
* @param[in]     handle Handler for ARC4(work area).
* @param[out]    cipher Output cipher text.
* @param[out]    cipher_length Output byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Arc4EncryptFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Arc4EncryptFinal(tsip_arc4_handle_t *handle, uint8_t *cipher, uint32_t *cipher_length)
{
    e_tsip_err_t error_code;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_arc4_enc_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    *cipher_length = 0;
    error_code = R_TSIP_Arc4EncryptFinalPrivate();
    memset(handle, 0, sizeof(tsip_arc4_handle_t));
    return error_code;
}
/*******************************
 End of function R_TSIP_Arc4EncryptFinal
 *******************************/
#endif  /* TSIP_ARC4_ENCRYPT == 1 */

#if TSIP_ARC4_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Arc4DecryptInit
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using ARC4 algorithm.
* @param[out]    handle Handler for ARC4(work area).
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Arc4DecryptInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Arc4DecryptInit(tsip_arc4_handle_t *handle, tsip_arc4_key_index_t *key_index)
{
    memset(handle, 0, sizeof(tsip_arc4_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_arc4_dec_private_id = g_private_id_counter;
    handle->id = g_arc4_dec_private_id;
    return R_TSIP_Arc4DecryptInitPrivate(key_index);
}
/*******************************
 End of function R_TSIP_Arc4DecryptInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Arc4DecryptUpdate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using ARC4 algorithm.
* @param[in,out] handle Handler for ARC4(work area).
* @param[in]     cipher Input cipher text.
* @param[out]    plain Output plain text.
* @param[in]     cipher_length Input byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Arc4DecryptUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Arc4DecryptUpdate(tsip_arc4_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_length)
{
    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_arc4_dec_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    /* Process of fractions using last_1_block_as_fraction is not implemented. */
    /* Casting uint32_t pointer is used for address. */
    return R_TSIP_Arc4DecryptUpdatePrivate((uint32_t*)cipher, (uint32_t*)plain, cipher_length >> 2);
}
/*******************************
 End of function R_TSIP_Arc4DecryptUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Arc4DecryptFinal
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using ARC4 algorithm.
* @param[in]     handle Handler for ARC4(work area).
* @param[out]    plain Output plain text.
* @param[out]    plain_length Output byte size of plain.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Arc4DecryptFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Arc4DecryptFinal(tsip_arc4_handle_t *handle, uint8_t *plain, uint32_t *plain_length)
{
    e_tsip_err_t error_code;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_arc4_dec_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    *plain_length = 0;
    error_code = R_TSIP_Arc4DecryptFinalPrivate();
    memset(handle, 0, sizeof(tsip_arc4_handle_t));
    return error_code;
}
/*******************************
 End of function R_TSIP_Arc4DecryptFinal
 *******************************/
#endif  /* TSIP_ARC4_DECRYPT == 1 */

