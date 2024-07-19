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
 * Copyright (C) 2017-2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_tsip_rx.c
 * Version      : 1.19
 * Description  : Interface definition for the r_tsip_rx TSIP module.
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
uint32_t S_RAM[R_TSIP_SRAM_WORD_SIZE];
uint32_t S_HEAP[R_TSIP_SHEAP_WORD_SIZE];
uint32_t S_INST[R_TSIP_SINST_WORD_SIZE];
uint32_t S_INST2[R_TSIP_SINST2_WORD_SIZE];

uint32_t g_private_id_counter;
#if TSIP_AES_128_ECB_ENCRYPT == 1
extern uint32_t g_aes128ecbenc_private_id;
#endif
#if TSIP_AES_128_ECB_DECRYPT == 1
extern uint32_t g_aes128ecbdec_private_id;
#endif
#if TSIP_AES_256_ECB_ENCRYPT == 1
extern uint32_t g_aes256ecbenc_private_id;
#endif
#if TSIP_AES_256_ECB_DECRYPT == 1
extern uint32_t g_aes256ecbdec_private_id;
#endif
#if TSIP_AES_128_CBC_ENCRYPT == 1
extern uint32_t g_aes128cbcenc_private_id;
#endif
#if TSIP_AES_128_CBC_DECRYPT == 1
extern uint32_t g_aes128cbcdec_private_id;
#endif
#if TSIP_AES_256_CBC_ENCRYPT == 1
extern uint32_t g_aes256cbcenc_private_id;
#endif
#if TSIP_AES_256_CBC_DECRYPT == 1
extern uint32_t g_aes256cbcdec_private_id;
#endif
#if TSIP_AES_128_CTR == 1
extern uint32_t g_aes128ctr_private_id;
#endif
#if TSIP_AES_256_CTR == 1
extern uint32_t g_aes256ctr_private_id;
#endif
#if TSIP_AES_128_CMAC == 1
extern uint32_t g_aes128cmacgen_private_id;
extern uint32_t g_aes128cmacver_private_id;
#endif
#if TSIP_AES_256_CMAC == 1
extern uint32_t g_aes256cmacgen_private_id;
extern uint32_t g_aes256cmacver_private_id;
#endif
#if TSIP_AES_128_GCM_ENCRYPT == 1
extern uint32_t g_aes128gcmenc_private_id;
#endif
#if TSIP_AES_128_GCM_DECRYPT == 1
extern uint32_t g_aes128gcmdec_private_id;
#endif
#if TSIP_AES_256_GCM_ENCRYPT == 1
extern uint32_t g_aes256gcmenc_private_id;
#endif
#if TSIP_AES_256_GCM_DECRYPT == 1
extern uint32_t g_aes256gcmdec_private_id;
#endif
#if TSIP_AES_128_CCM_ENCRYPT == 1
extern uint32_t g_aes128ccmenc_private_id;
#endif
#if TSIP_AES_128_CCM_DECRYPT == 1
extern uint32_t g_aes128ccmdec_private_id;
#endif
#if TSIP_AES_256_CCM_ENCRYPT == 1
extern uint32_t g_aes256ccmenc_private_id;
#endif
#if TSIP_AES_256_CCM_DECRYPT == 1
extern uint32_t g_aes256ccmdec_private_id;
#endif
#if TSIP_TDES_ECB_ENCRYPT == 1
extern uint32_t g_tdes_ecbenc_private_id;
#endif
#if TSIP_TDES_ECB_DECRYPT == 1
extern uint32_t g_tdes_ecbdec_private_id;
#endif
#if TSIP_TDES_CBC_ENCRYPT == 1
extern uint32_t g_tdes_cbcenc_private_id;
#endif
#if TSIP_TDES_CBC_DECRYPT == 1
extern uint32_t g_tdes_cbcdec_private_id;
#endif
#if TSIP_ARC4_ENCRYPT == 1
extern uint32_t g_arc4_enc_private_id;
#endif
#if TSIP_ARC4_DECRYPT == 1
extern uint32_t g_arc4_dec_private_id;
#endif
#if TSIP_SHA_1_HMAC == 1
extern uint32_t g_sha1hmacgen_private_id;
extern uint32_t g_sha1hmacver_private_id;
#endif
#if TSIP_SHA_256_HMAC == 1
extern uint32_t g_sha256hmacgen_private_id;
extern uint32_t g_sha256hmacver_private_id;
#endif
#if TSIP_ECDH_P256 == 1
extern uint32_t g_ecdh256_private_id;
#endif

#if TSIP_FIRMWARE_UPDATE == 1
TSIP_GEN_MAC_CB_FUNC_T TSIP_GEN_MAC_CB_FUNC;
#endif

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
TSIP_SEC_B_SECURE_BOOT
static uint32_t *sp_user_program_mac;

static e_tsip_err_t R_TSIP_SelfCheck1Private(void);
static e_tsip_err_t R_TSIP_SelfCheck2Private(void);

/***********************************************************************************************************************
* Function Name: R_TSIP_Open
*******************************************************************************************************************/ /**
* @details       Input power to the TSIP
* @param[in]     key_index_1 TLS cooperation RSA public keyring key index
* @param[in]     key_index_2 Key update keyring key index
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_RETRY: self-test2 fail
* @see           R_TSIP_SelfCheck1Private()
* @see           R_TSIP_SelfCheck2Private()
* @note          None
*/
TSIP_SEC_P_SECURE_BOOT
e_tsip_err_t R_TSIP_Open(tsip_tls_ca_certification_public_key_index_t *key_index_1,
        tsip_update_key_ring_t *key_index_2)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
    /* Casting structure pointer is used for address. */
    MSTP_SECURITY = 0u;
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    R_TSIP_SoftwareReset();
    error_code = R_TSIP_SelfCheck1Private();
    if (TSIP_SUCCESS == error_code)
    {
        error_code = R_TSIP_SelfCheck2Private();
    }
#ifdef __LIT
    /* Casting structure pointer is used for address. */
    TSIP.REG_1D4H.WORD = 0x0000ffff;
#endif

    g_private_id_counter = 0;
#if TSIP_AES_128_ECB_ENCRYPT == 1
    g_aes128ecbenc_private_id = 0;
#endif
#if TSIP_AES_128_ECB_DECRYPT == 1
    g_aes128ecbdec_private_id = 0;
#endif
#if TSIP_AES_256_ECB_ENCRYPT == 1
    g_aes256ecbenc_private_id = 0;
#endif
#if TSIP_AES_256_ECB_DECRYPT == 1
    g_aes256ecbdec_private_id = 0;
#endif
#if TSIP_AES_128_CBC_ENCRYPT == 1
    g_aes128cbcenc_private_id = 0;
#endif
#if TSIP_AES_128_CBC_DECRYPT == 1
    g_aes128cbcdec_private_id = 0;
#endif
#if TSIP_AES_256_CBC_ENCRYPT == 1
    g_aes256cbcenc_private_id = 0;
#endif
#if TSIP_AES_256_CBC_DECRYPT == 1
    g_aes256cbcdec_private_id = 0;
#endif
#if TSIP_AES_128_CTR == 1
    g_aes128ctr_private_id = 0;
#endif
#if TSIP_AES_256_CTR == 1
    g_aes256ctr_private_id = 0;
#endif
#if TSIP_AES_128_CMAC == 1
    g_aes128cmacgen_private_id = 0;
    g_aes128cmacver_private_id = 0;
#endif
#if TSIP_AES_256_CMAC == 1
    g_aes256cmacgen_private_id = 0;
    g_aes256cmacver_private_id = 0;
#endif
#if TSIP_AES_128_GCM_ENCRYPT == 1
    g_aes128gcmenc_private_id = 0;
#endif
#if TSIP_AES_128_GCM_DECRYPT == 1
    g_aes128gcmdec_private_id = 0;
#endif
#if TSIP_AES_256_GCM_ENCRYPT == 1
    g_aes256gcmenc_private_id = 0;
#endif
#if TSIP_AES_256_GCM_DECRYPT == 1
    g_aes256gcmdec_private_id = 0;
#endif
#if TSIP_AES_128_CCM_ENCRYPT == 1
    g_aes128ccmenc_private_id = 0;
#endif
#if TSIP_AES_128_CCM_DECRYPT == 1
    g_aes128ccmdec_private_id = 0;
#endif
#if TSIP_AES_256_CCM_ENCRYPT == 1
    g_aes256ccmenc_private_id = 0;
#endif
#if TSIP_AES_256_CCM_DECRYPT == 1
    g_aes256ccmdec_private_id = 0;
#endif
#if TSIP_TDES_ECB_ENCRYPT == 1
    g_tdes_ecbenc_private_id = 0;
#endif
#if TSIP_TDES_ECB_DECRYPT == 1
    g_tdes_ecbdec_private_id = 0;
#endif
#if TSIP_TDES_CBC_ENCRYPT == 1
    g_tdes_cbcenc_private_id = 0;
#endif
#if TSIP_TDES_CBC_DECRYPT == 1
    g_tdes_cbcdec_private_id = 0;
#endif
#if TSIP_ARC4_ENCRYPT == 1
    g_arc4_enc_private_id = 0;
#endif
#if TSIP_ARC4_DECRYPT == 1
    g_arc4_dec_private_id = 0;
#endif
#if TSIP_SHA_1_HMAC == 1
    g_sha1hmacgen_private_id = 0;
    g_sha1hmacver_private_id = 0;
#endif
#if TSIP_SHA_256_HMAC == 1
    g_sha256hmacgen_private_id = 0;
    g_sha256hmacver_private_id = 0;
#endif
#if TSIP_ECDH_P256 == 1
    g_ecdh256_private_id = 0;
#endif

    if (NULL != key_index_1)
    {
        memcpy(S_INST, &key_index_1->value, sizeof(S_INST));
    }
    if (NULL != key_index_2)
    {
        memcpy(S_INST2, &key_index_2->value, sizeof(S_INST2));
    }

    return error_code;
}
/**************************
 End of function R_TSIP_Open
 **************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_Close
*******************************************************************************************************************/ /**
* @details       Stop power to the TSIP
* @retval        TSIP_SUCCESS: Normal termination.
* @note          None
*/
TSIP_SEC_P_SECURE_BOOT
e_tsip_err_t R_TSIP_Close(void)
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
    /* Casting structure pointer is used for address. */
    MSTP_SECURITY = 1u;
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);
    return TSIP_SUCCESS;
}
/***************************
 End of function R_TSIP_Close
 ***************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_SoftwareReset
*******************************************************************************************************************/ /**
* @details       Software reset to TSIP
* @note          None
*/
TSIP_SEC_P_SECURE_BOOT
void R_TSIP_SoftwareReset(void)
{
    R_TSIP_SoftwareResetSub();
    return;
}
/***********************************
 End of function R_TSIP_SoftwareReset
 ***********************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_SelfCheck1Private
*******************************************************************************************************************/ /**
* @details       Self check No1 for TSIP
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @see           R_TSIP_SelfCheck1Sub()
* @note          None
*/
TSIP_SEC_P_SECURE_BOOT
static e_tsip_err_t R_TSIP_SelfCheck1Private(void)
{
    return R_TSIP_SelfCheck1Sub();
}
/***************************************
 End of function R_TSIP_SelfCheck1Private
 ***************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_SelfCheck2Private
*******************************************************************************************************************/ /**
* @details       Self check No2 for TSIP
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_RETRY: self-test2 fail
* @see           R_TSIP_SelfCheck2Sub()
* @note          None
*/
TSIP_SEC_P_SECURE_BOOT
static e_tsip_err_t R_TSIP_SelfCheck2Private(void)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;

    R_BSP_InterruptsDisable();
    error_code = R_TSIP_SelfCheck2Sub();
    R_BSP_InterruptsEnable();

    return error_code;
}
/***************************************
 End of function R_TSIP_SelfCheck2Private
 ***************************************/

/***********************************************************************************************************************
* Function Name: firm_mac_read
*******************************************************************************************************************/ /**
* @details       Read the firmware mac value.
* @param[in]     InData_Program MAC value reading pointer
* @note          None
*/
TSIP_SEC_P_SECURE_BOOT
void firm_mac_read(uint32_t *InData_Program)
{
    memcpy(InData_Program, sp_user_program_mac, 16u);
    return;
}
/****************************
 End of function firm_mac_read
 ****************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_VerifyFirmwareMAC
*******************************************************************************************************************/ /**
* @details       Verify the MAC value using firmware.
* @param[in]     InData_Program Firmware
* @param[in]     MAX_CNT Word size of firmware.
* @param[in]     InData_MAC MAC value to be compared (16byte)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           R_TSIP_VerifyFirmwareMacSub()
* @note          None
*/
TSIP_SEC_P_SECURE_BOOT
e_tsip_err_t R_TSIP_VerifyFirmwareMAC(uint32_t *InData_Program, uint32_t MAX_CNT, uint32_t *InData_MAC)
{
    if (0u == MAX_CNT)
    {
        return TSIP_ERR_PARAMETER;
    }

    sp_user_program_mac = InData_MAC;
    return R_TSIP_VerifyFirmwareMacSub(InData_Program, MAX_CNT);
}
/***************************************
 End of function R_TSIP_VerifyFirmwareMAC
 ***************************************/

#if TSIP_FIRMWARE_UPDATE == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_StartUpdateFirmware
*******************************************************************************************************************/ /**
* @details       State Transit to the Firm Update State.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @see           R_TSIP_StartUpdateFirmwareSub()
* @note          None
*/
TSIP_SEC_P_SECURE_BOOT
e_tsip_err_t R_TSIP_StartUpdateFirmware(void)
{
    return R_TSIP_StartUpdateFirmwareSub();
}
/*****************************************
 End of function R_TSIP_StartUpdateFirmware
 *****************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateFirmwareMAC
*******************************************************************************************************************/ /**
* @details       Decrypt the firmware and generates new MAC for the encrypted firmware and the firmware checksum value.
* @param[in]     InData_KeyIndex key Generation Information Area for generating MAC value and decrypting the encrypted
* user program.
* @param[in]     InData_SessionKey Session Key area for confirming checksum.
* @param[in]     InData_UpProgram Temporaly Area for storing encrypted firmware data.
* @param[in]     InData_IV  Initial vector area for decrypting the encrypted firmware.
* @param[out]    OutData_Program Temporaly Area for storing decrypted firmware data.
* @param[in]     MAX_CNT The word size for encrypted firmware.
* @param[in]     p_callback It is called multiple times when user's action is required. The contents of teh action is
* determined by teh enum TSIP_FW_CB_REQ_TYPE.
* @param[in]     tsip_firmware_generate_mac_resume_handle The handle of work area for resume firmware update.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_CALLBACK_UNREGIST: Callback function is not registered
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_RESUME_FIRMWARE_GENERATE_MAC: Continuation of R_TSIP_GenerateFirmwareMAC
* @see           R_TSIP_GenerateFirmwareMacSub()
* @note          None
*/
TSIP_SEC_P_SECURE_BOOT
e_tsip_err_t R_TSIP_GenerateFirmwareMAC(uint32_t *InData_KeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_UpProgram, uint32_t *InData_IV, uint32_t *OutData_Program, uint32_t MAX_CNT,
        TSIP_GEN_MAC_CB_FUNC_T p_callback,
        tsip_firmware_generate_mac_resume_handle_t *tsip_firmware_generate_mac_resume_handle)
{
    if (NULL == p_callback)
    {
        return TSIP_ERR_CALLBACK_UNREGIST;
    }

    if (0u == MAX_CNT)
    {
        return TSIP_ERR_PARAMETER;
    }

    TSIP_GEN_MAC_CB_FUNC = p_callback;
    return R_TSIP_GenerateFirmwareMacSub(InData_KeyIndex, InData_SessionKey, InData_UpProgram, InData_IV,
            OutData_Program, MAX_CNT, tsip_firmware_generate_mac_resume_handle);
}
#endif /* TSIP_FIRMWARE_UPDATE == 1 */
/*****************************************
 End of function R_TSIP_GenerateFirmwareMAC
 *****************************************/
TSIP_SEC_DEFAULT

/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateRandomNumber
*******************************************************************************************************************/ /**
* @details       This API can generate 4 word random number.
* @param[out]    random Stores 4words (16 bytes) random data.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @see           R_TSIP_GenerateRandomNumberPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateRandomNumber(uint32_t *random)
{
    return R_TSIP_GenerateRandomNumberPrivate(random);
}
/******************************************
 End of function R_TSIP_GenerateRandomNumber
 ******************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateUpdateKeyRingKeyIndex
*******************************************************************************************************************/ /**
* @details       The API for outputting the key generation information of the key update key ring.
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the key update key ring encrypted with AES128-ECB mode
* @param[out]    key_index Output the key update key ring Generation Information (17 words)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @see           R_TSIP_SelfCheck1Private()
* @see           R_TSIP_SelfCheck2Private()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateUpdateKeyRingKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_update_key_ring_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateUpdateKeyRingKeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_UPDATE_KEY_RING;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/******************************************
 End of function R_TSIP_GenerateUpdateKeyRingKeyIndex
 ******************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_GetVersion
*******************************************************************************************************************/ /**
* @details       Returns the version of this module. The version number is encoded such that the top two bytes are the
* major version number and the bottom two bytes are the minor version number.
* @return        Version number
* @note          None
*/
uint32_t R_TSIP_GetVersion(void)
{
    return ((TSIP_VERSION_MAJOR << 16u) | TSIP_VERSION_MINOR);
}
/*********************************
 End of function  R_TSIP_GetVersion
 *********************************/

