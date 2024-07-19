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
 * File Name    : r_tsip_rx_private.c
 * Version      : 1.21
 * Description  : Interface definition for the TSIP private layer.
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

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateRandomNumberPrivate
*******************************************************************************************************************/ /**
* @details       This function can generate 4 word random number.
* @param[out]    random Stores 4words (16 bytes) random data.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @see           R_TSIP_GenerateRandomNumberSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateRandomNumberPrivate(uint32_t *random)
{
    e_tsip_err_t ercd;

    ercd = R_TSIP_GenerateRandomNumberSub(random);

    return ercd;
}
/**************************************
 End of function R_TSIP_GenerateRandomNumberPrivate
 **************************************/

#if TSIP_AES_128_ECB_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128EcbEncryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES ECB mode algorithm with AES 128 key.
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128EcbEncryptInitPrivate(tsip_aes_key_index_t *key_index)
{
    uint32_t indata_keytype = 0; /* For normal */
    uint32_t indata_cmd = change_endian_long(0); /* ECB-Encrypt command */
    uint32_t indata_iv[4] = /* dummy */
    {
        0
    };

    if ((TSIP_KEY_INDEX_TYPE_AES128 != key_index->type) && (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH != key_index->type))
    {
        return TSIP_ERR_KEY_SET;
    }
    else if (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    else
    {
        R_BSP_NOP();
    }
    return R_TSIP_Aes128EncryptDecryptInitSub(&indata_keytype, &indata_cmd, key_index->value, indata_iv);
}
/*************************************************
 End of function R_TSIP_Aes128EcbEncryptInitPrivate
 **************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128EcbEncryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES ECB mode algorithm with AES 128 key.
* @param[in]     InData_Text Input plain text.
* @param[out]    OutData_Text Output cipher text.
* @param[in]     MAX_CNT Input word size of InData_Text.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes128EncryptDecryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128EcbEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
{
    R_TSIP_Aes128EncryptDecryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);

    return TSIP_SUCCESS;
}
/***************************************************
 End of function R_TSIP_Aes128EcbEncryptUpdatePrivate
 ****************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128EcbEncryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES ECB mode algorithm with AES 128 key.
* @param[out]    OutData_Text Output cipher text.
* @param[out]    OutData_length Output byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128EncryptDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128EcbEncryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length)
{
    return R_TSIP_Aes128EncryptDecryptFinalSub();
}
/***************************************************
 End of function R_TSIP_Aes128EcbEncryptFinalPrivate
 ****************************************************/
#endif /* TSIP_AES_128_ECB_ENCRYPT == 1 */

#if TSIP_AES_128_ECB_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128EcbDecryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES ECB mode algorithm with AES 128 key.
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128EcbDecryptInitPrivate(tsip_aes_key_index_t *key_index)
{
    uint32_t indata_keytype = 0; /* For normal */
    uint32_t indata_cmd = change_endian_long(1); /* ECB-Decrypt command */
    uint32_t indata_iv[4] = /* dummy */
    {
        0
    };

    if ((TSIP_KEY_INDEX_TYPE_AES128 != key_index->type) && (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH != key_index->type))
    {
        return TSIP_ERR_KEY_SET;
    }
    else if (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    else
    {
        R_BSP_NOP();
    }
    return R_TSIP_Aes128EncryptDecryptInitSub(&indata_keytype, &indata_cmd, key_index->value, indata_iv);
}
/*************************************************
 End of function R_TSIP_Aes128EcbDecryptInitPrivate
 **************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128EcbDecryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES ECB mode algorithm with AES 128 key.
* @param[in]     InData_Text Input cipher text.
* @param[out]    OutData_Text Output plain text.
* @param[in]     MAX_CNT Input word size of InData_Text.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes128EncryptDecryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128EcbDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
{
    R_TSIP_Aes128EncryptDecryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);

    return TSIP_SUCCESS;
}
/***************************************************
 End of function R_TSIP_Aes128EcbDecryptUpdatePrivate
 ****************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128EcbDecryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES ECB mode algorithm with AES 128 key.
* @param[out]    OutData_Text Output plain text.
* @param[out]    OutData_length Output byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128EncryptDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128EcbDecryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length)
{
    return R_TSIP_Aes128EncryptDecryptFinalSub();
}
/***************************************************
 End of function R_TSIP_Aes128EcbDecryptFinalPrivate
 ****************************************************/
#endif /* TSIP_AES_128_ECB_DECRYPT == 1 */

#if TSIP_AES_256_ECB_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256EcbEncryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES ECB mode algorithm with AES 256 key.
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256EcbEncryptInitPrivate(tsip_aes_key_index_t *key_index)
{
    uint32_t indata_keytype = 0; /* For normal */
    uint32_t indata_cmd = change_endian_long(0); /* ECB-Encrypt command */
    uint32_t indata_iv[4] = /* dummy */
    {
        0
    };

    if ((TSIP_KEY_INDEX_TYPE_AES256 != key_index->type) && (TSIP_KEY_INDEX_TYPE_AES256_FOR_ECDH != key_index->type))
    {
        return TSIP_ERR_KEY_SET;
    }
    else if (TSIP_KEY_INDEX_TYPE_AES256_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    else
    {
        R_BSP_NOP();
    }
    return R_TSIP_Aes256EncryptDecryptInitSub(&indata_keytype, &indata_cmd, key_index->value, indata_iv);
}
/*************************************************
 End of function R_TSIP_Aes256EcbEncryptInitPrivate
 **************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256EcbEncryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES ECB mode algorithm with AES 256 key.
* @param[in]     InData_Text Input plain text.
* @param[out]    OutData_Text Output cipher text.
* @param[in]     MAX_CNT Input word size of InData_Text.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes256EncryptDecryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256EcbEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
{
    R_TSIP_Aes256EncryptDecryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);

    return TSIP_SUCCESS;
}
/***************************************************
 End of function R_TSIP_Aes256EcbEncryptUpdatePrivate
 ****************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256EcbEncryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES ECB mode algorithm with AES 256 key.
* @param[out]    OutData_Text Output cipher text.
* @param[out]    OutData_length Output byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256EncryptDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256EcbEncryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length)
{
    return R_TSIP_Aes256EncryptDecryptFinalSub();
}
/***************************************************
 End of function R_TSIP_Aes256EcbEncryptFinalPrivate
 ****************************************************/
#endif /* TSIP_AES_256_ECB_ENCRYPT == 1 */

#if TSIP_AES_256_ECB_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256EcbDecryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using AES ECB mode algorithm with AES 256 key.
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256EcbDecryptInitPrivate(tsip_aes_key_index_t *key_index)
{
    uint32_t indata_keytype = 0; /* For normal */
    uint32_t indata_cmd = change_endian_long(1); /* ECB-Decrypt command */
    uint32_t indata_iv[4] = /* dummy */
    {
        0
    };

    if ((TSIP_KEY_INDEX_TYPE_AES256 != key_index->type) && (TSIP_KEY_INDEX_TYPE_AES256_FOR_ECDH != key_index->type))
    {
        return TSIP_ERR_KEY_SET;
    }
    else if (TSIP_KEY_INDEX_TYPE_AES256_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    else
    {
        R_BSP_NOP();
    }
    return R_TSIP_Aes256EncryptDecryptInitSub(&indata_keytype, &indata_cmd, key_index->value, indata_iv);
}
/*************************************************
 End of function R_TSIP_Aes256EcbDecryptInitPrivate
 **************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256EcbDecryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES ECB mode algorithm with AES 256 key.
* @param[in]     InData_Text Input cipher text.
* @param[out]    OutData_Text Output plain text.
* @param[in]     MAX_CNT Input word size of InData_Text.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes256EncryptDecryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256EcbDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
{
    R_TSIP_Aes256EncryptDecryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);

    return TSIP_SUCCESS;
}
/***************************************************
 End of function R_TSIP_Aes256EcbDecryptUpdatePrivate
 ****************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256EcbDecryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES ECB mode algorithm with AES 256 key.
* @param[out]    OutData_Text Output plain text.
* @param[out]    OutData_length Output byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256EncryptDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256EcbDecryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length)
{
    return R_TSIP_Aes256EncryptDecryptFinalSub();
}
/***************************************************
 End of function R_TSIP_Aes256EcbDecryptFinalPrivate
 ****************************************************/
#endif /* TSIP_AES_256_ECB_DECRYPT == 1 */

#if TSIP_AES_128_CBC_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CbcEncryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CBC mode algorithm with AES 128 key.
* @param[in]     key_index User key generation information area.
* @param[in]     InData_IV Input initial vector.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CbcEncryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV)
{
    uint32_t indata_keytype = 0; /* For normal */
    uint32_t indata_cmd = change_endian_long(2); /* CBC-Encrypt command */

    if ((TSIP_KEY_INDEX_TYPE_AES128 != key_index->type) && ((TSIP_KEY_INDEX_TYPE_AES128_FOR_TLS != key_index->type)
    && (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH != key_index->type)))
    {
        return TSIP_ERR_KEY_SET;
    }
    else if (TSIP_KEY_INDEX_TYPE_AES128_FOR_TLS == key_index->type)
    {
        indata_cmd = change_endian_long(5); /* CBC-Encrypt for TLS command */
    }
    else if (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    else
    {
        R_BSP_NOP();
    }
    return R_TSIP_Aes128EncryptDecryptInitSub(&indata_keytype, &indata_cmd, key_index->value, InData_IV);
}
/*************************************************
 End of function R_TSIP_Aes128CbcEncryptInitPrivate
 **************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CbcEncryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CBC mode algorithm with AES 128 key.
* @param[in]     InData_Text Input plain text.
* @param[out]    OutData_Text Output cipher text.
* @param[in]     MAX_CNT Input word size of InData_Text.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes128EncryptDecryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CbcEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
{
    R_TSIP_Aes128EncryptDecryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);

    return TSIP_SUCCESS;
}
/***************************************************
 End of function R_TSIP_Aes128CbcEncryptUpdatePrivate
 ****************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CbcEncryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CBC mode algorithm with AES 128 key.
* @param[out]    OutData_Text Output cipher text.
* @param[out]    OutData_length Output byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128EncryptDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CbcEncryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length)
{
    return R_TSIP_Aes128EncryptDecryptFinalSub();
}
/***************************************************
 End of function R_TSIP_Aes128CbcEncryptFinalPrivate
 ***************************************************/
#endif /* TSIP_AES_128_CBC_ENCRYPT == 1 */

#if TSIP_AES_128_CBC_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CbcDecryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using AES CBC mode algorithm with AES 128 key.
* @param[in]     key_index User key generation information area.
* @param[in]     InData_IV Input initial vector.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CbcDecryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV)
{
    uint32_t indata_keytype = 0; /* For normal */
    uint32_t indata_cmd = change_endian_long(3); /* CBC-Decrypt command */

    if ((TSIP_KEY_INDEX_TYPE_AES128 != key_index->type) && ((TSIP_KEY_INDEX_TYPE_AES128_FOR_TLS != key_index->type)
    && (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH != key_index->type)))
    {
        return TSIP_ERR_KEY_SET;
    }
    else if (TSIP_KEY_INDEX_TYPE_AES128_FOR_TLS == key_index->type)
    {
        indata_cmd = change_endian_long(6); /* CBC-Decrypt for TLS command */
    }
    else if (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    else
    {
        R_BSP_NOP();
    }
    return R_TSIP_Aes128EncryptDecryptInitSub(&indata_keytype, &indata_cmd, key_index->value, InData_IV);
}
/*************************************************
 End of function R_TSIP_Aes128CbcDecryptInitPrivate
 **************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CbcDecryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES CBC mode algorithm with AES 128 key.
* @param[in]     InData_Text Input cipher text.
* @param[out]    OutData_Text Output plain text.
* @param[in]     MAX_CNT Input word size of InData_Text.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes128EncryptDecryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CbcDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
{
    R_TSIP_Aes128EncryptDecryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);

    return TSIP_SUCCESS;
}
/***************************************************
 End of function R_TSIP_Aes128CbcDecryptUpdatePrivate
 ****************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CbcDecryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES CBC mode algorithm with AES 128 key.
* @param[out]    OutData_Text Output plain text.
* @param[out]    OutData_length Output byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128EncryptDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CbcDecryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length)
{
    return R_TSIP_Aes128EncryptDecryptFinalSub();
}
/***************************************************
 End of function R_TSIP_Aes128CbcDecryptFinalPrivate
 ***************************************************/
#endif /* TSIP_AES_128_CBC_DECRYPT == 1 */

#if TSIP_AES_256_CBC_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CbcEncryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CBC mode algorithm with AES 256 key.
* @param[in]     key_index User key generation information area.
* @param[in]     InData_IV Input initial vector.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CbcEncryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV)
{
    uint32_t indata_keytype = 0; /* For normal */
    uint32_t indata_cmd = change_endian_long(2); /* CBC-Encrypt command */

    if ((TSIP_KEY_INDEX_TYPE_AES256 != key_index->type) && ((TSIP_KEY_INDEX_TYPE_AES256_FOR_TLS != key_index->type)
    && (TSIP_KEY_INDEX_TYPE_AES256_FOR_ECDH != key_index->type)))
    {
        return TSIP_ERR_KEY_SET;
    }
    else if (TSIP_KEY_INDEX_TYPE_AES256_FOR_TLS == key_index->type)
    {
        indata_cmd = change_endian_long(5); /* CBC-Encrypt for TLS command */
    }
    else if (TSIP_KEY_INDEX_TYPE_AES256_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    else
    {
        R_BSP_NOP();
    }
    return R_TSIP_Aes256EncryptDecryptInitSub(&indata_keytype, &indata_cmd, key_index->value, InData_IV);
}
/*************************************************
 End of function R_TSIP_Aes256CbcEncryptInitPrivate
 **************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CbcEncryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CBC mode algorithm with AES 256 key.
* @param[in]     InData_Text Input plain text.
* @param[out]    OutData_Text Output cipher text.
* @param[in]     MAX_CNT Input word size of InData_Text.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes256EncryptDecryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CbcEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
{
    R_TSIP_Aes256EncryptDecryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);

    return TSIP_SUCCESS;
}
/***************************************************
 End of function R_TSIP_Aes256CbcEncryptUpdatePrivate
 ****************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CbcEncryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CBC mode algorithm with AES 256 key.
* @param[out]    OutData_Text Output cipher text.
* @param[out]    OutData_length Output byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256EncryptDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CbcEncryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length)
{
    return R_TSIP_Aes256EncryptDecryptFinalSub();
}
/***************************************************
 End of function R_TSIP_Aes256CbcEncryptFinalPrivate
 ***************************************************/
#endif /* TSIP_AES_256_CBC_ENCRYPT == 1 */

#if TSIP_AES_256_CBC_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CbcDecryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using AES CBC mode algorithm with AES 256 key.
* @param[in]     key_index User key generation information area.
* @param[in]     InData_IV Input initial vector.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CbcDecryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV)
{
    uint32_t indata_keytype = 0; /* For normal */
    uint32_t indata_cmd = change_endian_long(3); /* CBC-Decrypt command */

    if ((TSIP_KEY_INDEX_TYPE_AES256 != key_index->type) && ((TSIP_KEY_INDEX_TYPE_AES256_FOR_TLS != key_index->type)
    && (TSIP_KEY_INDEX_TYPE_AES256_FOR_ECDH != key_index->type)))
    {
        return TSIP_ERR_KEY_SET;
    }
    else if (TSIP_KEY_INDEX_TYPE_AES256_FOR_TLS == key_index->type)
    {
        indata_cmd = change_endian_long(6); /* CBC-Decrypt for TLS command */
    }
    else if (TSIP_KEY_INDEX_TYPE_AES256_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    else
    {
        R_BSP_NOP();
    }
    return R_TSIP_Aes256EncryptDecryptInitSub(&indata_keytype, &indata_cmd, key_index->value, InData_IV);
}
/*************************************************
 End of function R_TSIP_Aes256CbcDecryptInitPrivate
 **************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CbcDecryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using AES CBC mode algorithm with AES 256 key.
* @param[in]     InData_Text Input cipher text.
* @param[out]    OutData_Text Output plain text.
* @param[in]     MAX_CNT Input word size of InData_Text.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes256EncryptDecryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CbcDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
{
    R_TSIP_Aes256EncryptDecryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);

    return TSIP_SUCCESS;
}
/***************************************************
 End of function R_TSIP_Aes256CbcDecryptUpdatePrivate
 ****************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CbcDecryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using AES CBC mode algorithm with AES 256 key.
* @param[out]    OutData_Text Output plain text.
* @param[out]    OutData_length Output byte size of cipher.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256EncryptDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CbcDecryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length)
{
    return R_TSIP_Aes256EncryptDecryptFinalSub();
}
/***************************************************
 End of function R_TSIP_Aes256CbcDecryptFinalPrivate
 ***************************************************/
#endif /* TSIP_AES_256_CBC_DECRYPT == 1 */

#if TSIP_AES_128_CTR == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CtrInitPrivate
*******************************************************************************************************************/ /**
* @details       Use AES CTR mode algorithm with AES 128 key.
* @param[in]     key_index User key generation information area.
* @param[in]     InData_IV Input initial counter.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CtrInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV)
{
    uint32_t indata_keytype = 0; /* For normal */
    uint32_t indata_cmd = change_endian_long(4); /* CTR command */

    if ((TSIP_KEY_INDEX_TYPE_AES128 != key_index->type) && (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH != key_index->type))
    {
        return TSIP_ERR_KEY_SET;
    }
    else if (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    else
    {
        R_BSP_NOP();
    }
    return R_TSIP_Aes128EncryptDecryptInitSub(&indata_keytype, &indata_cmd, key_index->value, InData_IV);
}
/*************************************************
 End of function R_TSIP_Aes128CtrInitPrivate
 **************************************************/
#endif /* TSIP_AES_128_CTR == 1 */

#if TSIP_AES_256_CTR == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CtrInitPrivate
*******************************************************************************************************************/ /**
* @details       Use AES CTR mode algorithm with AES 256 key.
* @param[in]     key_index User key generation information area.
* @param[in]     InData_IV Input initial counter.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CtrInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV)
{
    uint32_t indata_keytype = 0; /* For normal */
    uint32_t indata_cmd = change_endian_long(4); /* CTR command */

    if ((TSIP_KEY_INDEX_TYPE_AES256 != key_index->type) && (TSIP_KEY_INDEX_TYPE_AES256_FOR_ECDH != key_index->type))
    {
        return TSIP_ERR_KEY_SET;
    }
    else if (TSIP_KEY_INDEX_TYPE_AES256_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    else
    {
        R_BSP_NOP();
    }
    return R_TSIP_Aes256EncryptDecryptInitSub(&indata_keytype, &indata_cmd, key_index->value, InData_IV);
}
/*************************************************
 End of function R_TSIP_Aes256CtrInitPrivate
 **************************************************/
#endif /* TSIP_AES_256_CTR == 1 */

#if TSIP_AES_128_CTR == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CtrUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Use AES CTR mode algorithm with AES 128 key.
* @param[in]     InData_Text Input text.
* @param[out]    OutData_Text Output text.
* @param[in]     MAX_CNT Input word size of InData_Text.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes128EncryptDecryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CtrUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
{
    R_TSIP_Aes128EncryptDecryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);

    return TSIP_SUCCESS;
}
/***************************************************
 End of function R_TSIP_Aes128CtrUpdatePrivate
 ****************************************************/
#endif /* TSIP_AES_128_CTR == 1 */

#if TSIP_AES_256_CTR == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CtrUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Use AES CTR mode algorithm with AES 256 key.
* @param[in]     InData_Text Input text.
* @param[out]    OutData_Text Output text.
* @param[in]     MAX_CNT Input word size of InData_Text.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes256EncryptDecryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CtrUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
{
    R_TSIP_Aes256EncryptDecryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);

    return TSIP_SUCCESS;
}
/***************************************************
 End of function R_TSIP_Aes256CtrUpdatePrivate
 ****************************************************/
#endif /* TSIP_AES_256_CTR == 1 */

#if TSIP_AES_128_CTR == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CtrFinalPrivate
*******************************************************************************************************************/ /**
* @details       Use AES CTR mode algorithm with AES 128 key.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128EncryptDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CtrFinalPrivate(void)
{
    return R_TSIP_Aes128EncryptDecryptFinalSub();
}
/***************************************************
 End of function R_TSIP_Aes128CtrFinalPrivate
 ***************************************************/
#endif /* TSIP_AES_128_CTR == 1 */

#if TSIP_AES_256_CTR == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CtrFinalPrivate
*******************************************************************************************************************/ /**
* @details       Use AES CTR mode algorithm with AES 256 key.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256EncryptDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CtrFinalPrivate(void)
{
    return R_TSIP_Aes256EncryptDecryptFinalSub();
}
/***************************************************
 End of function R_TSIP_Aes256CtrFinalPrivate
 ***************************************************/
#endif /* TSIP_AES_256_CTR == 1 */

#if TSIP_AES_128_GCM_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128GcmEncryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES GCM algorithm with AES 128 key.
* @param[in]     key_index User key generation information area.
* @param[in]     tls13_phase handshake phase.
* @param[in]     tls13_mode handshake protocol.
* @param[in]     tls13_handle handler for TLS1.3(work area).
* @param[in]     InData_IV Input initial vector.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128GcmEncryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128GcmEncryptInitPrivate(tsip_aes_key_index_t *key_index, e_tsip_tls13_phase_t tls13_phase,
        e_tsip_tls13_mode_t tls13_mode, tsip_tls13_handle_t *tls13_handle, uint32_t *InData_IV)
{
    uint32_t indata_keytype;
    uint32_t indata_datatype  = 0;
    uint32_t indata_cmd       = 0;
    uint32_t indata_seqnum[2] = {0, 0};

    if (TSIP_KEY_INDEX_TYPE_AES128_FOR_TLS == key_index->type)
    {
        indata_keytype = change_endian_long(1); /* For TLS */
    }
    else if (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    else if ((TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_CLIENT == key_index->type)
            || (TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_SERVER == key_index->type))
    {
        indata_keytype = change_endian_long(4); /* For TLS1.3 */

        if (TSIP_TLS13_PHASE_HANDSHAKE == tls13_phase)
        {
            if (R_TSIP_TLS13_RESET_SEQNUM == tls13_handle->flag_handshake_tx) /* 1st data transmission to server. */
            {
                tls13_handle->flag_handshake_tx   = R_TSIP_TLS13_INCREMENT_SEQNUM;
                tls13_handle->seqnum_handshake_tx = 0;
            }
            else
            {
                tls13_handle->seqnum_handshake_tx++;
            }

            /* Casting uint32_t is used to input. */
            indata_seqnum[0] = change_endian_long(
                    (uint32_t)((tls13_handle->seqnum_handshake_tx & 0xFFFFFFFF00000000) >> 32));
            /* Casting uint32_t is used to input. */
            indata_seqnum[1] = change_endian_long((uint32_t)(tls13_handle->seqnum_handshake_tx & 0x00000000FFFFFFFF));

            if (TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_CLIENT == key_index->type)
            {
                indata_datatype = change_endian_long(0);
            }
            else /* TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_SERVER == key_index->type */
            {
                indata_datatype = change_endian_long(2);
            }
        }
        else /* TSIP_TLS13_PHASE_APPLICATION */
        {
            if (R_TSIP_TLS13_RESET_SEQNUM == tls13_handle->flag_application_tx) /* 1st data transmission to server. */
            {
                tls13_handle->flag_application_tx   = R_TSIP_TLS13_INCREMENT_SEQNUM;
                tls13_handle->seqnum_application_tx = 0;
            }
            else
            {
                tls13_handle->seqnum_application_tx++;
            }

            /* Casting uint32_t is used to input. */
            indata_seqnum[0] = change_endian_long(
                    (uint32_t)((tls13_handle->seqnum_application_tx & 0xFFFFFFFF00000000) >> 32));
            /* Casting uint32_t is used to input. */
            indata_seqnum[1] = change_endian_long((uint32_t)(tls13_handle->seqnum_application_tx & 0x00000000FFFFFFFF));

            if (TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_CLIENT == key_index->type)
            {
                indata_datatype = change_endian_long(1);
            }
            else /* TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_SERVER == key_index->type */
            {
                indata_datatype = change_endian_long(3);
            }
        }

        /* Casting uint32_t is used to input. */
        indata_cmd = change_endian_long((uint32_t)tls13_mode);
    }
    else
    {
        indata_keytype = 0; /* For normal */
    }
    return R_TSIP_Aes128GcmEncryptInitSub(&indata_keytype, &indata_datatype, &indata_cmd, key_index->value, InData_IV,
            indata_seqnum);
}
/**************************************
 End of function R_TSIP_Aes128GcmEncryptInitPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128GcmEncryptUpdateAadPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES GCM algorithm with AES 128 key for additional authentication data.
* @param[in]     InData_DataA Input additional authentication data.
* @param[in]     MAX_CNT Input additional authenticated data byte size.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes128GcmEncryptUpdateAADSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128GcmEncryptUpdateAadPrivate(uint32_t *InData_DataA, uint32_t MAX_CNT)
{
    R_TSIP_Aes128GcmEncryptUpdateAADSub(InData_DataA, MAX_CNT);
    return TSIP_SUCCESS;
}
/**************************************
 End of function R_TSIP_Aes128GcmEncryptUpdateAadPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128GcmEncryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES GCM algorithm with AES 128 key.
* @param[in]     InData_Text Input plain text.
* @param[in]     MAX_CNT Input byte size of plain.
* @param[out]    OutData_Text Output cipher text.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes128GcmEncryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128GcmEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT, uint32_t *OutData_Text)
{
    R_TSIP_Aes128GcmEncryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);
    return TSIP_SUCCESS;
}
/**************************************
 End of function R_TSIP_Aes128GcmEncryptUpdatePrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128GcmEncryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES GCM algorithm with AES 128 key.
* @param[in]     InData_Text Input plain text.
* @param[in]     InData_DataALen Input byte size of additional authentication data.
* @param[in]     InData_TextLen Input byte size of plain.
* @param[out]    OutData_Text Output cipher text.
* @param[out]    OutData_DataT Output atag.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128GcmEncryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128GcmEncryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_DataALen,
        uint32_t *InData_TextLen, uint32_t *OutData_Text, uint32_t *OutData_DataT)
{
    return R_TSIP_Aes128GcmEncryptFinalSub(InData_Text, InData_DataALen, InData_TextLen, OutData_Text, OutData_DataT);
}
/**************************************
 End of function R_TSIP_Aes128GcmEncryptFinalPrivate
 **************************************/
#endif /* TSIP_AES_128_GCM_ENCRYPT == 1 */

#if TSIP_AES_128_GCM_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128GcmDecryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES GCM algorithm with AES 128 key.
* @param[in]     key_index User key generation information area.
* @param[in]     tls13_phase handshake phase.
* @param[in]     tls13_mode handshake protocol.
* @param[in]     tls13_handle handler for TLS1.3(work area).
* @param[in]     InData_IV Input initial vector.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128GcmDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128GcmDecryptInitPrivate(tsip_aes_key_index_t *key_index, e_tsip_tls13_phase_t tls13_phase,
        e_tsip_tls13_mode_t tls13_mode, tsip_tls13_handle_t *tls13_handle, uint32_t *InData_IV)
{
    uint32_t indata_keytype;
    uint32_t indata_datatype  = 0;
    uint32_t indata_cmd       = 0;
    uint32_t indata_seqnum[2] = {0, 0};

    if (TSIP_KEY_INDEX_TYPE_AES128_FOR_TLS == key_index->type)
    {
        indata_keytype = change_endian_long(1); /* For TLS */
    }
    else if (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    else if ((TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_CLIENT == key_index->type)
            || (TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_SERVER == key_index->type))
    {
        indata_keytype = change_endian_long(4); /* For TLS1.3 */

        if (TSIP_TLS13_PHASE_HANDSHAKE == tls13_phase)
        {
            if (R_TSIP_TLS13_RESET_SEQNUM == tls13_handle->flag_handshake_rx) /* 1st data reception from server. */
            {
                tls13_handle->flag_handshake_rx   = R_TSIP_TLS13_INCREMENT_SEQNUM;
                tls13_handle->seqnum_handshake_rx = 0;
            }
            else
            {
                tls13_handle->seqnum_handshake_rx++;
            }

            /* Casting uint32_t is used to input. */
            indata_seqnum[0] = change_endian_long(
                    (uint32_t)((tls13_handle->seqnum_handshake_rx & 0xFFFFFFFF00000000) >> 32));
            /* Casting uint32_t is used to input. */
            indata_seqnum[1] = change_endian_long((uint32_t)(tls13_handle->seqnum_handshake_rx & 0x00000000FFFFFFFF));

            if (TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_CLIENT == key_index->type)
            {
                indata_datatype = change_endian_long(0);
            }
            else /* TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_SERVER == key_index->type */
            {
                indata_datatype = change_endian_long(2);
            }
        }
        else /* TSIP_TLS13_PHASE_APPLICATION */
        {
            if (R_TSIP_TLS13_RESET_SEQNUM == tls13_handle->flag_application_rx) /* 1st data reception from server. */
            {
                tls13_handle->flag_application_rx   = R_TSIP_TLS13_INCREMENT_SEQNUM;
                tls13_handle->seqnum_application_rx = 0;
            }
            else
            {
                tls13_handle->seqnum_application_rx++;
            }

            /* Casting uint32_t is used to input. */
            indata_seqnum[0] = change_endian_long(
                    (uint32_t)((tls13_handle->seqnum_application_rx & 0xFFFFFFFF00000000) >> 32));
            /* Casting uint32_t is used to input. */
            indata_seqnum[1] = change_endian_long((uint32_t)(tls13_handle->seqnum_application_rx & 0x00000000FFFFFFFF));

            if (TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_CLIENT == key_index->type)
            {
                indata_datatype = change_endian_long(1);
            }
            else /* TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_SERVER == key_index->type */
            {
                indata_datatype = change_endian_long(3);
            }
        }

        /* Casting uint32_t is used to input. */
        indata_cmd = change_endian_long((uint32_t)tls13_mode);
    }
    else
    {
        indata_keytype = 0; /* For normal */
    }
    return R_TSIP_Aes128GcmDecryptInitSub(&indata_keytype, &indata_datatype, &indata_cmd, key_index->value, InData_IV,
            indata_seqnum);
}
/**************************************
 End of function R_TSIP_Aes128GcmDecryptInitPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128GcmDecryptUpdateAadPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES GCM algorithm with AES 128 key for additional authentication data.
* @param[in]     InData_DataA Input additional authentication data.
* @param[in]     MAX_CNT Input additional authenticated data byte size.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes128GcmDecryptUpdateAADSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128GcmDecryptUpdateAadPrivate(uint32_t *InData_DataA, uint32_t MAX_CNT)
{
    R_TSIP_Aes128GcmDecryptUpdateAADSub(InData_DataA, MAX_CNT);
    return TSIP_SUCCESS;
}
/**************************************
 End of function R_TSIP_Aes128GcmDecryptUpdateAadPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128GcmDecryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES GCM algorithm with AES 128 key.
* @param[in]     InData_Text Input plain text.
* @param[in]     MAX_CNT Input byte size of cipher.
* @param[out]    OutData_Text Output plain text.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes128GcmDecryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128GcmDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT, uint32_t *OutData_Text)
{
    R_TSIP_Aes128GcmDecryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);
    return TSIP_SUCCESS;
}
/**************************************
 End of function R_TSIP_Aes128GcmDecryptUpdatePrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128GcmDecryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES GCM algorithm with AES 128 key.
* @param[in]     InData_Text Input cipher text.
* @param[in]     InData_DataT Input atag.
* @param[in]     InData_DataALen Input byte size of additional authentication data.
* @param[in]     InData_TextLen Input byte size of cipher.
* @param[in]     InData_DataTLen Input byte size of atag.
* @param[out]    OutData_Text Output plain text.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_AUTHENTICATION: Authentication NG
* @see           R_TSIP_Aes128GcmDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128GcmDecryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataALen, uint32_t *InData_TextLen, uint32_t *InData_DataTLen, uint32_t *OutData_Text)
{
    return R_TSIP_Aes128GcmDecryptFinalSub(InData_Text, InData_DataT, InData_DataALen, InData_TextLen, InData_DataTLen,
            OutData_Text);
}
/**************************************
 End of function R_TSIP_Aes128GcmDecryptFinalPrivate
 **************************************/
#endif /* TSIP_AES_128_GCM_DECRYPT == 1 */

#if TSIP_AES_256_GCM_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256GcmEncryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES GCM algorithm with AES 256 key.
* @param[in]     key_index User key generation information area.
* @param[in]     InData_IV Input initial vector.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256GcmEncryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256GcmEncryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV)
{
    uint32_t indata_keytype = 0; /* For normal */

    if (TSIP_KEY_INDEX_TYPE_AES256_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    return R_TSIP_Aes256GcmEncryptInitSub(&indata_keytype, key_index->value, InData_IV);
}
/**************************************
 End of function R_TSIP_Aes256GcmEncryptInitPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256GcmEncryptUpdateAadPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES GCM algorithm with AES 256 key for additional authentication data.
* @param[in]     InData_DataA Input additional authentication data.
* @param[in]     MAX_CNT Input additional authenticated data byte size.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes256GcmEncryptUpdateAADSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256GcmEncryptUpdateAadPrivate(uint32_t *InData_DataA, uint32_t MAX_CNT)
{
    R_TSIP_Aes256GcmEncryptUpdateAADSub(InData_DataA, MAX_CNT);
    return TSIP_SUCCESS;
}
/**************************************
 End of function R_TSIP_Aes256GcmEncryptUpdateAadPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256GcmEncryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES GCM algorithm with AES 256 key.
* @param[in]     InData_Text Input plain text.
* @param[in]     MAX_CNT Input byte size of plain.
* @param[out]    OutData_Text Output cipher text.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes256GcmEncryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256GcmEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT, uint32_t *OutData_Text)
{
    R_TSIP_Aes256GcmEncryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);
    return TSIP_SUCCESS;
}
/**************************************
 End of function R_TSIP_Aes256GcmEncryptUpdatePrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256GcmEncryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES GCM algorithm with AES 256 key.
* @param[in]     InData_Text Input plain text.
* @param[in]     InData_DataALen Input byte size of additional authentication data.
* @param[in]     InData_TextLen Input byte size of plain.
* @param[out]    OutData_Text Output cipher text.
* @param[out]    OutData_DataT Output atag.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256GcmEncryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256GcmEncryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_DataALen,
        uint32_t *InData_TextLen, uint32_t *OutData_Text, uint32_t *OutData_DataT)
{
    return R_TSIP_Aes256GcmEncryptFinalSub(InData_Text, InData_DataALen, InData_TextLen, OutData_Text, OutData_DataT);
}
/**************************************
 End of function R_TSIP_Aes256GcmEncryptFinalPrivate
 **************************************/
#endif /* TSIP_AES_256_GCM_ENCRYPT == 1 */

#if TSIP_AES_256_GCM_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256GcmDecryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES GCM algorithm with AES 256 key.
* @param[in]     key_index User key generation information area.
* @param[in]     InData_IV Input initial vector.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256GcmDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256GcmDecryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV)
{
    uint32_t indata_keytype = 0; /* For normal */

    if (TSIP_KEY_INDEX_TYPE_AES256_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    return R_TSIP_Aes256GcmDecryptInitSub(&indata_keytype, key_index->value, InData_IV);
}
/**************************************
 End of function R_TSIP_Aes256GcmDecryptInitPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256GcmDecryptUpdateAadPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES GCM algorithm with AES 256 key for additional authentication data.
* @param[in]     InData_DataA Input additional authentication data.
* @param[in]     MAX_CNT Input additional authenticated data byte size.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes256GcmDecryptUpdateAADSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256GcmDecryptUpdateAadPrivate(uint32_t *InData_DataA, uint32_t MAX_CNT)
{
    R_TSIP_Aes256GcmDecryptUpdateAADSub(InData_DataA, MAX_CNT);
    return TSIP_SUCCESS;
}
/**************************************
 End of function R_TSIP_Aes256GcmDecryptUpdateAadPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256GcmDecryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES GCM algorithm with AES 256 key.
* @param[in]     InData_Text Input plain text.
* @param[in]     MAX_CNT Input byte size of cipher.
* @param[out]    OutData_Text Output plain text.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes256GcmDecryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256GcmDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT, uint32_t *OutData_Text)
{
    R_TSIP_Aes256GcmDecryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);
    return TSIP_SUCCESS;
}
/**************************************
 End of function R_TSIP_Aes256GcmDecryptUpdatePrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256GcmDecryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES GCM algorithm with AES 256 key.
* @param[in]     InData_Text Input cipher text.
* @param[in]     InData_DataT Input atag.
* @param[in]     InData_DataALen Input byte size of additional authentication data.
* @param[in]     InData_TextLen Input byte size of cipher.
* @param[in]     InData_DataTLen Input byte size of atag.
* @param[out]    OutData_Text Output plain text.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_AUTHENTICATION: Authentication NG
* @see           R_TSIP_Aes256GcmDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256GcmDecryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataALen, uint32_t *InData_TextLen, uint32_t *InData_DataTLen, uint32_t *OutData_Text)
{
    return R_TSIP_Aes256GcmDecryptFinalSub(InData_Text, InData_DataT, InData_DataALen, InData_TextLen, InData_DataTLen,
            OutData_Text);
}
/**************************************
 End of function R_TSIP_Aes256GcmDecryptFinalPrivate
 **************************************/
#endif /* TSIP_AES_256_GCM_DECRYPT == 1 */

#if TSIP_AES_128_GCM_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128GcmEncryptUpdateTransitionPrivate
*******************************************************************************************************************/ /**
* @details       Transition TSIP mode for encrypt data using AES128 GCM algorithm.
* @see           R_TSIP_Aes128GcmEncryptUpdateTransitionSub()
* @note          None
*/
void R_TSIP_Aes128GcmEncryptUpdateTransitionPrivate(void)
{
    R_TSIP_Aes128GcmEncryptUpdateTransitionSub();
    return;
}
/**************************************
 End of function R_TSIP_Aes128GcmEncryptUpdateTransitionPrivate
 **************************************/
#endif /* TSIP_AES_128_GCM_ENCRYPT == 1 */

#if TSIP_AES_128_GCM_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128GcmDecryptUpdateTransitionPrivate
*******************************************************************************************************************/ /**
* @details       Transition TSIP mode for decrypt data using AES128 GCM algorithm.
* @see           R_TSIP_Aes128GcmDecryptUpdateTransitionSub()
* @note          None
*/
void R_TSIP_Aes128GcmDecryptUpdateTransitionPrivate(void)
{
    R_TSIP_Aes128GcmDecryptUpdateTransitionSub();
    return;
}
/**************************************
 End of function R_TSIP_Aes128GcmDecryptUpdateTransitionPrivate
 **************************************/
#endif /* TSIP_AES_128_GCM_DECRYPT == 1 */

#if TSIP_AES_256_GCM_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256GcmEncryptUpdateTransitionPrivate
*******************************************************************************************************************/ /**
* @details       Transition TSIP mode for encrypt data using AES256 GCM algorithm.
* @see           R_TSIP_Aes256GcmEncryptUpdateTransitionSub()
* @note          None
*/
void R_TSIP_Aes256GcmEncryptUpdateTransitionPrivate(void)
{
    R_TSIP_Aes256GcmEncryptUpdateTransitionSub();
    return;
}
/**************************************
 End of function R_TSIP_Aes256GcmEncryptUpdateTransitionPrivate
 **************************************/
#endif /* TSIP_AES_256_GCM_ENCRYPT == 1 */

#if TSIP_AES_256_GCM_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256GcmDecryptUpdateTransitionPrivate
*******************************************************************************************************************/ /**
* @details       Transition TSIP mode for decrypt data using AES256 GCM algorithm.
* @see           R_TSIP_Aes256GcmDecryptUpdateTransitionSub()
* @note          None
*/
void R_TSIP_Aes256GcmDecryptUpdateTransitionPrivate(void)
{
    R_TSIP_Aes256GcmDecryptUpdateTransitionSub();
    return;
}
/**************************************
 End of function R_TSIP_Aes256GcmDecryptUpdateTransitionPrivate
 **************************************/
#endif /* TSIP_AES_256_GCM_DECRYPT == 1 */

#if TSIP_AES_128_CCM_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CcmEncryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CCM algorithm with AES 128 key.
* @param[in]     key_index User key generation information area.
* @param[in]     tls13_phase handshake phase.
* @param[in]     tls13_mode handshake protocol.
* @param[in]     tls13_handle handler for TLS1.3(work area).
* @param[in]     InData_TextLen Input byte size of plain.
* @param[in]     InData_IV Counter.
* @param[in]     InData_Header Formatted data area.
* @param[in]     Header_Len Formatted data length.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128CcmEncryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CcmEncryptInitPrivate(tsip_aes_key_index_t *key_index, e_tsip_tls13_phase_t tls13_phase,
        e_tsip_tls13_mode_t tls13_mode, tsip_tls13_handle_t *tls13_handle, uint32_t InData_TextLen,
        uint32_t *InData_IV, uint32_t *InData_Header, uint32_t Header_Len)
{
    uint32_t indata_keytype;
    uint32_t indata_datatype  = 0;
    uint32_t indata_cmd       = 0;
    uint32_t indata_textlen   = 0;
    uint32_t indata_seqnum[2] = {0, 0};

    if (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    else if ((TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_CLIENT == key_index->type)
            || (TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_SERVER == key_index->type))
    {
        indata_keytype = change_endian_long(4); /* For TLS1.3 */

        if (TSIP_TLS13_PHASE_HANDSHAKE == tls13_phase)
        {
            if (R_TSIP_TLS13_RESET_SEQNUM == tls13_handle->flag_handshake_tx) /* 1st data transmission to server. */
            {
                tls13_handle->flag_handshake_tx   = R_TSIP_TLS13_INCREMENT_SEQNUM;
                tls13_handle->seqnum_handshake_tx = 0;
            }
            else
            {
                tls13_handle->seqnum_handshake_tx++;
            }

            /* Casting uint32_t is used to input. */
            indata_seqnum[0] = change_endian_long(
                    (uint32_t)((tls13_handle->seqnum_handshake_tx & 0xFFFFFFFF00000000) >> 32));
            /* Casting uint32_t is used to input. */
            indata_seqnum[1] = change_endian_long((uint32_t)(tls13_handle->seqnum_handshake_tx & 0x00000000FFFFFFFF));

            if (TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_CLIENT == key_index->type)
            {
                indata_datatype = change_endian_long(0);
            }
            else /* TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_SERVER == key_index->type */
            {
                indata_datatype = change_endian_long(2);
            }
        }
        else /* TSIP_TLS13_PHASE_APPLICATION */
        {
            if (R_TSIP_TLS13_RESET_SEQNUM == tls13_handle->flag_application_tx) /* 1st data transmission to server. */
            {
                tls13_handle->flag_application_tx   = R_TSIP_TLS13_INCREMENT_SEQNUM;
                tls13_handle->seqnum_application_tx = 0;
            }
            else
            {
                tls13_handle->seqnum_application_tx++;
            }

            /* Casting uint32_t is used to input. */
            indata_seqnum[0] = change_endian_long(
                    (uint32_t)((tls13_handle->seqnum_application_tx & 0xFFFFFFFF00000000) >> 32));
            /* Casting uint32_t is used to input. */
            indata_seqnum[1] = change_endian_long((uint32_t)(tls13_handle->seqnum_application_tx & 0x00000000FFFFFFFF));

            if (TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_CLIENT == key_index->type)
            {
                indata_datatype = change_endian_long(1);
            }
            else /* TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_SERVER == key_index->type */
            {
                indata_datatype = change_endian_long(3);
            }
        }

        /* Casting uint32_t is used to input. */
        indata_cmd = change_endian_long((uint32_t)tls13_mode);
    }
    else
    {
        indata_keytype = 0; /* For normal */
    }

    indata_textlen = change_endian_long(InData_TextLen);

    return R_TSIP_Aes128CcmEncryptInitSub(&indata_keytype, &indata_datatype, &indata_cmd, &indata_textlen,
            key_index->value, InData_IV, InData_Header, Header_Len, indata_seqnum);
}
/**************************************
 End of function R_TSIP_Aes128CcmEncryptInitPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CcmEncryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CCM algorithm with AES 128 key.
* @param[in]     InData_Text Input plain text.
* @param[out]    OutData_Text Output cipher text.
* @param[in]     MAX_CNT Number of words to encrypt.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes128CcmEncryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CcmEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
{
    R_TSIP_Aes128CcmEncryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);
    return TSIP_SUCCESS;
}
/**************************************
 End of function R_TSIP_Aes128CcmEncryptUpdatePrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CcmEncryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CCM algorithm with AES 128 key.
* @param[in]     InData_Text Input plain text.
* @param[out]    OutData_Text Output cipher text.
* @param[out]    OutData_DataT Output MAC data.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128CcmEncryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CcmEncryptFinalPrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t *OutData_MAC)
{
    return R_TSIP_Aes128CcmEncryptFinalSub(InData_Text, OutData_Text, OutData_MAC);
}
/**************************************
 End of function R_TSIP_Aes128CcmEncryptFinalPrivate
 **************************************/
#endif /* TSIP_AES_128_CCM_ENCRYPT == 1 */

#if TSIP_AES_128_CCM_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CcmDecryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES CCM algorithm with AES 128 key.
* @param[in]     key_index User key generation information area.
* @param[in]     tls13_phase handshake phase.
* @param[in]     tls13_mode handshake protocol.
* @param[in]     tls13_handle handler for TLS1.3(work area).
* @param[in]     InData_TextLen Input byte size of cipher.
* @param[in]     InData_MACLength MAC data length(byte).
* @param[in]     InData_IV Counter.
* @param[in]     InData_Header Formatted data area.
* @param[in]     Header_Len Formatted data length.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128CcmDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CcmDecryptInitPrivate(tsip_aes_key_index_t *key_index, e_tsip_tls13_phase_t tls13_phase,
        e_tsip_tls13_mode_t tls13_mode, tsip_tls13_handle_t *tls13_handle, uint32_t InData_TextLen,
        uint32_t InData_MACLength, uint32_t *InData_IV, uint32_t *InData_Header, uint32_t Header_Len)
{
    uint32_t indata_keytype;
    uint32_t indata_datatype  = 0;
    uint32_t indata_cmd       = 0;
    uint32_t indata_textlen   = 0;
    uint32_t indata_maclength = 0;
    uint32_t indata_seqnum[2] = {0, 0};

    if (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    else if ((TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_CLIENT == key_index->type)
            || (TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_SERVER == key_index->type))
    {
        indata_keytype = change_endian_long(4); /* For TLS1.3 */

        if (TSIP_TLS13_PHASE_HANDSHAKE == tls13_phase)
        {
            if (R_TSIP_TLS13_RESET_SEQNUM == tls13_handle->flag_handshake_rx) /* 1st data reception from server. */
            {
                tls13_handle->flag_handshake_rx   = R_TSIP_TLS13_INCREMENT_SEQNUM;
                tls13_handle->seqnum_handshake_rx = 0;
            }
            else
            {
                tls13_handle->seqnum_handshake_rx++;
            }

            /* Casting uint32_t is used to input. */
            indata_seqnum[0] = change_endian_long(
                    (uint32_t)((tls13_handle->seqnum_handshake_rx & 0xFFFFFFFF00000000) >> 32));
            /* Casting uint32_t is used to input. */
            indata_seqnum[1] = change_endian_long((uint32_t)(tls13_handle->seqnum_handshake_rx & 0x00000000FFFFFFFF));

            if (TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_CLIENT == key_index->type)
            {
                indata_datatype = change_endian_long(0);
            }
            else /* TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_SERVER == key_index->type */
            {
                indata_datatype = change_endian_long(2);
            }
        }
        else /* TSIP_TLS13_PHASE_APPLICATION */
        {
            if (R_TSIP_TLS13_RESET_SEQNUM == tls13_handle->flag_application_rx) /* 1st data reception from server. */
            {
                tls13_handle->flag_application_rx   = R_TSIP_TLS13_INCREMENT_SEQNUM;
                tls13_handle->seqnum_application_rx = 0;
            }
            else
            {
                tls13_handle->seqnum_application_rx++;
            }

            /* Casting uint32_t is used to input. */
            indata_seqnum[0] = change_endian_long(
                    (uint32_t)((tls13_handle->seqnum_application_rx & 0xFFFFFFFF00000000) >> 32));
            /* Casting uint32_t is used to input. */
            indata_seqnum[1] = change_endian_long((uint32_t)(tls13_handle->seqnum_application_rx & 0x00000000FFFFFFFF));

            if (TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_CLIENT == key_index->type)
            {
                indata_datatype = change_endian_long(1);
            }
            else /* TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_SERVER == key_index->type */
            {
                indata_datatype = change_endian_long(3);
            }
        }

        /* Casting uint32_t is used to input. */
        indata_cmd = change_endian_long((uint32_t)tls13_mode);
    }
    else
    {
        indata_keytype = 0; /* For normal */
    }

    indata_textlen   = change_endian_long(InData_TextLen);
    indata_maclength = change_endian_long(InData_MACLength);

    return R_TSIP_Aes128CcmDecryptInitSub(&indata_keytype, &indata_datatype, &indata_cmd, &indata_textlen,
            &indata_maclength, key_index->value, InData_IV, InData_Header, Header_Len, indata_seqnum);
}
/**************************************
 End of function R_TSIP_Aes128CcmDecryptInitPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CcmDecryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES CCM algorithm with AES 128 key.
* @param[in]     InData_Text Input cipher text.
* @param[out]    OutData_Text Output plain text.
* @param[in]     MAX_CNT Number of words to encrypt.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes128CcmDecryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CcmDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
{
    R_TSIP_Aes128CcmDecryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);
    return TSIP_SUCCESS;
}
/**************************************
 End of function R_TSIP_Aes128CcmDecryptUpdatePrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CcmDecryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES CCM algorithm with AES 128 key.
* @param[in]     InData_Text Input cipher text.
* @param[in]     InData_MAC Input MAC data.
* @param[out]    OutData_Text Output plain text.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128CcmDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CcmDecryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_MAC, uint32_t *OutData_Text)
{
    return R_TSIP_Aes128CcmDecryptFinalSub(InData_Text, InData_MAC, OutData_Text);
}
/**************************************
 End of function R_TSIP_Aes128CcmDecryptFinalPrivate
 **************************************/
#endif /* TSIP_AES_128_CCM_DECRYPT == 1 */

#if TSIP_AES_256_CCM_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CcmEncryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CCM algorithm with AES 256 key.
* @param[in]     key_index User key generation information area.
* @param[in]     InData_IV Counter.
* @param[in]     InData_Header Formatted data area.
* @param[in]     Header_Len Formatted data length.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256CcmEncryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CcmEncryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV,
        uint32_t *InData_Header, uint32_t Header_Len)
{
    uint32_t indata_keytype = 0; /* For normal */

    if (TSIP_KEY_INDEX_TYPE_AES256_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    return R_TSIP_Aes256CcmEncryptInitSub(&indata_keytype, key_index->value, InData_IV, InData_Header, Header_Len);
}
/**************************************
 End of function R_TSIP_Aes256CcmEncryptInitPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CcmEncryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CCM algorithm with AES 256 key.
* @param[in]     InData_Text Input plain text.
* @param[out]    OutData_Text Output cipher text.
* @param[in]     MAX_CNT Number of words to encrypt.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes256CcmEncryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CcmEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
{
    R_TSIP_Aes256CcmEncryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);
    return TSIP_SUCCESS;
}
/**************************************
 End of function R_TSIP_Aes256CcmEncryptUpdatePrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CcmEncryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CCM algorithm with AES 256 key.
* @param[in]     InData_TextLen Input byte size of plain.
* @param[in]     InData_Text Input plain text.
* @param[out]    OutData_Text Output cipher text.
* @param[out]    OutData_DataT Output MAC data.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256CcmEncryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CcmEncryptFinalPrivate(uint32_t *InData_TextLen, uint32_t *InData_Text,
        uint32_t *OutData_Text, uint32_t *OutData_MAC)
{
    return R_TSIP_Aes256CcmEncryptFinalSub(InData_TextLen, InData_Text, OutData_Text, OutData_MAC);
}
/**************************************
 End of function R_TSIP_Aes256CcmEncryptFinalPrivate
 **************************************/
#endif /* TSIP_AES_256_CCM_ENCRYPT == 1 */

#if TSIP_AES_256_CCM_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CcmDecryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES CCM algorithm with AES 256 key.
* @param[in]     key_index User key generation information area.
* @param[in]     InData_IV Counter.
* @param[in]     InData_Header Formatted data area.
* @param[in]     Header_Len Formatted data length.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256CcmDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CcmDecryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV,
        uint32_t *InData_Header, uint32_t Header_Len)
{
    uint32_t indata_keytype = 0; /* For normal */

    if (TSIP_KEY_INDEX_TYPE_AES256_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    return R_TSIP_Aes256CcmDecryptInitSub(&indata_keytype, key_index->value, InData_IV, InData_Header, Header_Len);
}
/**************************************
 End of function R_TSIP_Aes256CcmDecryptInitPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CcmDecryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES CCM algorithm with AES 256 key.
* @param[in]     InData_Text Input cipher text.
* @param[out]    OutData_Text Output plain text.
* @param[in]     MAX_CNT Number of words to encrypt.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes256CcmDecryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CcmDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
{
    R_TSIP_Aes256CcmDecryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);
    return TSIP_SUCCESS;
}
/**************************************
 End of function R_TSIP_Aes256CcmDecryptUpdatePrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CcmDecryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using AES CCM algorithm with AES 256 key.
* @param[in]     InData_Text Input cipher text.
* @param[in]     InData_TextLen Input byte size of cipher.
* @param[in]     InData_MAC Input MAC data.
* @param[in]     InData_MACLength Input byte size of MAC data.
* @param[out]    OutData_Text Output plain text.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256CcmDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CcmDecryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_TextLen, uint32_t *InData_MAC,
        uint32_t *InData_MACLength, uint32_t *OutData_Text)
{
    return R_TSIP_Aes256CcmDecryptFinalSub(InData_Text, InData_TextLen, InData_MAC, InData_MACLength, OutData_Text);
}
/**************************************
 End of function R_TSIP_Aes256CcmDecryptFinalPrivate
 **************************************/
#endif /* TSIP_AES_256_CCM_DECRYPT == 1 */


#if TSIP_AES_128_CMAC == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CmacGenerateInitPrivate
*******************************************************************************************************************/ /**
* @details       Prepare to execute CMAC calculation.
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128CmacInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CmacGenerateInitPrivate(tsip_aes_key_index_t *key_index)
{
    uint32_t indata_keytype = 0; /* For normal */

    if (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    return R_TSIP_Aes128CmacInitSub(&indata_keytype, key_index->value);
}
/****************************************
 End of function R_TSIP_Aes128CmacGenerateInitPrivate
 ****************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CmacGenerateUpdatePrivate
*******************************************************************************************************************/ /**
* @details       generates a MAC value from the message specified.
* @param[in]     InData_Text Message data area (data_len byte)
* @param[in]     MAX_CNT Message data length (0 to arbitrary byte)
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes128CmacUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CmacGenerateUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT)
{
    R_TSIP_Aes128CmacUpdateSub(InData_Text, MAX_CNT);
    return TSIP_SUCCESS;
}
/****************************************
 End of function R_TSIP_Aes128CmacGenerateUpdatePrivate
 ****************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CmacGenerateFinalPrivate
*******************************************************************************************************************/ /**
* @details       outputs the Mac value.
* @param[in]     All_Msg_Len Input byte size of message data.
* @param[in]     InData_Text Input message data.
* @param[out]    OutData_DataT MAC data area (data_len byte)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128CmacFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CmacGenerateFinalPrivate(uint32_t All_Msg_Len, uint32_t *InData_Text,
        uint32_t *OutData_DataT)
{
    uint32_t indata_cmd = 0;
    uint32_t indata_datat[4] = /* dummy */
    {
        0
    };
    uint32_t indata_datat_len = 0; /* dummy */

    if ((0 == (All_Msg_Len % 16)) && (0 != All_Msg_Len))
    {
        indata_cmd = 0;
    }
    else
    {
        indata_cmd = 1;
    }
    return R_TSIP_Aes128CmacFinalSub(&indata_cmd, InData_Text, indata_datat, &indata_datat_len, OutData_DataT);
}
/****************************************
 End of function R_TSIP_Aes128CmacGenerateFinalPrivate
 ****************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CmacVerifyInitPrivate
*******************************************************************************************************************/ /**
* @details       prepare to execute CMAC calculation.
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128CmacInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CmacVerifyInitPrivate(tsip_aes_key_index_t *key_index)
{
    uint32_t indata_keytype = 0; /* For normal */

    if (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    return R_TSIP_Aes128CmacInitSub(&indata_keytype, key_index->value);
}
/****************************************
 End of function R_TSIP_Aes128CmacVerifyInitPrivate
 ****************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CmacVerifyUpdatePrivate
*******************************************************************************************************************/ /**
* @details       veryfies the MAC value.
* @param[in]     InData_Text Message data area (data_len byte)
* @param[in]     MAX_CNT Message data length (0 to arbitrary byte)
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes128CmacUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CmacVerifyUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT)
{
    R_TSIP_Aes128CmacUpdateSub(InData_Text, MAX_CNT);
    return TSIP_SUCCESS;
}
/****************************************
 End of function R_TSIP_Aes128CmacVerifyUpdatePrivate
 ****************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CmacVerifyFinalPrivate
*******************************************************************************************************************/ /**
* @details       validates the Mac value.
* @param[in]     All_Msg_Len Input byte size of message data.
* @param[in]     InData_Text Input message data.
* @param[in]     InData_DataT MAC data area (data_len byte)
* @param[in]     InData_DataTLen MAC data length
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_AUTHENTICATION: Authentication NG
* @see           R_TSIP_Aes128CmacFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CmacVerifyFinalPrivate(uint32_t All_Msg_Len, uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataTLen)
{
    uint32_t indata_cmd = 0;
    uint32_t out_data[4] =
    {
        0
    };

    if ((0 == (All_Msg_Len % 16)) && (0 != All_Msg_Len))
    {
        indata_cmd = 2;
    }
    else
    {
        indata_cmd = 3;
    }
    return R_TSIP_Aes128CmacFinalSub(&indata_cmd, InData_Text, InData_DataT, InData_DataTLen, out_data);
}
/****************************************
 End of function R_TSIP_Aes128CmacVerifyFinalPrivate
 ****************************************/
#endif /* TSIP_AES_128_CMAC == 1 */

#if TSIP_AES_256_CMAC == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CmacGenerateInitPrivate
*******************************************************************************************************************/ /**
* @details       Prepare to execute CMAC calculation.
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256CmacInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CmacGenerateInitPrivate(tsip_aes_key_index_t *key_index)
{
    uint32_t indata_keytype = 0; /* For normal */

    if (TSIP_KEY_INDEX_TYPE_AES256_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    return R_TSIP_Aes256CmacInitSub(&indata_keytype, key_index->value);
}
/****************************************
 End of function R_TSIP_Aes256CmacGenerateInitPrivate
 ****************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CmacGenerateUpdatePrivate
*******************************************************************************************************************/ /**
* @details       generates a MAC value from the message specified.
* @param[in]     InData_Text Message data area (data_len byte)
* @param[in]     MAX_CNT Message data length (0 to arbitrary byte)
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes256CmacUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CmacGenerateUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT)
{
    R_TSIP_Aes256CmacUpdateSub(InData_Text, MAX_CNT);
    return TSIP_SUCCESS;
}
/****************************************
 End of function R_TSIP_Aes256CmacGenerateUpdatePrivate
 ****************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CmacGenerateFinalPrivate
*******************************************************************************************************************/ /**
* @details       outputs the Mac value.
* @param[in]     All_Msg_Len Input byte size of message data.
* @param[in]     InData_Text Input message data.
* @param[out]    OutData_DataT MAC data area (data_len byte)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256CmacFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CmacGenerateFinalPrivate(uint32_t All_Msg_Len, uint32_t *InData_Text,
        uint32_t *OutData_DataT)
{
    uint32_t indata_cmd = 0;
    uint32_t indata_datat[4] = /* dummy */
    {
        0
    };
    uint32_t indata_datat_len = 0; /* dummy */

    if ((0 == (All_Msg_Len % 16)) && (0 != All_Msg_Len))
    {
        indata_cmd = 0;
    }
    else
    {
        indata_cmd = 1;
    }
    return R_TSIP_Aes256CmacFinalSub(&indata_cmd, InData_Text, indata_datat, &indata_datat_len, OutData_DataT);
}
/****************************************
 End of function R_TSIP_Aes256CmacGenerateFinalPrivate
 ****************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CmacVerifyInitPrivate
*******************************************************************************************************************/ /**
* @details       prepare to execute CMAC calculation.
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes256CmacInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CmacVerifyInitPrivate(tsip_aes_key_index_t *key_index)
{
    uint32_t indata_keytype = 0; /* For normal */

    if (TSIP_KEY_INDEX_TYPE_AES256_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    return R_TSIP_Aes256CmacInitSub(&indata_keytype, key_index->value);
}
/****************************************
 End of function R_TSIP_Aes256CmacVerifyInitPrivate
 ****************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CmacVerifyUpdatePrivate
*******************************************************************************************************************/ /**
* @details       veryfies the MAC value.
* @param[in]     InData_Text Message data area (data_len byte)
* @param[in]     MAX_CNT Message data length (0 to arbitrary byte)
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Aes256CmacUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CmacVerifyUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT)
{
    R_TSIP_Aes256CmacUpdateSub(InData_Text, MAX_CNT);
    return TSIP_SUCCESS;
}
/****************************************
 End of function R_TSIP_Aes256CmacVerifyUpdatePrivate
 ****************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256CmacVerifyFinalPrivate
*******************************************************************************************************************/ /**
* @details       validates the Mac value.
* @param[in]     All_Msg_Len Input byte size of message data.
* @param[in]     InData_Text Input message data.
* @param[in]     InData_DataT MAC data area (data_len byte)
* @param[in]     InData_DataTLen MAC data length
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_AUTHENTICATION: Authentication NG
* @see           R_TSIP_Aes256CmacFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CmacVerifyFinalPrivate(uint32_t All_Msg_Len, uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataTLen)
{
    uint32_t indata_cmd = 0;
    uint32_t out_data[4] =
    {
        0
    };

    if ((0 == (All_Msg_Len % 16)) && (0 != All_Msg_Len))
    {
        indata_cmd = 2;
    }
    else
    {
        indata_cmd = 3;
    }
    return R_TSIP_Aes256CmacFinalSub(&indata_cmd, InData_Text, InData_DataT, InData_DataTLen, out_data);
}
/****************************************
 End of function R_TSIP_Aes256CmacVerifyFinalPrivate
 **************************************/
#endif /* TSIP_AES_256_CMAC == 1 */

#if TSIP_AES_128_KEY_WRAP == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128KeyWrapPrivate
*******************************************************************************************************************/ /**
* @details       Wrap InData_WrappedKeyIndex with InData_KeyIndex.
* @param[in]     InData_KeyIndex key index used for wrapping
* @param[in]     InData_WrappedKeyType select key to be wrapped
* @param[in]     InData_WrappedKeyIndex key index to be wrapped
* @param[out]    OutData_Text wrapped key
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_AESKeyWrapSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128KeyWrapPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_WrappedKeyType,
        uint32_t *InData_WrappedKeyIndex, uint32_t *OutData_Text)
{
    uint32_t indata_cmd = 0;
    uint32_t key_index_size = 0;
    uint32_t wrapped_key_size = 0;

    indata_cmd = change_endian_long(0);

    if (R_TSIP_KEYWRAP_AES128 == (*InData_WrappedKeyType))
    {
        wrapped_key_size = 6;
        key_index_size = 12;
    }
    else    /* if (R_TSIP_KEYWRAP_AES256 == (*InData_WrappedKeyType)) */
    {
        wrapped_key_size = 10;
        key_index_size = 16;
    }

    *InData_WrappedKeyType = change_endian_long(*InData_WrappedKeyType);

    return R_TSIP_AESKeyWrapSub(&indata_cmd, InData_KeyIndex, key_index_size, InData_WrappedKeyType,
            InData_WrappedKeyIndex, wrapped_key_size, OutData_Text);
}
/**************************************
 End of function R_TSIP_Aes128KeyWrapPrivate
 **************************************/
#endif /* TSIP_AES_128_KEY_WRAP == 1 */

#if TSIP_AES_256_KEY_WRAP == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256KeyWrapPrivate
*******************************************************************************************************************/ /**
* @details       Wrap InData_WrappedKeyIndex with InData_KeyIndex.
* @param[in]     InData_KeyIndex key index used for wrapping
* @param[in]     InData_WrappedKeyType select key to be wrapped
* @param[in]     InData_WrappedKeyIndex key index to be wrapped
* @param[out]    OutData_Text wrapped key
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_AESKeyWrapSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256KeyWrapPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_WrappedKeyType,
        uint32_t *InData_WrappedKeyIndex, uint32_t *OutData_Text)
{
    uint32_t indata_cmd = 0;
    uint32_t key_index_size = 0;
    uint32_t wrapped_key_size = 0;

    indata_cmd = change_endian_long(1);

    if (R_TSIP_KEYWRAP_AES128 == (*InData_WrappedKeyType))
    {
        wrapped_key_size = 6;
        key_index_size = 12;
    }
    else    /* if (R_TSIP_KEYWRAP_AES256 == (*InData_WrappedKeyType)) */
    {
        wrapped_key_size = 10;
        key_index_size = 16;
    }

    *InData_WrappedKeyType = change_endian_long(*InData_WrappedKeyType);

    return R_TSIP_AESKeyWrapSub(&indata_cmd, InData_KeyIndex, key_index_size, InData_WrappedKeyType,
            InData_WrappedKeyIndex, wrapped_key_size, OutData_Text);
}
/**************************************
 End of function R_TSIP_Aes256KeyWrapPrivate
 **************************************/
#endif /* TSIP_AES_256_KEY_WRAP == 1 */

#if TSIP_AES_128_KEY_WRAP == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128KeyUnWrapPrivate
*******************************************************************************************************************/ /**
* @details       Unwrap InData_Text with key_index.
* @param[in]     key_index key index used for unwrapping
* @param[in]     InData_WrappedKeyType select key to be unwrapped
* @param[in]     InData_Text wrapped key
* @param[out]    OutData_KeyIndex key index to be unwrapped
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_AESKeyUnwrapSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128KeyUnWrapPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_WrappedKeyType,
        uint32_t *InData_Text, uint32_t *OutData_KeyIndex)
{
    uint32_t indata_keytype = 0; /* For normal */
    uint32_t indata_cmd = 0;
    uint32_t key_index_size = 0;
    uint32_t wrapped_key_size = 0;

    if (TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    indata_cmd = change_endian_long(0);

    if (R_TSIP_KEYWRAP_AES128 == (*InData_WrappedKeyType))
    {
        wrapped_key_size = 6;
        key_index_size = 12;
    }
    else    /* if (R_TSIP_KEYWRAP_AES256 == (*InData_WrappedKeyType)) */
    {
        wrapped_key_size = 10;
        key_index_size = 16;
    }

    *InData_WrappedKeyType = change_endian_long(*InData_WrappedKeyType);

    return R_TSIP_AESKeyUnwrapSub(&indata_keytype, &indata_cmd, key_index->value, key_index_size,
            InData_WrappedKeyType, InData_Text, wrapped_key_size, OutData_KeyIndex);
}
/**************************************
 End of function R_TSIP_Aes128KeyUnWrapPrivate
 **************************************/
#endif /* TSIP_AES_128_KEY_WRAP == 1 */

#if TSIP_AES_256_KEY_WRAP == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes256KeyUnWrapPrivate
*******************************************************************************************************************/ /**
* @details       Unwrap InData_Text with key_index.
* @param[in]     key_index key index used for unwrapping
* @param[in]     InData_WrappedKeyType select key to be unwrapped
* @param[in]     InData_Text wrapped key
* @param[out]    OutData_KeyIndex key index to be unwrapped
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_AESKeyUnwrapSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256KeyUnWrapPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_WrappedKeyType,
        uint32_t *InData_Text, uint32_t *OutData_KeyIndex)
{
    uint32_t indata_keytype = 0; /* For normal */
    uint32_t indata_cmd = 0;
    uint32_t key_index_size = 0;
    uint32_t wrapped_key_size = 0;

    if (TSIP_KEY_INDEX_TYPE_AES256_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    indata_cmd = change_endian_long(1);

    if (R_TSIP_KEYWRAP_AES128 == (*InData_WrappedKeyType))
    {
        wrapped_key_size = 6;
        key_index_size = 12;
    }
    else    /* if (R_TSIP_KEYWRAP_AES256 == (*InData_WrappedKeyType)) */
    {
        wrapped_key_size = 10;
        key_index_size = 16;
    }

    *InData_WrappedKeyType = change_endian_long(*InData_WrappedKeyType);

    return R_TSIP_AESKeyUnwrapSub(&indata_keytype, &indata_cmd, key_index->value, key_index_size,
            InData_WrappedKeyType, InData_Text, wrapped_key_size, OutData_KeyIndex);
}
/**************************************
 End of function R_TSIP_Aes256KeyUnWrapPrivate
 **************************************/
#endif /* TSIP_AES_256_KEY_WRAP == 1 */

#if TSIP_TDES_ECB_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_TdesEcbEncryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using TDES ECB mode algorithm.
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_TdesEncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TdesEcbEncryptInitPrivate(tsip_tdes_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    uint32_t indata_iv[4] = /* dummy */
    {
        0
    };

    if (TSIP_KEY_INDEX_TYPE_TDES == key_index->type)
    {
        indata_cmd = change_endian_long(0); /* ECB-Encrypt command */
    }
    else
    {
        return TSIP_ERR_KEY_SET;
    }

    return R_TSIP_TdesEncryptDecryptInitSub(&indata_cmd, key_index->value, indata_iv);
}
/*************************************************
 End of function R_TSIP_TdesEcbEncryptInitPrivate
 **************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TdesEcbEncryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using TDES ECB mode algorithm.
* @param[in]     InData_Text Input plain text.
* @param[out]    OutData_Text Output cipher text.
* @param[in]     MAX_CNT Input word size of InData_Text.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_TdesEncryptDecryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TdesEcbEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
{
    R_TSIP_TdesEncryptDecryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);

    return TSIP_SUCCESS;
}
/***************************************************
 End of function R_TSIP_TdesEcbEncryptUpdatePrivate
 ****************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TdesEcbEncryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using TDES ECB mode algorithm.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TdesEncryptDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TdesEcbEncryptFinalPrivate(void)
{
    return R_TSIP_TdesEncryptDecryptFinalSub();
}
/***************************************************
 End of function R_TSIP_TdesEcbEncryptFinalPrivate
 ****************************************************/
#endif  /* TSIP_TDES_ECB_ENCRYPT == 1 */

#if TSIP_TDES_ECB_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_TdesEcbDecryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using TDES ECB mode algorithm.
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_TdesEncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TdesEcbDecryptInitPrivate(tsip_tdes_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    uint32_t indata_iv[4] = /* dummy */
    {
        0
    };

    if (TSIP_KEY_INDEX_TYPE_TDES == key_index->type)
    {
        indata_cmd = change_endian_long(1); /* ECB-Decrypt command */
    }
    else
    {
        return TSIP_ERR_KEY_SET;
    }

    return R_TSIP_TdesEncryptDecryptInitSub(&indata_cmd, key_index->value, indata_iv);
}
/*************************************************
 End of function R_TSIP_TdesEcbDecryptInitPrivate
 **************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TdesEcbDecryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using TDES ECB mode algorithm.
* @param[in]     InData_Text Input cipher text.
* @param[out]    OutData_Text Output plain text.
* @param[in]     MAX_CNT Input word size of InData_Text.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_TdesEncryptDecryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TdesEcbDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
{
    R_TSIP_TdesEncryptDecryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);

    return TSIP_SUCCESS;
}
/***************************************************
 End of function R_TSIP_TdesEcbDecryptUpdatePrivate
 ****************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TdesEcbDecryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using TDES ECB mode algorithm.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TdesEncryptDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TdesEcbDecryptFinalPrivate(void)
{
    return R_TSIP_TdesEncryptDecryptFinalSub();
}
/***************************************************
 End of function R_TSIP_TdesEcbDecryptFinalPrivate
 ****************************************************/
#endif  /* TSIP_TDES_ECB_DECRYPT == 1 */

#if TSIP_TDES_CBC_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_TdesCbcEncryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using TDES CBC mode algorithm.
* @param[in]     key_index User key generation information area.
* @param[in]     InData_IV Input initial vector.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_TdesEncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TdesCbcEncryptInitPrivate(tsip_tdes_key_index_t *key_index, uint32_t *InData_IV)
{
    uint32_t indata_cmd = 0;

    if (TSIP_KEY_INDEX_TYPE_TDES == key_index->type)
    {
        indata_cmd = change_endian_long(2); /* CBC-Encrypt command */
    }
    else
    {
        return TSIP_ERR_KEY_SET;
    }

    return R_TSIP_TdesEncryptDecryptInitSub(&indata_cmd, key_index->value, InData_IV);
}
/*************************************************
 End of function R_TSIP_TdesCbcEncryptInitPrivate
 **************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TdesCbcEncryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using TDES CBC mode algorithm.
* @param[in]     InData_Text Input plain text.
* @param[out]    OutData_Text Output cipher text.
* @param[in]     MAX_CNT Input word size of InData_Text.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_TdesEncryptDecryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TdesCbcEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
{
    R_TSIP_TdesEncryptDecryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);

    return TSIP_SUCCESS;
}
/***************************************************
 End of function R_TSIP_TdesCbcEncryptUpdatePrivate
 ****************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TdesCbcEncryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using TDES CBC mode algorithm.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TdesEncryptDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TdesCbcEncryptFinalPrivate(void)
{
    return R_TSIP_TdesEncryptDecryptFinalSub();
}
/***************************************************
 End of function R_TSIP_TdesCbcEncryptFinalPrivate
 ***************************************************/

#endif  /* TSIP_TDES_CBC_ENCRYPT == 1 */

#if TSIP_TDES_CBC_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_TdesCbcDecryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using TDES CBC mode algorithm.
* @param[in]     key_index User key generation information area.
* @param[in]     InData_IV Input initial vector.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_TdesEncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TdesCbcDecryptInitPrivate(tsip_tdes_key_index_t *key_index, uint32_t *InData_IV)
{
    uint32_t indata_cmd = 0;

    if (TSIP_KEY_INDEX_TYPE_TDES == key_index->type)
    {
        indata_cmd = change_endian_long(3); /* CBC-Decrypt command */
    }
    else
    {
        return TSIP_ERR_KEY_SET;
    }

    return R_TSIP_TdesEncryptDecryptInitSub(&indata_cmd, key_index->value, InData_IV);
}
/*************************************************
 End of function R_TSIP_TdesCbcDecryptInitPrivate
 **************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TdesCbcDecryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using TDES CBC mode algorithm.
* @param[in]     InData_Text Input cipher text.
* @param[out]    OutData_Text Output plain text.
* @param[in]     MAX_CNT Input word size of InData_Text.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_TdesEncryptDecryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TdesCbcDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
{
    R_TSIP_TdesEncryptDecryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);

    return TSIP_SUCCESS;
}
/***************************************************
 End of function R_TSIP_TdesCbcDecryptUpdatePrivate
 ****************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_TdesCbcDecryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to cipher using TDES CBC mode algorithm.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_TdesEncryptDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_TdesCbcDecryptFinalPrivate(void)
{
    return R_TSIP_TdesEncryptDecryptFinalSub();
}
/***************************************************
 End of function R_TSIP_TdesCbcDecryptFinalPrivate
 ***************************************************/
#endif  /* TSIP_TDES_CBC_DECRYPT == 1 */

#if TSIP_ARC4_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Arc4EncryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using ARC4 mode algorithm.
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Arc4EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Arc4EncryptInitPrivate(tsip_arc4_key_index_t *key_index)
{
    return R_TSIP_Arc4EncryptDecryptInitSub(key_index->value);
}
/*************************************************
 End of function R_TSIP_Arc4EncryptInitPrivate
 **************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Arc4EncryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using ARC4 mode algorithm.
* @param[in]     InData_Text Input plain text.
* @param[out]    OutData_Text Output cipher text.
* @param[in]     MAX_CNT Input word size of InData_Text.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Arc4EncryptDecryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Arc4EncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
{
    R_TSIP_Arc4EncryptDecryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);
    return TSIP_SUCCESS;
}
/***************************************************
 End of function R_TSIP_Arc4EncryptUpdatePrivate
 ****************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Arc4EncryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using ARC4 mode algorithm.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Arc4EncryptDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Arc4EncryptFinalPrivate(void)
{
    return R_TSIP_Arc4EncryptDecryptFinalSub();
}
/***************************************************
 End of function R_TSIP_Arc4EncryptFinalPrivate
 ****************************************************/
#endif  /* TSIP_ARC4_ENCRYPT == 1 */

#if TSIP_ARC4_DECRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Arc4DecryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using ARC4 mode algorithm.
* @param[in]     key_index User key generation information area.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Arc4EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Arc4DecryptInitPrivate(tsip_arc4_key_index_t *key_index)
{
    return R_TSIP_Arc4EncryptDecryptInitSub(key_index->value);
}
/*************************************************
 End of function R_TSIP_Arc4DecryptInitPrivate
 **************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Arc4DecryptUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using ARC4 mode algorithm.
* @param[in]     InData_Text Input cipher text.
* @param[out]    OutData_Text Output plain text.
* @param[in]     MAX_CNT Input word size of InData_Text.
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Arc4EncryptDecryptUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Arc4DecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
{
    R_TSIP_Arc4EncryptDecryptUpdateSub(InData_Text, OutData_Text, MAX_CNT);
    return TSIP_SUCCESS;
}
/***************************************************
 End of function R_TSIP_Arc4DecryptUpdatePrivate
 ****************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Arc4DecryptFinalPrivate
*******************************************************************************************************************/ /**
* @details       Decrypt data to plain using ARC4 mode algorithm.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Arc4EncryptDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Arc4DecryptFinalPrivate(void)
{
    return R_TSIP_Arc4EncryptDecryptFinalSub();
}
/***************************************************
 End of function R_TSIP_Arc4DecryptFinalPrivate
 ****************************************************/
#endif  /* TSIP_ARC4_DECRYPT == 1 */

#if TSIP_SHA_1 == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Sha1InitPrivate
*******************************************************************************************************************/ /**
* @details       prepares to execute the SHA1 hash operation
* @param[out]    handle SHA1 handler (work area)
* @retval        TSIP_SUCCESS: Normal termination.
* @note          None
*/
TSIP_SEC_P_SECURE_BOOT
e_tsip_err_t R_TSIP_Sha1InitPrivate(tsip_sha_md5_handle_t *handle)
{
    uint32_t sha_initval[5] =
    {
        0
    };

    sha_initval[0] = change_endian_long(0x67452301UL);
    sha_initval[1] = change_endian_long(0xefcdab89UL);
    sha_initval[2] = change_endian_long(0x98badcfeUL);
    sha_initval[3] = change_endian_long(0x10325476UL);
    sha_initval[4] = change_endian_long(0xc3d2e1f0UL);
    memcpy(handle->current_hash, sha_initval, R_TSIP_SHA1_HASH_LENGTH_BYTE_SIZE);

    return TSIP_SUCCESS;
}
/**************************************
 End of function R_TSIP_Sha1InitPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha1UpdatePrivate
*******************************************************************************************************************/ /**
* @details       calculate a hash value
* @param[in,out] handle SHA1 handler (work area).
* @param[in]     InData_PaddingMsg Input message.
* @param[in]     MAX_CNT Input message length
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @see           R_TSIP_Sha1GenerateMessageDigestSub()
* @note          None
*/
TSIP_SEC_P_SECURE_BOOT
e_tsip_err_t R_TSIP_Sha1UpdatePrivate(tsip_sha_md5_handle_t *handle, uint32_t *InData_PaddedMsg, uint32_t MAX_CNT)
{
    uint32_t out_data[5] =
    {
        0
    };
    e_tsip_err_t ercd = TSIP_SUCCESS;

    /*Casting uint32_t pointer is used for address.*/
    ercd = R_TSIP_Sha1GenerateMessageDigestSub((uint32_t *) handle->current_hash, InData_PaddedMsg, MAX_CNT, out_data);
    memcpy(handle->current_hash, out_data, R_TSIP_SHA1_HASH_LENGTH_BYTE_SIZE);

    return ercd;
}
/**************************************
 End of function R_TSIP_Sha1UpdatePrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha1FinalPrivate
*******************************************************************************************************************/ /**
* @details       calculate a hash value
* @param[in,out] handle SHA1 handler (work area).
* @param[in]     MAX_CNT Input message length
* @param[out]    OutData_MsgDigest Output Hash data
* @param[out]    OutData_Length Output Hash length
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @see           R_TSIP_Sha1GenerateMessageDigestSub()
* @note          None
*/
TSIP_SEC_P_SECURE_BOOT
e_tsip_err_t R_TSIP_Sha1FinalPrivate(tsip_sha_md5_handle_t *handle, uint32_t MAX_CNT,
        uint32_t *OutData_MsgDigest, uint32_t *OutData_Length)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;

    /* Casting uint32_t pointer is used for address. */
    ercd = R_TSIP_Sha1GenerateMessageDigestSub((uint32_t*)handle->current_hash, (uint32_t*)handle->sha_buffer,
            MAX_CNT, OutData_MsgDigest);
    memcpy(handle->current_hash, OutData_MsgDigest, R_TSIP_SHA1_HASH_LENGTH_BYTE_SIZE);
    *OutData_Length = R_TSIP_SHA1_HASH_LENGTH_BYTE_SIZE;
    return ercd;
}
/**************************************
 End of function R_TSIP_Sha1FinalPrivate
 **************************************/
#endif /* TSIP_SHA_1 == 1 */
TSIP_SEC_DEFAULT

#if TSIP_SHA_256 == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Sha256InitPrivate
*******************************************************************************************************************/ /**
* @details       prepares to execute the SHA256 hash operation
* @param[out]    handle SHA256 handler (work area)
* @retval        TSIP_SUCCESS: Normal termination.
* @note          None
*/
e_tsip_err_t R_TSIP_Sha256InitPrivate(tsip_sha_md5_handle_t *handle)
{
    uint32_t indata_sha256initval[8] =
    {
        0
    };

    indata_sha256initval[0] = change_endian_long(0x6a09e667UL);
    indata_sha256initval[1] = change_endian_long(0xbb67ae85UL);
    indata_sha256initval[2] = change_endian_long(0x3c6ef372UL);
    indata_sha256initval[3] = change_endian_long(0xa54ff53aUL);
    indata_sha256initval[4] = change_endian_long(0x510e527fUL);
    indata_sha256initval[5] = change_endian_long(0x9b05688cUL);
    indata_sha256initval[6] = change_endian_long(0x1f83d9abUL);
    indata_sha256initval[7] = change_endian_long(0x5be0cd19UL);

    memcpy(handle->current_hash, indata_sha256initval, R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE);

    return TSIP_SUCCESS;
}
/**************************************
 End of function R_TSIP_Sha256InitPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha256UpdatePrivate
*******************************************************************************************************************/ /**
* @details       calculate a hash value
* @param[in,out] handle SHA256 handler (work area).
* @param[in]     InData_PaddingMsg Input message.
* @param[in]     MAX_CNT Input message length
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @see           R_TSIP_Sha224256GenerateMessageDigestSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha256UpdatePrivate(tsip_sha_md5_handle_t *handle, uint32_t *InData_PaddedMsg, uint32_t MAX_CNT)
{
    uint32_t out_data[8] =
    {
        0
    };
    e_tsip_err_t ercd = TSIP_SUCCESS;

    /*Casting uint32_t pointer is used for address.*/
    ercd = R_TSIP_Sha224256GenerateMessageDigestSub((uint32_t *) handle->current_hash, InData_PaddedMsg, MAX_CNT,
            out_data);
    memcpy(handle->current_hash, out_data, R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE);

    return ercd;
}
/**************************************
 End of function R_TSIP_Sha256UpdatePrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha256FinalPrivate
*******************************************************************************************************************/ /**
* @details       calculate a hash value
* @param[in,out] handle SHA256 handler (work area).
* @param[in]     MAX_CNT Input message length
* @param[out]    OutData_MsgDigest Output Hash data
* @param[out]    OutData_Length Output Hash length
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @see           R_TSIP_Sha224256GenerateMessageDigestSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha256FinalPrivate(tsip_sha_md5_handle_t *handle, uint32_t MAX_CNT,
        uint32_t *OutData_MsgDigest, uint32_t *OutData_Length)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;

    /* Casting uint32_t pointer is used for address. */
    ercd = R_TSIP_Sha224256GenerateMessageDigestSub((uint32_t*)handle->current_hash, (uint32_t*)handle->sha_buffer,
            MAX_CNT, OutData_MsgDigest);
    memcpy(handle->current_hash, OutData_MsgDigest, R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE);
    *OutData_Length = R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE;
    return ercd;
}
/**************************************
 End of function R_TSIP_Sha256FinalPrivate
 **************************************/
#endif /* TSIP_SHA_256 == 1 */

#if TSIP_MD5 == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Md5InitPrivate
*******************************************************************************************************************/ /**
* @details       prepares to execute the MD5 hash operation
* @param[out]    handle MD5 handler (work area)
* @retval        TSIP_SUCCESS: Normal termination.
* @note          None
*/
e_tsip_err_t R_TSIP_Md5InitPrivate(tsip_sha_md5_handle_t *handle)
{
    uint32_t md5_initval[4] =
    {
        0
    };

    md5_initval[0] = change_endian_long(0x67452301UL);
    md5_initval[1] = change_endian_long(0xefcdab89UL);
    md5_initval[2] = change_endian_long(0x98badcfeUL);
    md5_initval[3] = change_endian_long(0x10325476UL);
    memcpy(handle->current_hash, md5_initval, R_TSIP_MD5_HASH_LENGTH_BYTE_SIZE);

    return TSIP_SUCCESS;
}
/**************************************
 End of function R_TSIP_Md5InitPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Md5UpdatePrivate
*******************************************************************************************************************/ /**
* @details       calculate a hash value
* @param[in,out] handle MD5 handler (work area).
* @param[in]     InData_PaddingMsg Input message.
* @param[in]     MAX_CNT Input message length
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @see           R_TSIP_Md5GenerateMessageDigestSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Md5UpdatePrivate(tsip_sha_md5_handle_t *handle, uint32_t *InData_PaddedMsg, uint32_t MAX_CNT)
{
    uint32_t out_data[5] =
    {
        0
    };
    e_tsip_err_t ercd = TSIP_SUCCESS;

    /*Casting uint32_t pointer is used for address.*/
    ercd = R_TSIP_Md5GenerateMessageDigestSub((uint32_t*) handle->current_hash, InData_PaddedMsg, MAX_CNT, out_data);
    memcpy(handle->current_hash, out_data, R_TSIP_MD5_HASH_LENGTH_BYTE_SIZE);

    return ercd;
}
/**************************************
 End of function R_TSIP_Md5UpdatePrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Md5FinalPrivate
*******************************************************************************************************************/ /**
* @details       calculate a hash value
* @param[in,out] handle MD5 handler (work area).
* @param[in]     MAX_CNT Input message length
* @param[out]    OutData_MsgDigest Output Hash data
* @param[out]    OutData_Length Output Hash length
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @see           R_TSIP_Md5GenerateMessageDigestSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Md5FinalPrivate(tsip_sha_md5_handle_t *handle, uint32_t MAX_CNT,
        uint32_t *OutData_MsgDigest, uint32_t *OutData_Length)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;

    /* Casting uint32_t pointer is used for address. */
    ercd = R_TSIP_Md5GenerateMessageDigestSub((uint32_t*)handle->current_hash, (uint32_t*)handle->sha_buffer,
            MAX_CNT, OutData_MsgDigest);
    /* Casting is valid because it matches the type to the right side or argument. */
    OutData_MsgDigest[0] = R_BSP_REVL(OutData_MsgDigest[0]);
    /* Casting is valid because it matches the type to the right side or argument. */
    OutData_MsgDigest[1] = R_BSP_REVL(OutData_MsgDigest[1]);
    /* Casting is valid because it matches the type to the right side or argument. */
    OutData_MsgDigest[2] = R_BSP_REVL(OutData_MsgDigest[2]);
    /* Casting is valid because it matches the type to the right side or argument. */
    OutData_MsgDigest[3] = R_BSP_REVL(OutData_MsgDigest[3]);
    memcpy(handle->current_hash, OutData_MsgDigest, R_TSIP_MD5_HASH_LENGTH_BYTE_SIZE);
    *OutData_Length = R_TSIP_MD5_HASH_LENGTH_BYTE_SIZE;
    return ercd;
}
/**************************************
 End of function R_TSIP_Md5FinalPrivate
 **************************************/
#endif  /* TSIP_MD5 == 1 */

#if TSIP_PRV_USE_RSA_1024
/***********************************************************************************************************************
* Function Name: R_TSIP_Rsa1024ModularExponentEncryptPrivate
*******************************************************************************************************************/ /**
* @details       Modular exponentiation operation with RSA 1024bit public key
* @param[in]     InData_KeyIndex the public key index information
* @param[in]     InData_Text input data
* @param[out]    OutData_Text output data
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Rsa1024ModularExponentEncryptSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Rsa1024ModularExponentEncryptPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_Text,
        uint32_t *OutData_Text)
{
    e_tsip_err_t ercd;

    ercd = R_TSIP_Rsa1024ModularExponentEncryptSub(InData_KeyIndex, InData_Text, OutData_Text);

    return ercd;
}
/**************************************
 End of function R_TSIP_Rsa1024ModularExponentEncryptPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Rsa1024ModularExponentDecryptPrivate
*******************************************************************************************************************/ /**
* @details       Modular exponentiation operation with RSA 1024bit private key
* @param[in]     InData_KeyIndex the private key index information
* @param[in]     InData_Text input data
* @param[out]    OutData_Text output data
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Rsa1024ModularExponentDecryptSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Rsa1024ModularExponentDecryptPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_Text,
        uint32_t *OutData_Text)
{
    e_tsip_err_t ercd;

    ercd = R_TSIP_Rsa1024ModularExponentDecryptSub(InData_KeyIndex, InData_Text, OutData_Text);

    return ercd;
}
/**************************************
 End of function R_TSIP_Rsa1024ModularExponentDecryptPrivate
 **************************************/
#endif /* TSIP_PRV_USE_RSA_1024 */

#if TSIP_PRV_USE_RSA_2048
/***********************************************************************************************************************
* Function Name: R_TSIP_Rsa2048ModularExponentEncryptPrivate
*******************************************************************************************************************/ /**
* @details       Modular exponentiation operation with RSA 2048bit public key
* @param[in]     InData_KeyIndex the public key index information
* @param[in]     InData_Text input data
* @param[out]    OutData_Text output data
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Rsa2048ModularExponentEncryptSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Rsa2048ModularExponentEncryptPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_Text,
        uint32_t *OutData_Text)
{
    e_tsip_err_t ercd;

    ercd = R_TSIP_Rsa2048ModularExponentEncryptSub(InData_KeyIndex, InData_Text, OutData_Text);

    return ercd;
}
/**************************************
 End of function R_TSIP_Rsa2048ModularExponentEncryptPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Rsa2048ModularExponentDecryptPrivate
*******************************************************************************************************************/ /**
* @details       Modular exponentiation operation with RSA 2048bit private key
* @param[in]     InData_KeyIndex the private key index information
* @param[in]     InData_Text input data
* @param[out]    OutData_Text output data
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Rsa2048ModularExponentDecryptSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Rsa2048ModularExponentDecryptPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_Text,
        uint32_t *OutData_Text)
{
    e_tsip_err_t ercd;

    ercd = R_TSIP_Rsa2048ModularExponentDecryptSub(InData_KeyIndex, InData_Text, OutData_Text);

    return ercd;
}
/**************************************
 End of function R_TSIP_Rsa2048ModularExponentDecryptPrivate
 **************************************/
#endif /* TSIP_PRV_USE_RSA_2048 */

#if TSIP_PRV_USE_RSA_3072
/***********************************************************************************************************************
* Function Name: R_TSIP_Rsa3072ModularExponentEncryptPrivate
*******************************************************************************************************************/ /**
* @details       Modular exponentiation operation with RSA 3072bit public key
* @param[in]     InData_KeyIndex the public key index information
* @param[in]     InData_Text input data
* @param[out]    OutData_Text output data
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Rsa3072ModularExponentEncryptSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Rsa3072ModularExponentEncryptPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_Text,
        uint32_t *OutData_Text)
{
    e_tsip_err_t ercd;

    ercd = R_TSIP_Rsa3072ModularExponentEncryptSub(InData_KeyIndex, InData_Text, OutData_Text);

    return ercd;
}
/**************************************
 End of function R_TSIP_Rsa3072ModularExponentEncryptPrivate
 **************************************/
#endif /* TSIP_PRV_USE_RSA_3072 */

#if TSIP_PRV_USE_RSA_4096
/***********************************************************************************************************************
* Function Name: R_TSIP_Rsa4096ModularExponentEncryptPrivate
*******************************************************************************************************************/ /**
* @details       Modular exponentiation operation with RSA 4096bit public key
* @param[in]     InData_KeyIndex the public key index information
* @param[in]     InData_Text input data
* @param[out]    OutData_Text output data
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Rsa4096ModularExponentEncryptSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Rsa4096ModularExponentEncryptPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_Text,
        uint32_t *OutData_Text)
{
    e_tsip_err_t ercd;

    ercd = R_TSIP_Rsa4096ModularExponentEncryptSub(InData_KeyIndex, InData_Text, OutData_Text);

    return ercd;
}
/**************************************
 End of function R_TSIP_Rsa4096ModularExponentEncryptPrivate
 **************************************/
#endif /* TSIP_PRV_USE_RSA_4096 */

/***********************************************************************************************************************
* Function Name: R_TSIP_Rsa2048DhKeyAgreementPrivate
*******************************************************************************************************************/ /**
* @details       Calculate DH with RSA-2048.
* @param[in]     InData_KeyIndex key index information
* @param[in]     InData_PrimNumIndex private key index information to use DH calculation
* @param[in]     InData_Message message value
* @param[in]     InData_ModExp modulus exponent from receiver + MAC 2048 bit modulus exponent || 128 bit
* @param[out]    OutData_ModExp modulus exponent from sender + MAC 2048 bit modulus exponent || 128 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Rsa4096ModularExponentEncryptSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Rsa2048DhKeyAgreementPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_PrimNumIndex,
        uint32_t *InData_Message, uint32_t *InData_ModExp, uint32_t *OutData_ModExp)
{
    return R_TSIP_Rsa2048DhKeyAgreementSub(InData_KeyIndex, InData_PrimNumIndex, InData_Message, InData_ModExp,
            OutData_ModExp);
}
/**************************************
 End of function R_TSIP_Rsa2048DhKeyAgreementPrivate
 **************************************/

#if TSIP_SHA_1_HMAC  == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Sha1HmacGenerateInitPrivate
*******************************************************************************************************************/ /**
* @details       prepares to generate the SHA1 HMAC hash value
* @param[in]     handle SHA1 hamc handler.
* @param[in]     key_index key index
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Sha1HmacInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha1HmacGenerateInitPrivate(tsip_hmac_sha_handle_t *handle, tsip_hmac_sha_key_index_t *key_index)
{
    uint32_t indata_keytype = 0;

    if (TSIP_KEY_INDEX_TYPE_HMAC_SHA1_FOR_TLS != key_index->type)
    {
        indata_keytype = change_endian_long(0);
    }
    else
    {
        indata_keytype = change_endian_long(1);
    }
    return R_TSIP_Sha1HmacInitSub(&indata_keytype, key_index->value);
}
/**************************************
 End of function R_TSIP_Sha1HmacGenerateInitPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha1HmacVerifyInitPrivate
*******************************************************************************************************************/ /**
* @details       prepares to verify the SHA1 HMAC hash value
* @param[in]     handle SHA1 hamc handler.
* @param[in]     key_index key index
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Sha1HmacInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha1HmacVerifyInitPrivate(tsip_hmac_sha_handle_t *handle, tsip_hmac_sha_key_index_t *key_index)
{
    uint32_t indata_keytype = 0;

    if (TSIP_KEY_INDEX_TYPE_HMAC_SHA1_FOR_TLS != key_index->type)
    {
        indata_keytype = change_endian_long(0);
    }
    else
    {
        indata_keytype = change_endian_long(1);
    }
    return R_TSIP_Sha1HmacInitSub(&indata_keytype, key_index->value);
}
/**************************************
 End of function R_TSIP_Sha1HmacVerifyInitPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha1HmacGenerateUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Calculate the SHA1 HMAC hash value
* @param[in]     handle SHA1 hamc handler.
* @param[in]     InData_PaddedMsg Input value
* @param[in]     MAX_CNT Input value word size
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Sha1HmacUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha1HmacGenerateUpdatePrivate(tsip_hmac_sha_handle_t *handle, uint32_t *InData_PaddedMsg,
        uint32_t MAX_CNT)
{
    R_TSIP_Sha1HmacUpdateSub(InData_PaddedMsg, MAX_CNT);

    return TSIP_SUCCESS;
}
/**************************************
 End of function R_TSIP_Sha1HmacGenerateUpdatePrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha1HmacGenerateFinalPrivate
*******************************************************************************************************************/ /**
* @details       Generate the SHA1 HMAC hash value
* @param[in]     handle SHA1 hamc handler.
* @param[out]    OutData_Mac Output MAC value
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Sha1HmacFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha1HmacGenerateFinalPrivate(tsip_hmac_sha_handle_t *handle, uint32_t *OutData_Mac)
{
    uint32_t indata_cmd = 0;
    uint32_t indata_mac = 0; /*dummy*/
    uint32_t indata_length = 0; /*dummy*/

    indata_cmd = change_endian_long(0);
    return R_TSIP_Sha1HmacFinalSub(&indata_cmd, &indata_mac, &indata_length, OutData_Mac);
}
/**************************************
 End of function R_TSIP_Sha1HmacGenerateFinalPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha1HmacVerifyUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Calculate the SHA1 HMAC hash value
* @param[in]     handle SHA1 hamc handler (work area).
* @param[in]     InData_PaddedMsg Input value
* @param[in]     MAX_CNT Input value word size
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Sha1HmacUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha1HmacVerifyUpdatePrivate(tsip_hmac_sha_handle_t *handle, uint32_t *InData_PaddedMsg,
        uint32_t MAX_CNT)
{
    R_TSIP_Sha1HmacUpdateSub(InData_PaddedMsg, MAX_CNT);

    return TSIP_SUCCESS;
}
/**************************************
 End of function R_TSIP_Sha1HmacVerifyUpdatePrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha1HmacVerifyFinalPrivate
*******************************************************************************************************************/ /**
* @details       Verify the SHA1 HMAC hash value
* @param[in]     handle SHA1 hamc handler.
* @param[in]     InData_Mac Input MAC value
* @param[in]     InData_length Input MAC value byte size
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Sha1HmacFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha1HmacVerifyFinalPrivate(tsip_hmac_sha_handle_t *handle, uint32_t *InData_Mac,
        uint32_t *InData_length)
{
    uint32_t indata_cmd = 0;
    uint32_t outdata_mac = 0;

    indata_cmd = change_endian_long(1);
    return R_TSIP_Sha1HmacFinalSub(&indata_cmd, InData_Mac, InData_length, &outdata_mac);
}
/**************************************
 End of function R_TSIP_Sha1HmacVerifyFinalPrivate
 **************************************/
#endif /* TSIP_SHA_1_HMAC  == 1 */

#if TSIP_SHA_256_HMAC  == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Sha256HmacGenerateInitPrivate
*******************************************************************************************************************/ /**
* @details       prepares to generate the SHA256 HMAC hash value
* @param[in]     handle SHA256 hamc handler.
* @param[in]     key_index key index
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Sha256HmacInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha256HmacGenerateInitPrivate(tsip_hmac_sha_handle_t *handle, tsip_hmac_sha_key_index_t *key_index)
{
    uint32_t indata_keytype;
    uint32_t indata_cmd = 0;

    if (TSIP_KEY_INDEX_TYPE_HMAC_SHA256_FOR_TLS == key_index->type)
    {
        indata_keytype = change_endian_long(1); /* For TLS */
    }
    else if (TSIP_KEY_INDEX_TYPE_HMAC_SHA256_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    else if (TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_FINISHED_FOR_CLIENT == key_index->type)
    {
        indata_keytype = change_endian_long(4); /* For TLS1.3 */

        if (TSIP_TLS13_MODE_FULL_HANDSHAKE == key_index->tls13_mode)
        {
            indata_cmd = 0;
        }
        else if (TSIP_TLS13_MODE_RESUMPTION == key_index->tls13_mode)
        {
            indata_cmd = change_endian_long(1);
        }
        else /* TSIP_TLS13_MODE_0_RTT == key_index->tls13_mode */
        {
            indata_cmd = change_endian_long(2);
        }
    }
    else if (TSIP_KEY_INDEX_TYPE_TLS13_PSK_BINDER_KEY_FOR_CLIENT == key_index->type)
    {
        indata_keytype = change_endian_long(4); /* For TLS1.3 */
        indata_cmd     = change_endian_long(3); /* Binder Key for client */
    }
    else if (TSIP_KEY_INDEX_TYPE_TLS13_SERVER_FINISHED_FOR_SERVER == key_index->type)
    {
        indata_keytype = change_endian_long(4); /* For TLS1.3 */

        if (TSIP_TLS13_MODE_FULL_HANDSHAKE == key_index->tls13_mode)
        {
            indata_cmd = change_endian_long(4);
        }
        else if (TSIP_TLS13_MODE_RESUMPTION == key_index->tls13_mode)
        {
            indata_cmd = change_endian_long(5);
        }
        else /* TSIP_TLS13_MODE_0_RTT == key_index->tls13_mode */
        {
            indata_cmd = change_endian_long(6);
        }
    }
    else
    {
        indata_keytype = 0; /* For normal */
    }
    return R_TSIP_Sha256HmacInitSub(&indata_keytype, &indata_cmd, key_index->value);
}
/**************************************
 End of function R_TSIP_Sha256HmacGenerateInitPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha256HmacVerifyInitPrivate
*******************************************************************************************************************/ /**
* @details       prepares to generate the SHA256 HMAC hash value
* @param[in]     handle SHA256 hamc handler.
* @param[in]     key_index key index
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Sha256HmacInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha256HmacVerifyInitPrivate(tsip_hmac_sha_handle_t *handle, tsip_hmac_sha_key_index_t *key_index)
{
    uint32_t indata_keytype;
    uint32_t indata_cmd = 0;

    if (TSIP_KEY_INDEX_TYPE_HMAC_SHA256_FOR_TLS == key_index->type)
    {
        indata_keytype = change_endian_long(1); /* For TLS */
    }
    else if (TSIP_KEY_INDEX_TYPE_HMAC_SHA256_FOR_ECDH == key_index->type)
    {
        indata_keytype = change_endian_long(2); /* For ECDH */
    }
    else if (TSIP_KEY_INDEX_TYPE_TLS13_PSK_BINDER_KEY_FOR_SERVER == key_index->type)
    {
        indata_keytype = change_endian_long(4); /* For TLS1.3 */
        indata_cmd     = change_endian_long(7); /* Binder Key for server */
    }
    else
    {
        indata_keytype = 0; /* For normal */
    }
    return R_TSIP_Sha256HmacInitSub(&indata_keytype, &indata_cmd, key_index->value);
}
/**************************************
 End of function R_TSIP_Sha256HmacVerifyInitPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha256HmacGenerateUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Calculate the SHA256 HMAC hash value
* @param[in]     handle SHA256 hamc handler.
* @param[in]     InData_PaddedMsg Input value
* @param[in]     MAX_CNT Input value word size
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Sha256HmacUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha256HmacGenerateUpdatePrivate(tsip_hmac_sha_handle_t *handle, uint32_t *InData_PaddedMsg,
        uint32_t MAX_CNT)
{
    R_TSIP_Sha256HmacUpdateSub(InData_PaddedMsg, MAX_CNT);

    return TSIP_SUCCESS;
}
/**************************************
 End of function R_TSIP_Sha256HmacGenerateUpdatePrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha256HmacGenerateFinalPrivate
*******************************************************************************************************************/ /**
* @details       Generate the SHA256 HMAC hash value
* @param[in]     handle SHA256 hamc handler.
* @param[out]    OutData_Mac Output MAC value
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Sha256HmacFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha256HmacGenerateFinalPrivate(tsip_hmac_sha_handle_t *handle, uint32_t *OutData_Mac)
{
    uint32_t indata_cmd = 0;
    uint32_t indata_mac = 0; /*dummy*/
    uint32_t indata_length = 0; /*dummy*/

    indata_cmd = change_endian_long(0);
    return R_TSIP_Sha256HmacFinalSub(&indata_cmd, &indata_mac, &indata_length, OutData_Mac);
}
/**************************************
 End of function R_TSIP_Sha256HmacGenerateFinalPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha256HmacVerifyUpdatePrivate
*******************************************************************************************************************/ /**
* @details       Calculate the SHA256 HMAC hash value
* @param[in]     handle SHA256 hamc handler.
* @param[in]     InData_PaddedMsg Input value
* @param[in]     MAX_CNT Input value word size
* @retval        TSIP_SUCCESS: Normal termination.
* @see           R_TSIP_Sha256HmacUpdateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha256HmacVerifyUpdatePrivate(tsip_hmac_sha_handle_t *handle, uint32_t *InData_PaddedMsg,
        uint32_t MAX_CNT)
{
    R_TSIP_Sha256HmacUpdateSub(InData_PaddedMsg, MAX_CNT);

    return TSIP_SUCCESS;
}
/**************************************
 End of function R_TSIP_Sha256HmacVerifyUpdatePrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Sha256HmacVerifyFinalPrivate
*******************************************************************************************************************/ /**
* @details       Verify the SHA256 HMAC hash value
* @param[in]     handle SHA256 hamc handler.
* @param[in]     InData_Mac Input MAC value
* @param[in]     InData_length Input MAC value byte size
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Sha256HmacFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Sha256HmacVerifyFinalPrivate(tsip_hmac_sha_handle_t *handle, uint32_t *InData_Mac,
        uint32_t *InData_length)
{
    uint32_t indata_cmd = 0;
    uint32_t outdata_mac = 0;

    indata_cmd = change_endian_long(1);
    return R_TSIP_Sha256HmacFinalSub(&indata_cmd, InData_Mac, InData_length, &outdata_mac);
}
/**************************************
 End of function R_TSIP_Sha256HmacVerifyFinalPrivate
 **************************************/
#endif /* TSIP_SHA_256_HMAC  == 1 */

#if TSIP_ECDH_P256 == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_EcdhReadPublicKeyPrivate
*******************************************************************************************************************/ /**
* @details       Verify signature of ECC P-256 public key and output public key generation information.
* @param[in]     InData_Cmd key_id use or not
* @param[in]     InData_KeyIndex public key index for signature verification
* @param[in]     InData_data API function argument "public_key_data" plus "stop bit" and "message length"
* @param[in]     InData_Signature signature text
* @param[out]    OutData_KeyIndex public key index for API function argument "public_key_data"
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_DlmsCosemQeuSignatureVerificationSub()
* @note          None
*/
e_tsip_err_t R_TSIP_EcdhReadPublicKeyPrivate(uint32_t *InData_Cmd, uint32_t *InData_KeyIndex, uint32_t *InData_data,
        uint32_t *InData_Signature, uint32_t *OutData_KeyIndex)
{
    return R_TSIP_DlmsCosemQeuSignatureVerificationSub(InData_Cmd, InData_KeyIndex, InData_data, InData_Signature,
            OutData_KeyIndex);
}
/**************************************
 End of function R_TSIP_EcdhReadPublicKeyPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_EcdhMakePublicKeyPrivate
*******************************************************************************************************************/ /**
* @details       Generate signature for public key generation information for ECDH key exchange.
* @param[in]     InData_Cmd key_id use or not
* @param[in]     InData_KeyType type of key to be exchanged
* @param[in]     InData_PubKeyIndex public key index(for ECDH)
* @param[in]     InData_PrivKeyIndex private key index for signature generation
* @param[in]     InData_key_id key_id
* @param[out]    OutData_data public key for key exchange
* @param[out]    OutData_Signature signature text
* @param[out]    OutData_KeyIndex private key index generated from random numbers(for ECDHE)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_DlmsCosemQevSignatureGenerationSub()
* @note          None
*/
e_tsip_err_t R_TSIP_EcdhMakePublicKeyPrivate(uint32_t *InData_Cmd, uint32_t *InData_KeyType,
        uint32_t *InData_PubKeyIndex, uint32_t *InData_PrivKeyIndex, uint32_t *InData_key_id, uint32_t *OutData_data,
        uint32_t *OutData_Signature, uint32_t *OutData_KeyIndex)
{
    return R_TSIP_DlmsCosemQevSignatureGenerationSub(InData_Cmd, InData_KeyType, InData_PubKeyIndex,
            InData_PrivKeyIndex, InData_key_id, OutData_data, OutData_Signature, OutData_KeyIndex);
}
/**************************************
 End of function R_TSIP_EcdhMakePublicKeyPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_EcdhCalculateSharedSecretIndexPrivate
*******************************************************************************************************************/ /**
* @details       Output key generation information of shared secret "Z" from partner public key and your private key.
* @param[in]     InData_KeyType type of key to be exchanged
* @param[in]     InData_PubKeyIndex public key index
* @param[in]     InData_PrivKeyIndex private key index
* @param[out]    OutData_KeyIndex key index of shared secret "Z"
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_DlmsCosemCalculateZSub()
* @note          None
*/
e_tsip_err_t R_TSIP_EcdhCalculateSharedSecretIndexPrivate(uint32_t *InData_KeyType, uint32_t *InData_PubKeyIndex,
        uint32_t *InData_PrivKeyIndex, uint32_t *OutData_KeyIndex)
{
    return R_TSIP_DlmsCosemCalculateZSub(InData_KeyType, InData_PubKeyIndex, InData_PrivKeyIndex, OutData_KeyIndex);
}
/**************************************
 End of function R_TSIP_EcdhCalculateSharedSecretIndexPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_EcdhKeyDerivationPrivate
*******************************************************************************************************************/ /**
* @details       Key is derived using the shared secret "Z" as the key material.
* @param[in]     InData_KeyIndexType Derived key type(0:AES-128, 1:AES-256, 2:HMAC-SHA256)
* @param[in]     InData_KeyIndex key index of shared secret "Z"
* @param[in]     InData_KDFType Algorithm used for key derivation calculation(0:SHA256, 1:HMAC-SHA256)
* @param[in]     InData_PaddedMsg additional data used for key derivation, preceded by Algorithm ID encoded value,
* followed by stop bit and data length
* @param[in]     MAX_CNT maximum number of loops when reading InData_PaddedMsg
* @param[in]     InData_SaltKeyIndex Salt key index
* @param[out]    OutData_KeyIndex key index of KEK
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_DlmsCosemCalculateKekSub()
* @note          None
*/
e_tsip_err_t R_TSIP_EcdhKeyDerivationPrivate(uint32_t *InData_KeyIndexType, uint32_t *InData_KeyIndex,
        uint32_t *InData_KDFType, uint32_t *InData_PaddedMsg, uint32_t MAX_CNT, uint32_t *InData_SaltKeyIndex,
        uint32_t *OutData_KeyIndex)
{
    return R_TSIP_DlmsCosemCalculateKekSub(InData_KeyIndexType, InData_KeyIndex, InData_KDFType, InData_PaddedMsg,
            MAX_CNT, InData_SaltKeyIndex, OutData_KeyIndex);
}
/**************************************
 End of function R_TSIP_EcdhKeyDerivationPrivate
 **************************************/
#endif  /* TSIP_ECDH_P256 == 1 */

/***********************************************************************************************************************
* Function Name: R_TSIP_EcdheP512KeyAgreementPrivate
*******************************************************************************************************************/ /**
* @details       Generate key pair with Brainpool P512r1 and calculate ECDHE
* @param[in]     InData_KeyIndex key index information
* @param[in]     InData_PubKey Brainpool P512r1 public key Q of receiver (1024 bit) || MAC (128 bit)
* @param[out]    OutData_PubKey Brainpool P512r1 public key Q of sender (1024 bit) || MAC (128 bit)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_EcdheP512KeyAgreementSub()
* @note          None
*/
e_tsip_err_t R_TSIP_EcdheP512KeyAgreementPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_PubKey,
        uint32_t *OutData_PubKey)
{
    return R_TSIP_EcdheP512KeyAgreementSub(InData_KeyIndex, InData_PubKey, OutData_PubKey);
}
/**************************************
 End of function R_TSIP_EcdheP512KeyAgreementPrivate
 **************************************/

