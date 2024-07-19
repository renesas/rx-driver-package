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
 * File Name    : r_tsip_hash_rx.c
 * Version      : 1.20
 * Description  : Interface definition for the r_tsip_hash_rx TSIP module.
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
/* Block length (in bytes) of SHA */
#define SHA_BLOCK8_LEN          (64u)
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
#if TSIP_SHA_1_HMAC == 1
uint32_t g_sha1hmacgen_private_id;
uint32_t g_sha1hmacver_private_id;
#endif
#if TSIP_SHA_256_HMAC == 1
uint32_t g_sha256hmacgen_private_id;
uint32_t g_sha256hmacver_private_id;
#endif

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/

#if TSIP_MD5 == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Md5Init
*******************************************************************************************************************/ /**
* @details       prepares to execute the MD5 hash operation
* @param[out]    handle MD5 handler (work area)
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Md5InitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Md5Init(tsip_sha_md5_handle_t *handle)
{
    memset(handle, 0, sizeof(tsip_sha_md5_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    handle->hash_type = R_TSIP_RSA_HASH_MD5;
    return R_TSIP_Md5InitPrivate(handle);
}
/*******************************
 End of function R_TSIP_Md5Init
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Md5Update
*******************************************************************************************************************/ /**
* @details       calculate a hash value
* @param[in,out] handle MD5 handler (work area).
* @param[in]     message Input message.
* @param[in]     message_length Input message length
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Md5UpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Md5Update(tsip_sha_md5_handle_t *handle, uint8_t *message, uint32_t message_length)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t length_rest = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (R_TSIP_RSA_HASH_MD5 != handle->hash_type)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->all_received_length += message_length;
    if ((handle->buffering_length + message_length) >= SHA_BLOCK8_LEN)
    {
        memcpy((handle->sha_buffer + handle->buffering_length), message, SHA_BLOCK8_LEN - handle->buffering_length);
        /* Casting uint32_t pointer is used for address. */
        ercd = R_TSIP_Md5UpdatePrivate(handle, (uint32_t*)(handle->sha_buffer), SHA_BLOCK8_LEN >> 2);
        length_rest = message_length - (SHA_BLOCK8_LEN - handle->buffering_length);
        memset(handle->sha_buffer, 0, sizeof(handle->sha_buffer));
        if (length_rest >= SHA_BLOCK8_LEN)
        {
            /* Casting uint32_t pointer is used for address. */
            ercd = R_TSIP_Md5UpdatePrivate(handle, (uint32_t*)(message + (SHA_BLOCK8_LEN - handle->buffering_length)),
                    ((length_rest / SHA_BLOCK8_LEN) * SHA_BLOCK8_LEN) >> 2);
            length_rest -= ((length_rest / SHA_BLOCK8_LEN) * SHA_BLOCK8_LEN);
        }
        handle->buffering_length = 0;
        memcpy(handle->sha_buffer, message + (message_length - length_rest), length_rest);
        handle->buffering_length = length_rest;
    }
    else
    {
        memcpy(handle->sha_buffer + handle->buffering_length, message, message_length);
        handle->buffering_length += message_length;
    }
    return ercd;
}
/*******************************
 End of function R_TSIP_Md5Update
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Md5Final
*******************************************************************************************************************/ /**
* @details       calculate a hash value
* @param[in]     handle MD5 handler (work area).
* @param[out]    digest Output Hash data
* @param[out]    digest_length Output Hash length
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Md5FinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Md5Final(tsip_sha_md5_handle_t *handle, uint8_t *digest, uint32_t *digest_length)
{
    uint32_t max_cnt_byte = 0;
    e_tsip_err_t ercd = TSIP_SUCCESS;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (R_TSIP_RSA_HASH_MD5 != handle->hash_type)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->sha_buffer[handle->buffering_length] = 0x80;
    if ((SHA_BLOCK8_LEN - handle->buffering_length) >= 9) /* another block unnecessary */
    {
        memset(handle->sha_buffer + (handle->buffering_length + 1), 0,
                (SHA_BLOCK8_LEN - 4) - (handle->buffering_length + 1));
        /* According c., but support only l < 2**32 case */
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[SHA_BLOCK8_LEN - 5] = (uint8_t) (((handle->all_received_length * 8) >> 24) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[SHA_BLOCK8_LEN - 6] = (uint8_t) (((handle->all_received_length * 8) >> 16) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[SHA_BLOCK8_LEN - 7] = (uint8_t) (((handle->all_received_length * 8) >> 8) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[SHA_BLOCK8_LEN - 8] = (uint8_t) ((handle->all_received_length * 8) & 0xff);
        max_cnt_byte = SHA_BLOCK8_LEN;
    }
    else /* another block necessary */
    {
        memset(handle->sha_buffer + (handle->buffering_length + 1), 0,
                ((2 * SHA_BLOCK8_LEN) - 4) - (handle->buffering_length + 1));
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[(2 * SHA_BLOCK8_LEN) - 5] = (uint8_t) ((handle->all_received_length * 8) >> 24);
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[(2 * SHA_BLOCK8_LEN) - 6] = (uint8_t) (((handle->all_received_length * 8) >> 16) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[(2 * SHA_BLOCK8_LEN) - 7] = (uint8_t) (((handle->all_received_length * 8) >> 8) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[(2 * SHA_BLOCK8_LEN) - 8] = (uint8_t) ((handle->all_received_length * 8) & 0xff);
        max_cnt_byte = 2 * SHA_BLOCK8_LEN;
    }
    /* Casting uint32_t pointer is used for address. */
    ercd = R_TSIP_Md5FinalPrivate(handle, max_cnt_byte >> 2, (uint32_t*)digest, digest_length);
    handle->all_received_length = 0;
    handle->buffering_length = 0;
    memset(handle->sha_buffer, 0, sizeof(handle->sha_buffer));
    return ercd;
}
/*******************************
 End of function R_TSIP_Md5Final
 *******************************/
#endif  /* TSIP_MD5 == 1 */

#if TSIP_SHA_1 == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Sha1Init
*******************************************************************************************************************/ /**
* @details       prepares to execute the SHA1 hash operation
* @param[out]    handle ySHA1 handler (work area)
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Sha1InitPrivate()
* @note          None
*/
TSIP_SEC_P_SECURE_BOOT
e_tsip_err_t R_TSIP_Sha1Init(tsip_sha_md5_handle_t *handle)
{
    memset(handle, 0, sizeof(tsip_sha_md5_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    handle->hash_type = R_TSIP_RSA_HASH_SHA1;
    return R_TSIP_Sha1InitPrivate(handle);
}
/*******************************
 End of function R_TSIP_Sha1Init
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha1Update
*******************************************************************************************************************/ /**
* @details       execute the SHA1 hash operation
* @param[in,out] handle SHA1 handler (work area).
* @param[in]     message Input message.
* @param[in]     message_length Input message length
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Sha1UpdatePrivate()
* @note          None
*/
TSIP_SEC_P_SECURE_BOOT
e_tsip_err_t R_TSIP_Sha1Update(tsip_sha_md5_handle_t *handle, uint8_t *message, uint32_t message_length)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t length_rest = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (R_TSIP_RSA_HASH_SHA1 != handle->hash_type)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->all_received_length += message_length;
    if ((handle->buffering_length + message_length) >= SHA_BLOCK8_LEN)
    {
        memcpy(handle->sha_buffer + handle->buffering_length, message, SHA_BLOCK8_LEN - handle->buffering_length);

        /*Casting uint32_t pointer is used for address.*/
        ercd = R_TSIP_Sha1UpdatePrivate(handle, (uint32_t *) (handle->sha_buffer),
        SHA_BLOCK8_LEN >> 2);
        length_rest = message_length - (SHA_BLOCK8_LEN - handle->buffering_length);
        memset(handle->sha_buffer, 0, sizeof(handle->sha_buffer));
        if (length_rest >= SHA_BLOCK8_LEN)
        {
            ercd = R_TSIP_Sha1UpdatePrivate(handle,
                    /* Casting uint32_t pointer is used for address. */
                    (uint32_t *) (message + (SHA_BLOCK8_LEN - handle->buffering_length)),
                    ((length_rest / SHA_BLOCK8_LEN) * SHA_BLOCK8_LEN) >> 2);
            length_rest -= ((length_rest / SHA_BLOCK8_LEN) * SHA_BLOCK8_LEN);
        }
        handle->buffering_length = 0;
        memcpy(handle->sha_buffer, message + (message_length - length_rest), length_rest);
        handle->buffering_length = length_rest;
    }
    else
    {
        memcpy(handle->sha_buffer + handle->buffering_length, message, message_length);
        handle->buffering_length += message_length;
    }
    return ercd;
}
/*******************************
 End of function R_TSIP_Sha1Update
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha1Final
*******************************************************************************************************************/ /**
* @details       calculate a hash value
* @param[in]     handle SHA1 handler (work area).
* @param[out]    digest Output Hash data
* @param[out]    digest_length Output Hash length
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Sha1FinalPrivate()
* @note          None
*/
TSIP_SEC_P_SECURE_BOOT
e_tsip_err_t R_TSIP_Sha1Final(tsip_sha_md5_handle_t *handle, uint8_t *digest, uint32_t *digest_length)
{
    uint32_t max_cnt_byte = 0;
    e_tsip_err_t ercd = TSIP_SUCCESS;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (R_TSIP_RSA_HASH_SHA1 != handle->hash_type)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->sha_buffer[handle->buffering_length] = 0x80;
    if ((SHA_BLOCK8_LEN - handle->buffering_length) >= 9) /* another block unnecessary */
    {
        memset(handle->sha_buffer + (handle->buffering_length + 1), 0,
                (SHA_BLOCK8_LEN - 4) - (handle->buffering_length + 1));

        /* According c., but support only l < 2**32 case */
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[SHA_BLOCK8_LEN - 4] = (uint8_t) ((handle->all_received_length * 8) >> 24);
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[SHA_BLOCK8_LEN - 3] = (uint8_t) (((handle->all_received_length * 8) >> 16) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[SHA_BLOCK8_LEN - 2] = (uint8_t) (((handle->all_received_length * 8) >> 8) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[SHA_BLOCK8_LEN - 1] = (uint8_t) ((handle->all_received_length * 8) & 0xff);
        max_cnt_byte = SHA_BLOCK8_LEN;
    }
    else
    {
        /* another block necessary */
        memset(handle->sha_buffer + (handle->buffering_length + 1), 0,
                ((2 * SHA_BLOCK8_LEN) - 4) - (handle->buffering_length + 1));

        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[(2 * SHA_BLOCK8_LEN) - 4] = (uint8_t) ((handle->all_received_length * 8) >> 24);
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[(2 * SHA_BLOCK8_LEN) - 3] = (uint8_t) (((handle->all_received_length * 8) >> 16) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[(2 * SHA_BLOCK8_LEN) - 2] = (uint8_t) (((handle->all_received_length * 8) >> 8) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[(2 * SHA_BLOCK8_LEN) - 1] = (uint8_t) ((handle->all_received_length * 8) & 0xff);
        max_cnt_byte = 2 * SHA_BLOCK8_LEN;
    }

    /* Casting uint32_t pointer is used for address. */
    ercd = R_TSIP_Sha1FinalPrivate(handle, max_cnt_byte >> 2, (uint32_t*)digest, digest_length);
    handle->all_received_length = 0;
    handle->buffering_length = 0;
    memset(handle->sha_buffer, 0, sizeof(handle->sha_buffer));
    return ercd;
}
/*******************************
 End of function R_TSIP_Sha1Final
 *******************************/
#endif /* TSIP_SHA_1 == 1 */
TSIP_SEC_DEFAULT

#if TSIP_SHA_256 == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Sha256Init
*******************************************************************************************************************/ /**
* @details       prepares to execute the SHA256 hash operation
* @param[out]    handle ySHA1 handler (work area)
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Sha256InitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha256Init(tsip_sha_md5_handle_t *handle)
{
    memset(handle, 0, sizeof(tsip_sha_md5_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    handle->hash_type = R_TSIP_RSA_HASH_SHA256;
    return R_TSIP_Sha256InitPrivate(handle);
}
/*******************************
 End of function R_TSIP_Sha256Init
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha256Update
*******************************************************************************************************************/ /**
* @details       execute the SHA256 hash operation
* @param[in,out] handle SHA256 handler (work area).
* @param[in]     message Input message.
* @param[in]     message_length Input message length
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Sha256UpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha256Update(tsip_sha_md5_handle_t *handle, uint8_t *message, uint32_t message_length)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t length_rest = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (R_TSIP_RSA_HASH_SHA256 != handle->hash_type)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->all_received_length += message_length;
    if ((handle->buffering_length + message_length) >= SHA_BLOCK8_LEN)
    {
        memcpy(handle->sha_buffer + handle->buffering_length, message, SHA_BLOCK8_LEN - handle->buffering_length);

        /*Casting uint32_t pointer is used for address.*/
        ercd = R_TSIP_Sha256UpdatePrivate(handle, (uint32_t *) (handle->sha_buffer),
        SHA_BLOCK8_LEN >> 2);
        length_rest = message_length - (SHA_BLOCK8_LEN - handle->buffering_length);
        memset(handle->sha_buffer, 0, sizeof(handle->sha_buffer));
        if (length_rest >= SHA_BLOCK8_LEN)
        {
            ercd = R_TSIP_Sha256UpdatePrivate(handle,
                    /* Casting uint32_t pointer is used for address. */
                    (uint32_t *) (message + (SHA_BLOCK8_LEN - handle->buffering_length)),
                    ((length_rest / SHA_BLOCK8_LEN) * SHA_BLOCK8_LEN) >> 2);
            length_rest -= ((length_rest / SHA_BLOCK8_LEN) * SHA_BLOCK8_LEN);
        }
        handle->buffering_length = 0;
        memcpy(handle->sha_buffer, message + (message_length - length_rest), length_rest);
        handle->buffering_length = length_rest;
    }
    else
    {
        memcpy(handle->sha_buffer + handle->buffering_length, message, message_length);
        handle->buffering_length += message_length;
    }
    return ercd;
}
/*******************************
 End of function R_TSIP_Sha256Update
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha256Final
*******************************************************************************************************************/ /**
* @details       calculate a hash value
* @param[in]     handle SHA256 handler (work area).
* @param[out]    digest Output Hash data
* @param[out]    digest_length Output Hash length
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Sha256FinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha256Final(tsip_sha_md5_handle_t *handle, uint8_t *digest, uint32_t *digest_length)
{
    uint32_t max_cnt_byte = 0;
    e_tsip_err_t ercd = TSIP_SUCCESS;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (R_TSIP_RSA_HASH_SHA256 != handle->hash_type)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->sha_buffer[handle->buffering_length] = 0x80;
    if ((SHA_BLOCK8_LEN - handle->buffering_length) >= 9) /* another block unnecessary */
    {
        memset(handle->sha_buffer + (handle->buffering_length + 1), 0,
                (SHA_BLOCK8_LEN - 4) - (handle->buffering_length + 1));

        /* According c., but support only l < 2**32 case */
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[SHA_BLOCK8_LEN - 4] = (uint8_t) ((handle->all_received_length * 8) >> 24);
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[SHA_BLOCK8_LEN - 3] = (uint8_t) (((handle->all_received_length * 8) >> 16) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[SHA_BLOCK8_LEN - 2] = (uint8_t) (((handle->all_received_length * 8) >> 8) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[SHA_BLOCK8_LEN - 1] = (uint8_t) ((handle->all_received_length * 8) & 0xff);
        max_cnt_byte = SHA_BLOCK8_LEN;
    }
    else
    {
        /* another block necessary */
        memset(handle->sha_buffer + (handle->buffering_length + 1), 0,
                ((2 * SHA_BLOCK8_LEN) - 4) - (handle->buffering_length + 1));

        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[(2 * SHA_BLOCK8_LEN) - 4] = (uint8_t) ((handle->all_received_length * 8) >> 24);
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[(2 * SHA_BLOCK8_LEN) - 3] = (uint8_t) (((handle->all_received_length * 8) >> 16) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[(2 * SHA_BLOCK8_LEN) - 2] = (uint8_t) (((handle->all_received_length * 8) >> 8) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->sha_buffer[(2 * SHA_BLOCK8_LEN) - 1] = (uint8_t) ((handle->all_received_length * 8) & 0xff);
        max_cnt_byte = 2 * SHA_BLOCK8_LEN;
    }

    /* Casting uint32_t pointer is used for address. */
    ercd = R_TSIP_Sha256FinalPrivate(handle, max_cnt_byte >> 2, (uint32_t*)digest, digest_length);
    handle->all_received_length = 0;
    handle->buffering_length = 0;
    memset(handle->sha_buffer, 0, sizeof(handle->sha_buffer));
    return ercd;
}
/*******************************
 End of function R_TSIP_Sha256Final
 *******************************/
#endif /* TSIP_SHA_256 == 1 */

#if (TSIP_MD5 | TSIP_SHA_1 | TSIP_SHA_256) != 0
/***********************************************************************************************************************
* Function Name: R_TSIP_GetCurrentHashDigestValue
*******************************************************************************************************************/ /**
* @details       Get hash value calculation progress data
* @param[in]     handle SHA,MD5 handler (work area).
* @param[out]    digest Output Hash data
* @param[out]    digest_length Output Hash length(bytes)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Md5FinalPrivate()
* @see           R_TSIP_Sha1FinalPrivate()
* @see           R_TSIP_Sha256FinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_GetCurrentHashDigestValue(tsip_sha_md5_handle_t *handle, uint8_t *digest, uint32_t *digest_length)
{
    uint32_t              max_cnt_byte = 0;
    tsip_sha_md5_handle_t tmp_handle;
    e_tsip_err_t          ercd = TSIP_SUCCESS;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (R_TSIP_RSA_HASH_SHA256 < handle->hash_type)
    {
        return TSIP_ERR_PARAMETER;
    }
    else
    {
        memcpy(&tmp_handle, handle, sizeof(tsip_sha_md5_handle_t));
        tmp_handle.sha_buffer[tmp_handle.buffering_length] = 0x80;

#if TSIP_MD5 == 1
        if (R_TSIP_RSA_HASH_MD5 == tmp_handle.hash_type)
        {
            if ((SHA_BLOCK8_LEN - tmp_handle.buffering_length) >= 9) /* another block unnecessary */
            {
                /* According c., but support only l < 2**32 case */
                max_cnt_byte = SHA_BLOCK8_LEN;
            }
            else /* another block necessary */
            {
                max_cnt_byte = 2 * SHA_BLOCK8_LEN;
            }
            memset(tmp_handle.sha_buffer + (tmp_handle.buffering_length + 1), 0,
                    (max_cnt_byte - 4) - (tmp_handle.buffering_length + 1));

            /* Casting uint32_t data to uint8_t data array. */
            tmp_handle.sha_buffer[max_cnt_byte - 5] = (uint8_t)((tmp_handle.all_received_length * 8) >> 24);

            /* Casting uint32_t data to uint8_t data array. */
            tmp_handle.sha_buffer[max_cnt_byte - 6] = (uint8_t)((tmp_handle.all_received_length * 8) >> 16);

            /* Casting uint32_t data to uint8_t data array. */
            tmp_handle.sha_buffer[max_cnt_byte - 7] = (uint8_t)((tmp_handle.all_received_length * 8) >> 8);

            /* Casting uint32_t data to uint8_t data array. */
            tmp_handle.sha_buffer[max_cnt_byte - 8] = (uint8_t)(tmp_handle.all_received_length * 8);

            /* Casting uint32_t pointer is used for address. */
            ercd = R_TSIP_Md5FinalPrivate(&tmp_handle, max_cnt_byte >> 2, (uint32_t*)digest, digest_length);
        }
#endif  /* TSIP_MD5 == 1 */
#if (TSIP_SHA_1 | TSIP_SHA_256) != 0
        if ((R_TSIP_RSA_HASH_SHA1 == tmp_handle.hash_type) || (R_TSIP_RSA_HASH_SHA256 == tmp_handle.hash_type))
        {
            if ((SHA_BLOCK8_LEN - tmp_handle.buffering_length) >= 9) /* another block unnecessary */
            {
                /* According c., but support only l < 2**32 case */
                max_cnt_byte = SHA_BLOCK8_LEN;
            }
            else /* another block necessary */
            {
                max_cnt_byte = 2 * SHA_BLOCK8_LEN;
            }
            memset(tmp_handle.sha_buffer + (tmp_handle.buffering_length + 1), 0,
                    (max_cnt_byte - 4) - (tmp_handle.buffering_length + 1));

            /* Casting uint32_t data to uint8_t data array. */
            tmp_handle.sha_buffer[max_cnt_byte - 4] = (uint8_t)((tmp_handle.all_received_length * 8) >> 24);

            /* Casting uint32_t data to uint8_t data array. */
            tmp_handle.sha_buffer[max_cnt_byte - 3] = (uint8_t)((tmp_handle.all_received_length * 8) >> 16);

            /* Casting uint32_t data to uint8_t data array. */
            tmp_handle.sha_buffer[max_cnt_byte - 2] = (uint8_t)((tmp_handle.all_received_length * 8) >> 8);

            /* Casting uint32_t data to uint8_t data array. */
            tmp_handle.sha_buffer[max_cnt_byte - 1] = (uint8_t)(tmp_handle.all_received_length * 8);

#if TSIP_SHA_1 == 1
            if (R_TSIP_RSA_HASH_SHA1 == tmp_handle.hash_type)
            {
                /* Casting uint32_t pointer is used for address. */
                ercd = R_TSIP_Sha1FinalPrivate(&tmp_handle, max_cnt_byte >> 2, (uint32_t*)digest, digest_length);
            }
#endif /* TSIP_SHA_1 == 1 */
#if TSIP_SHA_256 == 1
            if (R_TSIP_RSA_HASH_SHA256 == tmp_handle.hash_type)
            {
                /* Casting uint32_t pointer is used for address. */
                ercd = R_TSIP_Sha256FinalPrivate(&tmp_handle, max_cnt_byte >> 2, (uint32_t*)digest, digest_length);
            }
#endif /* TSIP_SHA_256 == 1 */
        }
#endif /* TSIP_SHA_1 | TSIP_SHA_256 != 0 */
    }

    return ercd;
}
/*******************************
 End of function R_TSIP_GetCurrentHashDigestValue
 *******************************/
#endif /* TSIP_MD5 | TSIP_SHA_1 | TSIP_SHA_256 != 0 */

#if TSIP_SHA_1_HMAC
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateSha1HmacKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate SHA-1 HMAC user key index.
* @param[in]     encrypted_provisioning_key Provisioning key wrapped in DLM.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key SHA-1 HMAC key encrypted and MAC.
* @param[out]    key_index SHA-1 HMAC key user key generation information (17 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateShaHmacKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateSha1HmacKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_hmac_sha_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;

    indata_cmd = change_endian_long(0); /* SHA-1 */
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_GenerateShaHmacKeyIndexSub(&install_key_ring_index, (uint32_t*)encrypted_provisioning_key,
        /* Casting uint32_t pointer is used for address. */
        &indata_cmd, (uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_HMAC_SHA1;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateSha1HmacKeyIndex
 *******************************/
#endif /* TSIP_SHA_1_HMAC */

#if TSIP_SHA_256_HMAC
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateSha256HmacKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate SHA-256 HMAC user key index.
* @param[in]     encrypted_provisioning_key Provisioning key wrapped in DLM.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key SHA-256 HMAC key encrypted and MAC.
* @param[out]    key_index SHA-256 HMAC key user key generation information (17 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateShaHmacKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateSha256HmacKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_hmac_sha_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;

    indata_cmd = change_endian_long(2); /* SHA-256 */
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_GenerateShaHmacKeyIndexSub(&install_key_ring_index, (uint32_t*)encrypted_provisioning_key,
        /* Casting uint32_t pointer is used for address. */
        &indata_cmd, (uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_HMAC_SHA256;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateSha256HmacKeyIndex
 *******************************/
#endif /* TSIP_SHA_256_HMAC */

#if TSIP_SHA_1_HMAC
/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateSha1HmacKeyIndex
*******************************************************************************************************************/ /**
* @details       Update SHA-1 HMAC user key index.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key SHA-1 HMAC key encrypted and MAC.
* @param[out]    key_index SHA-1 HMAC key user key generation information (17 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateShaHmacKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateSha1HmacKeyIndex(uint8_t *iv, uint8_t *encrypted_key, tsip_hmac_sha_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;

    indata_cmd = change_endian_long(0); /* SHA-1 */
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_UpdateShaHmacKeyIndexSub(&indata_cmd, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)
        &key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_HMAC_SHA1;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_UpdateSha1HmacKeyIndex
 *******************************/
#endif /* TSIP_SHA_1_HMAC */

#if TSIP_SHA_256_HMAC
/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateSha256HmacKeyIndex
*******************************************************************************************************************/ /**
* @details       Update SHA-256 HMAC user key index.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key SHA-256 HMAC key encrypted and MAC.
* @param[out]    key_index SHA-256 HMAC key user key generation information (17 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateShaHmacKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateSha256HmacKeyIndex(uint8_t *iv, uint8_t *encrypted_key, tsip_hmac_sha_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;

    indata_cmd = change_endian_long(2); /* SHA-256 */
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_UpdateShaHmacKeyIndexSub(&indata_cmd, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)
        &key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_HMAC_SHA256;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_UpdateSha256HmacKeyIndex
 *******************************/
#endif /* TSIP_SHA_256_HMAC */

#if TSIP_SHA_1_HMAC == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Sha1HmacGenerateInit
*******************************************************************************************************************/ /**
* @details       prepares to generate the SHA1 HMAC hash value
* @param[out]    handle Handler.
* @param[in]     key_index key index data
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Sha1HmacGenerateInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha1HmacGenerateInit(tsip_hmac_sha_handle_t *handle, tsip_hmac_sha_key_index_t *key_index)
{
    memset(handle, 0, sizeof(tsip_hmac_sha_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_sha1hmacgen_private_id = g_private_id_counter;
    handle->id = g_sha1hmacgen_private_id;
    return R_TSIP_Sha1HmacGenerateInitPrivate(handle, key_index);
}
/*******************************
 End of function R_TSIP_Sha1HmacGenerateInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha1HmacGenerateUpdate
*******************************************************************************************************************/ /**
* @details       Calculate the SHA1 HMAC hash value
* @param[in,out] handle Handle.
* @param[in]     message Input message text
* @param[in]     message_length Input message length
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Sha1HmacGenerateUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha1HmacGenerateUpdate(tsip_hmac_sha_handle_t *handle, uint8_t *message, uint32_t message_length)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t length_rest = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_sha1hmacgen_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    handle->all_received_length += message_length;
    if ((handle->buffering_length + message_length) >= SHA_BLOCK8_LEN)
    {
        memcpy((handle->hmac_buffer + handle->buffering_length), message,
                SHA_BLOCK8_LEN - handle->buffering_length);

        /*Casting uint32_t pointer is used for address.*/
        R_TSIP_Sha1HmacGenerateUpdatePrivate(handle, (uint32_t *) (handle->hmac_buffer), (SHA_BLOCK8_LEN) >> 2);
        length_rest = message_length - (SHA_BLOCK8_LEN - handle->buffering_length);
        memset(handle->hmac_buffer, 0, sizeof(handle->hmac_buffer));
        if (length_rest >= SHA_BLOCK8_LEN)
        {
            R_TSIP_Sha1HmacGenerateUpdatePrivate(handle,
                    /* Casting uint32_t pointer is used for address. */
                    (uint32_t *) (message + (SHA_BLOCK8_LEN - handle->buffering_length)),
                    ((length_rest / SHA_BLOCK8_LEN) * SHA_BLOCK8_LEN) >> 2);
            length_rest -= ((length_rest / SHA_BLOCK8_LEN) * SHA_BLOCK8_LEN);
        }
        handle->buffering_length = 0;
        memcpy(handle->hmac_buffer, message + (message_length - length_rest), length_rest);
        handle->buffering_length = length_rest;
    }
    else
    {
        memcpy(handle->hmac_buffer + handle->buffering_length, message, message_length);
        handle->buffering_length += message_length;
    }
    return ercd;
}
/*******************************
 End of function R_TSIP_Sha1HmacGenerateUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha1HmacGenerateFinal
*******************************************************************************************************************/ /**
* @details       Generate the SHA1 HMAC hash operation
* @param[in]     handle Handle.
* @param[out]    mac Output MAC
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Sha1HmacGenerateUpdatePrivate()
* @see           R_TSIP_Sha1HmacGenerateFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha1HmacGenerateFinal(tsip_hmac_sha_handle_t *handle, uint8_t *mac)
{
    uint32_t max_cnt_byte = 0;
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t length_tmp = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_sha1hmacgen_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->hmac_buffer[handle->buffering_length] = 0x80;
    length_tmp = handle->all_received_length + 64;
    if ((SHA_BLOCK8_LEN - handle->buffering_length) >= 9) /* another block unnecessary */
    {
        memset(handle->hmac_buffer + (handle->buffering_length + 1), 0,
                (SHA_BLOCK8_LEN - 4) - (handle->buffering_length + 1));

        /* According c., but support only l < 2**32 case */
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[SHA_BLOCK8_LEN - 4] = (uint8_t) ((length_tmp * 8) >> 24);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[SHA_BLOCK8_LEN - 3] = (uint8_t) (((length_tmp * 8) >> 16) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[SHA_BLOCK8_LEN - 2] = (uint8_t) (((length_tmp * 8) >> 8) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[SHA_BLOCK8_LEN - 1] = (uint8_t) ((length_tmp * 8) & 0xff);
        max_cnt_byte = SHA_BLOCK8_LEN;
    }
    else /* another block necessary */
    {
        memset(handle->hmac_buffer + (handle->buffering_length + 1), 0,
                ((2 * SHA_BLOCK8_LEN) - 4) - (handle->buffering_length + 1));

        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[(2 * SHA_BLOCK8_LEN) - 4] = (uint8_t) ((length_tmp * 8) >> 24);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[(2 * SHA_BLOCK8_LEN) - 3] = (uint8_t) (((length_tmp * 8) >> 16) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[(2 * SHA_BLOCK8_LEN) - 2] = (uint8_t) (((length_tmp * 8) >> 8) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[(2 * SHA_BLOCK8_LEN) - 1] = (uint8_t) ((length_tmp * 8) & 0xff);
        max_cnt_byte = 2 * SHA_BLOCK8_LEN;
    }

    /*Casting uint32_t pointer is used for address.*/
    R_TSIP_Sha1HmacGenerateUpdatePrivate(handle, (uint32_t *) (handle->hmac_buffer), (max_cnt_byte) >> 2);

    /*Casting uint32_t pointer is used for address.*/
    ercd = R_TSIP_Sha1HmacGenerateFinalPrivate(handle, (uint32_t *) (mac));
    handle->all_received_length = 0;
    handle->buffering_length = 0;
    memset(handle->hmac_buffer, 0, sizeof(handle->hmac_buffer));
    return ercd;
}
/*******************************
 End of function R_TSIP_Sha1HmacGenerateFinal
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha1HmacVerifyInit
*******************************************************************************************************************/ /**
* @details       prepares to verify the SHA1 HMAC hash value
* @param[out]    handle Handler.
* @param[in]     key_index key index data
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Sha1HmacVerifyInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha1HmacVerifyInit(tsip_hmac_sha_handle_t *handle, tsip_hmac_sha_key_index_t *key_index)
{
    memset(handle, 0, sizeof(tsip_hmac_sha_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_sha1hmacver_private_id = g_private_id_counter;
    handle->id = g_sha1hmacver_private_id;
    return R_TSIP_Sha1HmacVerifyInitPrivate(handle, key_index);
}
/*******************************
 End of function R_TSIP_Sha1HmacVerifyInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha1HmacVerifyUpdate
*******************************************************************************************************************/ /**
* @details       Calculate the SHA1 HMAC hash value
* @param[in,out] handle Handle.
* @param[in]     message Input message text
* @param[in]     message_length Input message length
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Sha1HmacVerifyUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha1HmacVerifyUpdate(tsip_hmac_sha_handle_t *handle, uint8_t *message, uint32_t message_length)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t length_rest = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_sha1hmacver_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    handle->all_received_length += message_length;
    if ((handle->buffering_length + message_length) >= SHA_BLOCK8_LEN)
    {
        memcpy((handle->hmac_buffer + handle->buffering_length), message,
                SHA_BLOCK8_LEN - handle->buffering_length);

        /*Casting uint32_t pointer is used for address.*/
        ercd = R_TSIP_Sha1HmacVerifyUpdatePrivate(handle, (uint32_t *) (handle->hmac_buffer), (SHA_BLOCK8_LEN) >> 2);
        length_rest = message_length - (SHA_BLOCK8_LEN - handle->buffering_length);
        memset(handle->hmac_buffer, 0, sizeof(handle->hmac_buffer));
        if (length_rest >= SHA_BLOCK8_LEN)
        {
            ercd = R_TSIP_Sha1HmacVerifyUpdatePrivate(handle,
                    /* Casting uint32_t pointer is used for address. */
                    (uint32_t *) (message + (SHA_BLOCK8_LEN - handle->buffering_length)),
                    ((length_rest / SHA_BLOCK8_LEN) * SHA_BLOCK8_LEN) >> 2);
            length_rest -= ((length_rest / SHA_BLOCK8_LEN) * SHA_BLOCK8_LEN);
        }
        handle->buffering_length = 0;
        memcpy(handle->hmac_buffer, message + (message_length - length_rest), length_rest);
        handle->buffering_length = length_rest;
    }
    else
    {
        memcpy(handle->hmac_buffer + handle->buffering_length, message, message_length);
        handle->buffering_length += message_length;
    }
    return ercd;
}
/*******************************
 End of function R_TSIP_Sha1HmacVerifyUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha1HmacVerifyFinal
*******************************************************************************************************************/ /**
* @details       Verify the SHA1 HMAC hash value
* @param[in]     handle Handle.
* @param[in]     mac Output hmac
* @param[in]     mac_length Output hmac length
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Sha1HmacVerifyUpdatePrivate()
* @see           R_TSIP_Sha1HmacVerifyFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha1HmacVerifyFinal(tsip_hmac_sha_handle_t *handle, uint8_t *mac, uint32_t mac_length)
{
    uint32_t max_cnt_byte = 0;
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t length_tmp = 0;
    uint32_t mac_length_tmp = 0;
    uint8_t mac_tmp[20] =
    {
        0
    };

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_sha1hmacver_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->hmac_buffer[handle->buffering_length] = 0x80;
    length_tmp = handle->all_received_length + 64;
    if ((SHA_BLOCK8_LEN - handle->buffering_length) >= 9) /* another block unnecessary */
    {
        memset(handle->hmac_buffer + (handle->buffering_length + 1), 0,
                (SHA_BLOCK8_LEN - 4) - (handle->buffering_length + 1));

        /* According c., but support only l < 2**32 case */
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[SHA_BLOCK8_LEN - 4] = (uint8_t) ((length_tmp * 8) >> 24);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[SHA_BLOCK8_LEN - 3] = (uint8_t) (((length_tmp * 8) >> 16) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[SHA_BLOCK8_LEN - 2] = (uint8_t) (((length_tmp * 8) >> 8) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[SHA_BLOCK8_LEN - 1] = (uint8_t) ((length_tmp * 8) & 0xff);
        max_cnt_byte = SHA_BLOCK8_LEN;
    }
    else /* another block necessary */
    {
        memset(handle->hmac_buffer + (handle->buffering_length + 1), 0,
                ((2 * SHA_BLOCK8_LEN) - 4) - (handle->buffering_length + 1));

        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[(2 * SHA_BLOCK8_LEN) - 4] = (uint8_t) ((length_tmp * 8) >> 24);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[(2 * SHA_BLOCK8_LEN) - 3] = (uint8_t) (((length_tmp * 8) >> 16) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[(2 * SHA_BLOCK8_LEN) - 2] = (uint8_t) (((length_tmp * 8) >> 8) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[(2 * SHA_BLOCK8_LEN) - 1] = (uint8_t) ((length_tmp * 8) & 0xff);
        max_cnt_byte = 2 * SHA_BLOCK8_LEN;
    }
    memcpy(mac_tmp, mac, mac_length);
    if ((4 <= mac_length) && (mac_length <= 20))
    {
        memset(mac_tmp + mac_length, 0, (sizeof(mac_tmp)) - mac_length);
    }
    else
    {
        return TSIP_ERR_PARAMETER;
    }
    mac_length_tmp = change_endian_long(mac_length);

    /*Casting uint32_t pointer is used for address.*/
    ercd = R_TSIP_Sha1HmacVerifyUpdatePrivate(handle, (uint32_t *) (handle->hmac_buffer), (max_cnt_byte) >> 2);

    /*Casting uint32_t pointer is used for address.*/
    ercd = R_TSIP_Sha1HmacVerifyFinalPrivate(handle, (uint32_t *) (mac_tmp), &mac_length_tmp);
    handle->all_received_length = 0;
    handle->buffering_length = 0;
    memset(handle->hmac_buffer, 0, sizeof(handle->hmac_buffer));
    return ercd;
}
/*******************************
 End of function R_TSIP_Sha1HmacVerifyFinal
 *******************************/
#endif /* TSIP_SHA_1_HMAC == 1 */

#if TSIP_SHA_256_HMAC == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Sha256HmacGenerateInit
*******************************************************************************************************************/ /**
* @details       prepares to generate the SHA256 HMAC hash value
* @param[out]    handle Handler.
* @param[in]     key_index key index data
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Sha256HmacGenerateInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha256HmacGenerateInit(tsip_hmac_sha_handle_t *handle, tsip_hmac_sha_key_index_t *key_index)
{
    memset(handle, 0, sizeof(tsip_hmac_sha_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_sha256hmacgen_private_id = g_private_id_counter;
    handle->id = g_sha256hmacgen_private_id;
    return R_TSIP_Sha256HmacGenerateInitPrivate(handle, key_index);
}
/*******************************
 End of function R_TSIP_Sha256HmacGenerateInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha256HmacGenerateUpdate
*******************************************************************************************************************/ /**
* @details       Calculate the SHA256 HMAC hash value
* @param[in,out] handle Handle.
* @param[in]     message Input message text
* @param[in]     message_length Input message length
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Sha256HmacGenerateUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha256HmacGenerateUpdate(tsip_hmac_sha_handle_t *handle, uint8_t *message, uint32_t message_length)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t length_rest = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_sha256hmacgen_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    handle->all_received_length += message_length;
    if ((handle->buffering_length + message_length) >= SHA_BLOCK8_LEN)
    {
        memcpy((handle->hmac_buffer + handle->buffering_length), message,
                SHA_BLOCK8_LEN - handle->buffering_length);

        /*Casting uint32_t pointer is used for address.*/
        ercd = R_TSIP_Sha256HmacGenerateUpdatePrivate(handle, (uint32_t *) (handle->hmac_buffer),
                (SHA_BLOCK8_LEN) >> 2);
        length_rest = message_length - (SHA_BLOCK8_LEN - handle->buffering_length);
        memset(handle->hmac_buffer, 0, sizeof(handle->hmac_buffer));
        if (length_rest >= SHA_BLOCK8_LEN)
        {
            ercd = R_TSIP_Sha256HmacGenerateUpdatePrivate(handle,
                    /* Casting uint32_t pointer is used for address. */
                    (uint32_t *) (message + (SHA_BLOCK8_LEN - handle->buffering_length)),
                    ((length_rest / SHA_BLOCK8_LEN) * SHA_BLOCK8_LEN) >> 2);
            length_rest -= ((length_rest / SHA_BLOCK8_LEN) * SHA_BLOCK8_LEN);
        }
        handle->buffering_length = 0;
        memcpy(handle->hmac_buffer, message + (message_length - length_rest), length_rest);
        handle->buffering_length = length_rest;
    }
    else
    {
        memcpy(handle->hmac_buffer + handle->buffering_length, message, message_length);
        handle->buffering_length += message_length;
    }
    return ercd;
}
/*******************************
 End of function R_TSIP_Sha256HmacGenerateUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha256HmacGenerateFinal
*******************************************************************************************************************/ /**
* @details       Generate the SHA256 HMAC hash operation
* @param[in]     handle Handle.
* @param[out]    mac Output MAC
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Sha256HmacGenerateUpdatePrivate()
* @see           R_TSIP_Sha256HmacGenerateFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha256HmacGenerateFinal(tsip_hmac_sha_handle_t *handle, uint8_t *mac)
{
    uint32_t max_cnt_byte = 0;
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t length_tmp = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_sha256hmacgen_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->hmac_buffer[handle->buffering_length] = 0x80;
    length_tmp = handle->all_received_length + 64;
    if ((SHA_BLOCK8_LEN - handle->buffering_length) >= 9) /* another block unnecessary */
    {
        memset(handle->hmac_buffer + (handle->buffering_length + 1), 0,
                (SHA_BLOCK8_LEN - 4) - (handle->buffering_length + 1));

        /* According c., but support only l < 2**32 case */
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[SHA_BLOCK8_LEN - 4] = (uint8_t) ((length_tmp * 8) >> 24);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[SHA_BLOCK8_LEN - 3] = (uint8_t) (((length_tmp * 8) >> 16) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[SHA_BLOCK8_LEN - 2] = (uint8_t) (((length_tmp * 8) >> 8) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[SHA_BLOCK8_LEN - 1] = (uint8_t) ((length_tmp * 8) & 0xff);
        max_cnt_byte = SHA_BLOCK8_LEN;
    }
    else /* another block necessary */
    {
        memset(handle->hmac_buffer + (handle->buffering_length + 1), 0,
                ((2 * SHA_BLOCK8_LEN) - 4) - (handle->buffering_length + 1));

        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[(2 * SHA_BLOCK8_LEN) - 4] = (uint8_t) ((length_tmp * 8) >> 24);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[(2 * SHA_BLOCK8_LEN) - 3] = (uint8_t) (((length_tmp * 8) >> 16) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[(2 * SHA_BLOCK8_LEN) - 2] = (uint8_t) (((length_tmp * 8) >> 8) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[(2 * SHA_BLOCK8_LEN) - 1] = (uint8_t) ((length_tmp * 8) & 0xff);
        max_cnt_byte = 2 * SHA_BLOCK8_LEN;
    }

    /*Casting uint32_t pointer is used for address.*/
    ercd = R_TSIP_Sha256HmacGenerateUpdatePrivate(handle, (uint32_t *) (handle->hmac_buffer), (max_cnt_byte) >> 2);

    /*Casting uint32_t pointer is used for address.*/
    ercd = R_TSIP_Sha256HmacGenerateFinalPrivate(handle, (uint32_t *) (mac));
    handle->all_received_length = 0;
    handle->buffering_length = 0;
    memset(handle->hmac_buffer, 0, sizeof(handle->hmac_buffer));
    return ercd;
}
/*******************************
 End of function R_TSIP_Sha256HmacGenerateFinal
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha256HmacVerifyInit
*******************************************************************************************************************/ /**
* @details       prepares to verify the SHA256 HMAC hash value
* @param[out]    handle Handler.
* @param[in]     key_index key index data
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Sha256HmacVerifyInitPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha256HmacVerifyInit(tsip_hmac_sha_handle_t *handle, tsip_hmac_sha_key_index_t *key_index)
{
    memset(handle, 0, sizeof(tsip_hmac_sha_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    g_private_id_counter++;
    g_sha256hmacver_private_id = g_private_id_counter;
    handle->id = g_sha256hmacver_private_id;
    return R_TSIP_Sha256HmacVerifyInitPrivate(handle, key_index);
}
/*******************************
 End of function R_TSIP_Sha256HmacVerifyInit
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha256HmacVerifyUpdate
*******************************************************************************************************************/ /**
* @details       Calculate the SHA256 HMAC hash value
* @param[in,out] handle Handle.
* @param[in]     message Input message text
* @param[in]     message_length Input message length
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Sha256HmacVerifyUpdatePrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha256HmacVerifyUpdate(tsip_hmac_sha_handle_t *handle, uint8_t *message, uint32_t message_length)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t length_rest = 0;

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_sha256hmacver_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    handle->all_received_length += message_length;
    if ((handle->buffering_length + message_length) >= SHA_BLOCK8_LEN)
    {
        memcpy((handle->hmac_buffer + handle->buffering_length), message,
                SHA_BLOCK8_LEN - handle->buffering_length);

        /*Casting uint32_t pointer is used for address.*/
        ercd = R_TSIP_Sha256HmacVerifyUpdatePrivate(handle, (uint32_t *) (handle->hmac_buffer), (SHA_BLOCK8_LEN) >> 2);
        length_rest = message_length - (SHA_BLOCK8_LEN - handle->buffering_length);
        memset(handle->hmac_buffer, 0, sizeof(handle->hmac_buffer));
        if (length_rest >= SHA_BLOCK8_LEN)
        {
            ercd = R_TSIP_Sha256HmacVerifyUpdatePrivate(handle,
                    /* Casting uint32_t pointer is used for address. */
                    (uint32_t *) (message + (SHA_BLOCK8_LEN - handle->buffering_length)),
                    ((length_rest / SHA_BLOCK8_LEN) * SHA_BLOCK8_LEN) >> 2);
            length_rest -= ((length_rest / SHA_BLOCK8_LEN) * SHA_BLOCK8_LEN);
        }
        handle->buffering_length = 0;
        memcpy(handle->hmac_buffer, message + (message_length - length_rest), length_rest);
        handle->buffering_length = length_rest;
    }
    else
    {
        memcpy(handle->hmac_buffer + handle->buffering_length, message, message_length);
        handle->buffering_length += message_length;
    }
    return ercd;
}
/*******************************
 End of function R_TSIP_Sha256HmacVerifyUpdate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha256HmacVerifyFinal
*******************************************************************************************************************/ /**
* @details       Verify the SHA256 HMAC hash value
* @param[in]     handle Handle.
* @param[in]     mac Output hmac
* @param[in]     mac_length Output hmac length
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_Sha256HmacVerifyUpdatePrivate()
* @see           R_TSIP_Sha256HmacVerifyFinalPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha256HmacVerifyFinal(tsip_hmac_sha_handle_t *handle, uint8_t *mac, uint32_t mac_length)
{
    uint32_t max_cnt_byte = 0;
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint32_t length_tmp = 0;
    uint32_t mac_length_tmp = 0;
    uint8_t mac_tmp[32] =
    {
        0
    };

    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != g_sha256hmacver_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->hmac_buffer[handle->buffering_length] = 0x80;
    length_tmp = handle->all_received_length + 64;
    if ((SHA_BLOCK8_LEN - handle->buffering_length) >= 9) /* another block unnecessary */
    {
        memset(handle->hmac_buffer + (handle->buffering_length + 1), 0,
                (SHA_BLOCK8_LEN - 4) - (handle->buffering_length + 1));

        /* According c., but support only l < 2**32 case */
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[SHA_BLOCK8_LEN - 4] = (uint8_t) ((length_tmp * 8) >> 24);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[SHA_BLOCK8_LEN - 3] = (uint8_t) (((length_tmp * 8) >> 16) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[SHA_BLOCK8_LEN - 2] = (uint8_t) (((length_tmp * 8) >> 8) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[SHA_BLOCK8_LEN - 1] = (uint8_t) ((length_tmp * 8) & 0xff);
        max_cnt_byte = SHA_BLOCK8_LEN;
    }
    else /* another block necessary */
    {
        memset(handle->hmac_buffer + (handle->buffering_length + 1), 0,
                ((2 * SHA_BLOCK8_LEN) - 4) - (handle->buffering_length + 1));

        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[(2 * SHA_BLOCK8_LEN) - 4] = (uint8_t) ((length_tmp * 8) >> 24);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[(2 * SHA_BLOCK8_LEN) - 3] = (uint8_t) (((length_tmp * 8) >> 16) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[(2 * SHA_BLOCK8_LEN) - 2] = (uint8_t) (((length_tmp * 8) >> 8) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        handle->hmac_buffer[(2 * SHA_BLOCK8_LEN) - 1] = (uint8_t) ((length_tmp * 8) & 0xff);
        max_cnt_byte = 2 * SHA_BLOCK8_LEN;
    }
    memcpy(mac_tmp, mac, mac_length);
    if ((4 <= mac_length) && (mac_length <= 32))
    {
        memset(mac_tmp + mac_length, 0, (sizeof(mac_tmp)) - mac_length);
    }
    else
    {
        return TSIP_ERR_PARAMETER;
    }
    mac_length_tmp = change_endian_long(mac_length);

    /*Casting uint32_t pointer is used for address.*/
    ercd = R_TSIP_Sha256HmacVerifyUpdatePrivate(handle, (uint32_t *) (handle->hmac_buffer), (max_cnt_byte) >> 2);

    /*Casting uint32_t pointer is used for address.*/
    ercd = R_TSIP_Sha256HmacVerifyFinalPrivate(handle, (uint32_t *) (mac_tmp), &mac_length_tmp);
    handle->all_received_length = 0;
    handle->buffering_length = 0;
    memset(handle->hmac_buffer, 0, sizeof(handle->hmac_buffer));
    return ercd;
}
/*******************************
 End of function R_TSIP_Sha256HmacVerifyFinal
 *******************************/
#endif /* TSIP_SHA_256_HMAC == 1 */

