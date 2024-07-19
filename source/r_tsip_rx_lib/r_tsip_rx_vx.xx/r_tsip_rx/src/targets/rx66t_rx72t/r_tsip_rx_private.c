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
 * Copyright (C) 2016-2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_tsip_rx_private.c
 * Version      : 1.19
 * Description  : Interface definition for the TSIP-Lite private layer.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 08.01.2016 1.00     First Release
 *         : 15.05.2017 1.01     Add AES-CMAC
 *         : 30.09.2017 1.03     Change API name for TSIP V.1.03.
 *         : 28.02.2018 1.04     Change API
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
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
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
* @see           R_TSIP_Aes128EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128EcbEncryptInitPrivate(tsip_aes_key_index_t *key_index)
{
    uint32_t indata_cmd = change_endian_long(0); /* ECB-Encrypt command */
    uint32_t indata_iv[4] = /* dummy */
    {
        0
    };

    if (TSIP_KEY_INDEX_TYPE_AES128 != key_index->type)
    {
        return TSIP_ERR_KEY_SET;
    }
    return R_TSIP_Aes128EncryptDecryptInitSub(&indata_cmd, key_index->value, indata_iv);
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
* @see           R_TSIP_Aes128EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128EcbDecryptInitPrivate(tsip_aes_key_index_t *key_index)
{
    uint32_t indata_cmd = change_endian_long(1); /* ECB-Decrypt command */
    uint32_t indata_iv[4] = /* dummy */
    {
        0
    };

    if (TSIP_KEY_INDEX_TYPE_AES128 != key_index->type)
    {
        return TSIP_ERR_KEY_SET;
    }
    return R_TSIP_Aes128EncryptDecryptInitSub(&indata_cmd, key_index->value, indata_iv);
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
* @param[out]    OutData_length Output byte size of plain.
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
* @see           R_TSIP_Aes256EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256EcbEncryptInitPrivate(tsip_aes_key_index_t *key_index)
{
    uint32_t indata_cmd = change_endian_long(0); /* ECB-Encrypt command */
    uint32_t indata_iv[4] = /* dummy */
    {
        0
    };

    if (TSIP_KEY_INDEX_TYPE_AES256 != key_index->type)
    {
        return TSIP_ERR_KEY_SET;
    }
    return R_TSIP_Aes256EncryptDecryptInitSub(&indata_cmd, key_index->value, indata_iv);
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
* @see           R_TSIP_Aes256EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256EcbDecryptInitPrivate(tsip_aes_key_index_t *key_index)
{
    uint32_t indata_cmd = change_endian_long(1); /* ECB-Decrypt command */
    uint32_t indata_iv[4] = /* dummy */
    {
        0
    };

    if (TSIP_KEY_INDEX_TYPE_AES256 != key_index->type)
    {
        return TSIP_ERR_KEY_SET;
    }
    return R_TSIP_Aes256EncryptDecryptInitSub(&indata_cmd, key_index->value, indata_iv);
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
* @see           R_TSIP_Aes128EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CbcEncryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV)
{
    uint32_t indata_cmd = change_endian_long(2); /* CBC-Encrypt command */

    if (TSIP_KEY_INDEX_TYPE_AES128 != key_index->type)
    {
        return TSIP_ERR_KEY_SET;
    }
    return R_TSIP_Aes128EncryptDecryptInitSub(&indata_cmd, key_index->value, InData_IV);
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
* @see           R_TSIP_Aes128EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CbcDecryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV)
{
    uint32_t indata_cmd = change_endian_long(3); /* CBC-Decrypt command */

    if (TSIP_KEY_INDEX_TYPE_AES128 != key_index->type)
    {
        return TSIP_ERR_KEY_SET;
    }
    return R_TSIP_Aes128EncryptDecryptInitSub(&indata_cmd, key_index->value, InData_IV);
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
* @see           R_TSIP_Aes256EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CbcEncryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV)
{
    uint32_t indata_cmd = change_endian_long(2); /* CBC-Encrypt command */

    if (TSIP_KEY_INDEX_TYPE_AES256 != key_index->type)
    {
        return TSIP_ERR_KEY_SET;
    }
    return R_TSIP_Aes256EncryptDecryptInitSub(&indata_cmd, key_index->value, InData_IV);
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
* @see           R_TSIP_Aes256EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CbcDecryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV)
{
    uint32_t indata_cmd = change_endian_long(3); /* CBC-Decrypt command */

    if (TSIP_KEY_INDEX_TYPE_AES256 != key_index->type)
    {
        return TSIP_ERR_KEY_SET;
    }
    return R_TSIP_Aes256EncryptDecryptInitSub(&indata_cmd, key_index->value, InData_IV);
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
* @see           R_TSIP_Aes128EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CtrInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV)
{
    uint32_t indata_cmd = change_endian_long(4); /* CTR command */

    if (TSIP_KEY_INDEX_TYPE_AES128 != key_index->type)
    {
        return TSIP_ERR_KEY_SET;
    }
    return R_TSIP_Aes128EncryptDecryptInitSub(&indata_cmd, key_index->value, InData_IV);
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
* @see           R_TSIP_Aes256EncryptDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CtrInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV)
{
    uint32_t indata_cmd = change_endian_long(4); /* CTR command */

    if (TSIP_KEY_INDEX_TYPE_AES256 != key_index->type)
    {
        return TSIP_ERR_KEY_SET;
    }
    return R_TSIP_Aes256EncryptDecryptInitSub(&indata_cmd, key_index->value, InData_IV);
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
* @param[in]     InData_IV Input initial vector.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Aes128GcmEncryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128GcmEncryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV)
{
    return R_TSIP_Aes128GcmEncryptInitSub(key_index->value, InData_IV);
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
* @param[in]     InData_IV Input initial vector.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Aes128GcmDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128GcmDecryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV)
{
    return R_TSIP_Aes128GcmDecryptInitSub(key_index->value, InData_IV);
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
* @see           R_TSIP_Aes256GcmEncryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256GcmEncryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV)
{
    return R_TSIP_Aes256GcmEncryptInitSub(key_index->value, InData_IV);
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
* @see           R_TSIP_Aes256GcmDecryptInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256GcmDecryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV)
{
    return R_TSIP_Aes256GcmDecryptInitSub(key_index->value, InData_IV);
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

#if TSIP_PRV_USE_AES_GCM
/***********************************************************************************************************************
* Function Name: R_TSIP_AesGcmEncryptUpdateTransitionPrivate
*******************************************************************************************************************/ /**
* @details       Transition TSIP mode for encrypt data using AES GCM algorithm.
* @see           R_TSIP_AesGcmEncryptDecryptUpdateTransitionSub()
* @note          None
*/
void R_TSIP_AesGcmEncryptUpdateTransitionPrivate(void)
{
    R_TSIP_AesGcmEncryptDecryptUpdateTransitionSub();
    return;
}
/**************************************
 End of function R_TSIP_AesGcmEncryptUpdateTransitionPrivate
 **************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_AesGcmDecryptUpdateTransitionPrivate
*******************************************************************************************************************/ /**
* @details       Transition TSIP mode for encrypt data using AES GCM algorithm.
* @see           R_TSIP_AesGcmEncryptDecryptUpdateTransitionSub()
* @note          None
*/
void R_TSIP_AesGcmDecryptUpdateTransitionPrivate(void)
{
    R_TSIP_AesGcmEncryptDecryptUpdateTransitionSub();
    return;
}
/**************************************
 End of function R_TSIP_AesGcmDecryptUpdateTransitionPrivate
 **************************************/
#endif /* TSIP_PRV_USE_AES_GCM */

#if TSIP_AES_128_CCM_ENCRYPT == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_Aes128CcmEncryptInitPrivate
*******************************************************************************************************************/ /**
* @details       Encrypt data to cipher using AES CCM algorithm with AES 128 key.
* @param[in]     key_index User key generation information area.
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
e_tsip_err_t R_TSIP_Aes128CcmEncryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV,
        uint32_t *InData_Header, uint32_t Header_Len)
{
    return R_TSIP_Aes128CcmEncryptInitSub(key_index->value, InData_IV, InData_Header, Header_Len);
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
* @param[in]     InData_TextLen Input byte size of plain.
* @param[in]     InData_Text Input plain text.
* @param[out]    OutData_Text Output cipher text.
* @param[out]    OutData_DataT Output MAC data.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128CcmEncryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CcmEncryptFinalPrivate(uint32_t *InData_TextLen, uint32_t *InData_Text,
        uint32_t *OutData_Text, uint32_t *OutData_MAC)
{
    return R_TSIP_Aes128CcmEncryptFinalSub(InData_TextLen, InData_Text, OutData_Text, OutData_MAC);
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
e_tsip_err_t R_TSIP_Aes128CcmDecryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV,
        uint32_t *InData_Header, uint32_t Header_Len)
{
    return R_TSIP_Aes128CcmDecryptInitSub(key_index->value, InData_IV, InData_Header, Header_Len);
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
* @param[in]     InData_TextLen Input byte size of cipher.
* @param[in]     InData_MAC Input MAC data.
* @param[in]     InData_MACLength Input byte size of MAC data.
* @param[out]    OutData_Text Output plain text.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_Aes128CcmDecryptFinalSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CcmDecryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_TextLen, uint32_t *InData_MAC,
        uint32_t *InData_MACLength, uint32_t *OutData_Text)
{
    return R_TSIP_Aes128CcmDecryptFinalSub(InData_Text, InData_TextLen, InData_MAC, InData_MACLength, OutData_Text);
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
    return R_TSIP_Aes256CcmEncryptInitSub(key_index->value, InData_IV, InData_Header, Header_Len);
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
    return R_TSIP_Aes256CcmDecryptInitSub(key_index->value, InData_IV, InData_Header, Header_Len);
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
* @see           R_TSIP_Aes128CmacInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CmacGenerateInitPrivate(tsip_aes_key_index_t *key_index)
{
    return R_TSIP_Aes128CmacInitSub(key_index->value);
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
* @see           R_TSIP_Aes128CmacInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CmacVerifyInitPrivate(tsip_aes_key_index_t *key_index)
{
    return R_TSIP_Aes128CmacInitSub(key_index->value);
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
* @see           R_TSIP_Aes256CmacInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CmacGenerateInitPrivate(tsip_aes_key_index_t *key_index)
{
    return R_TSIP_Aes256CmacInitSub(key_index->value);
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
* @see           R_TSIP_Aes256CmacInitSub()
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256CmacVerifyInitPrivate(tsip_aes_key_index_t *key_index)
{
    return R_TSIP_Aes256CmacInitSub(key_index->value);
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

    return R_TSIP_AESKeyUnwrapSub(&indata_cmd, key_index->value, key_index_size,
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

    return R_TSIP_AESKeyUnwrapSub(&indata_cmd, key_index->value, key_index_size,
            InData_WrappedKeyType, InData_Text, wrapped_key_size, OutData_KeyIndex);
}
/**************************************
 End of function R_TSIP_Aes256KeyUnWrapPrivate
 **************************************/
#endif /* TSIP_AES_256_KEY_WRAP == 1 */

