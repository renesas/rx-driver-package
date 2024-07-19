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
 * Copyright (C) 2015-2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_tsip_rx_private.h
 * Version      : 1.18
 * Description  : TSIP-Lite function private header file.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 01.10.2015 1.00     First Release
 *         : 15.05.2017 1.01     Add AES-CMAC
 *         : 30.09.2017 1.03     Change API name for TSIP V.1.03.
 *         : 28.02.2017 1.04     Change API to Init/Update/Final API
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
#include "r_tsip_rx_if.h"
#include "iodefine/r_tsip_rx26t_iodefine.h"

#ifndef R_TSIP_RX_PRIVATE_HEADER_FILE
#define R_TSIP_RX_PRIVATE_HEADER_FILE

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/* Depends on configuration */
#define TSIP_PRV_USE_AES_128_ECB_CBC_CTR (TSIP_AES_128_ECB_ENCRYPT || TSIP_AES_128_ECB_DECRYPT \
        || TSIP_AES_128_CBC_ENCRYPT || TSIP_AES_128_CBC_DECRYPT || TSIP_AES_128_CTR)
#define TSIP_PRV_USE_AES_256_ECB_CBC_CTR (TSIP_AES_256_ECB_ENCRYPT || TSIP_AES_256_ECB_DECRYPT \
        || TSIP_AES_256_CBC_ENCRYPT || TSIP_AES_256_CBC_DECRYPT || TSIP_AES_256_CTR)

#define TSIP_PRV_USE_AES_128_CCM (TSIP_AES_128_CCM_ENCRYPT || TSIP_AES_128_CCM_DECRYPT)
#define TSIP_PRV_USE_AES_256_CCM (TSIP_AES_256_CCM_ENCRYPT || TSIP_AES_256_CCM_DECRYPT)
#define TSIP_PRV_USE_AES_CCM     (TSIP_PRV_USE_AES_128_CCM || TSIP_PRV_USE_AES_256_CCM)

#define TSIP_PRV_USE_AES_128_GCM (TSIP_AES_128_GCM_ENCRYPT || TSIP_AES_128_GCM_DECRYPT)
#define TSIP_PRV_USE_AES_256_GCM (TSIP_AES_256_GCM_ENCRYPT || TSIP_AES_256_GCM_DECRYPT)
#define TSIP_PRV_USE_AES_GCM     (TSIP_PRV_USE_AES_128_GCM || TSIP_PRV_USE_AES_256_GCM)

#define TSIP_PRV_USE_AES_128 (TSIP_PRV_USE_AES_128_ECB_CBC_CTR || TSIP_AES_128_CMAC || TSIP_PRV_USE_AES_128_CCM \
        || TSIP_PRV_USE_AES_128_GCM || TSIP_AES_128_KEY_WRAP)
#define TSIP_PRV_USE_AES_256 (TSIP_PRV_USE_AES_256_ECB_CBC_CTR || TSIP_AES_256_CMAC || TSIP_PRV_USE_AES_256_CCM \
        || TSIP_PRV_USE_AES_256_GCM || TSIP_AES_256_KEY_WRAP)

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/
extern uint32_t const S_FLASH[];

extern uint32_t S_RAM[R_TSIP_SRAM_WORD_SIZE];
extern uint32_t S_INST[R_TSIP_SINST_WORD_SIZE];
extern uint32_t S_INST2[R_TSIP_SINST2_WORD_SIZE];

extern TSIP_GEN_MAC_CB_FUNC_T TSIP_GEN_MAC_CB_FUNC;

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
/* --------------------- TSIP driver wrapper layer ---------------------- */

e_tsip_err_t R_TSIP_GenerateRandomNumberPrivate(uint32_t *random);

e_tsip_err_t R_TSIP_Aes128EcbEncryptInitPrivate(tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes128EcbEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128EcbEncryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);
e_tsip_err_t R_TSIP_Aes128EcbDecryptInitPrivate(tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes128EcbDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128EcbDecryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);
e_tsip_err_t R_TSIP_Aes128CbcEncryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_Aes128CbcEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128CbcEncryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);
e_tsip_err_t R_TSIP_Aes128CbcDecryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_Aes128CbcDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128CbcDecryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);

e_tsip_err_t R_TSIP_Aes256EcbEncryptInitPrivate(tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes256EcbEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256EcbEncryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);
e_tsip_err_t R_TSIP_Aes256EcbDecryptInitPrivate(tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes256EcbDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256EcbDecryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);
e_tsip_err_t R_TSIP_Aes256CbcEncryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_Aes256CbcEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256CbcEncryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);
e_tsip_err_t R_TSIP_Aes256CbcDecryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_Aes256CbcDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256CbcDecryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);

e_tsip_err_t R_TSIP_Aes128CtrInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_Aes256CtrInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_Aes128CtrUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256CtrUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128CtrFinalPrivate();
e_tsip_err_t R_TSIP_Aes256CtrFinalPrivate();

e_tsip_err_t R_TSIP_Aes128GcmEncryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_Aes128GcmEncryptUpdateAadPrivate(uint32_t *InData_DataA, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128GcmEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT, uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Aes128GcmEncryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_DataALen,
        uint32_t *InData_TextLen, uint32_t *OutData_Text, uint32_t *OutData_DataT);
e_tsip_err_t R_TSIP_Aes128GcmDecryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_Aes128GcmDecryptUpdateAadPrivate(uint32_t *InData_DataA, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128GcmDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT, uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Aes128GcmDecryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataALen, uint32_t *InData_TextLen, uint32_t *InData_DataTLen, uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Aes256GcmEncryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_Aes256GcmEncryptUpdateAadPrivate(uint32_t *InData_DataA, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256GcmEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT, uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Aes256GcmEncryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_DataALen,
        uint32_t *InData_TextLen, uint32_t *OutData_Text, uint32_t *OutData_DataT);
e_tsip_err_t R_TSIP_Aes256GcmDecryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_Aes256GcmDecryptUpdateAadPrivate(uint32_t *InData_DataA, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256GcmDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT, uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Aes256GcmDecryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataALen, uint32_t *InData_TextLen, uint32_t *InData_DataTLen, uint32_t *OutData_Text);
void         R_TSIP_AesGcmEncryptUpdateTransitionPrivate(void);
void         R_TSIP_AesGcmDecryptUpdateTransitionPrivate(void);

e_tsip_err_t R_TSIP_Aes128CcmEncryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV,
        uint32_t *InData_Header, uint32_t Header_Len);
e_tsip_err_t R_TSIP_Aes128CcmEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128CcmEncryptFinalPrivate(uint32_t *InData_TextLen, uint32_t *InData_Text,
        uint32_t *OutData_Text, uint32_t *OutData_MAC);
e_tsip_err_t R_TSIP_Aes128CcmDecryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV,
        uint32_t *InData_Header, uint32_t Header_Len);
e_tsip_err_t R_TSIP_Aes128CcmDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128CcmDecryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_TextLen, uint32_t *InData_MAC,
        uint32_t *InData_MACLength, uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Aes256CcmEncryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV,
        uint32_t *InData_Header, uint32_t Header_Len);
e_tsip_err_t R_TSIP_Aes256CcmEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256CcmEncryptFinalPrivate(uint32_t *InData_TextLen, uint32_t *InData_Text,
        uint32_t *OutData_Text, uint32_t *OutData_MAC);
e_tsip_err_t R_TSIP_Aes256CcmDecryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV,
        uint32_t *InData_Header, uint32_t Header_Len);
e_tsip_err_t R_TSIP_Aes256CcmDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256CcmDecryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_TextLen, uint32_t *InData_MAC,
        uint32_t *InData_MACLength, uint32_t *OutData_Text);

e_tsip_err_t R_TSIP_Aes128CmacGenerateInitPrivate(tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes128CmacGenerateUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128CmacGenerateFinalPrivate(uint32_t All_Msg_Len, uint32_t *InData_Text,
        uint32_t *OutData_DataT);
e_tsip_err_t R_TSIP_Aes128CmacVerifyInitPrivate(tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes128CmacVerifyUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128CmacVerifyFinalPrivate(uint32_t All_Msg_Len, uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataTLen);
e_tsip_err_t R_TSIP_Aes256CmacGenerateInitPrivate(tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes256CmacGenerateUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256CmacGenerateFinalPrivate(uint32_t All_Msg_Len, uint32_t *InData_Text,
        uint32_t *OutData_DataT);
e_tsip_err_t R_TSIP_Aes256CmacVerifyInitPrivate(tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes256CmacVerifyUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256CmacVerifyFinalPrivate(uint32_t All_Msg_Len, uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataTLen);

e_tsip_err_t R_TSIP_Aes128KeyWrapPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_WrappedKeyType,
        uint32_t *InData_WrappedKeyIndex, uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Aes256KeyWrapPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_WrappedKeyType,
        uint32_t *InData_WrappedKeyIndex, uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Aes128KeyUnWrapPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_WrappedKeyType,
        uint32_t *InData_Text, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_Aes256KeyUnWrapPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_WrappedKeyType,
        uint32_t *InData_Text, uint32_t *OutData_KeyIndex);

/* --------------------- TSIP control procedure related ---------------------- */

void         R_TSIP_SoftwareResetSub(void);
e_tsip_err_t R_TSIP_SelfCheck1Sub(void);
e_tsip_err_t R_TSIP_SelfCheck2Sub(void);

e_tsip_err_t R_TSIP_GenerateAes128KeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateAes256KeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateAes128RandomKeyIndexSub(uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateAes256RandomKeyIndexSub(uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateRandomNumberSub(uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_GenerateUpdateKeyRingKeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_InstData);

e_tsip_err_t R_TSIP_UpdateAes128KeyIndexSub(uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_UpdateAes256KeyIndexSub(uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);

e_tsip_err_t R_TSIP_StartUpdateFirmwareSub(void);
e_tsip_err_t R_TSIP_GenerateFirmwareMacSub(uint32_t *InData_KeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_UpProgram, uint32_t *InData_IV, uint32_t *OutData_Program, uint32_t MAX_CNT,
        tsip_firmware_generate_mac_resume_handle_t *tsip_firmware_generate_mac_resume_handle);
e_tsip_err_t R_TSIP_VerifyFirmwareMacSub(uint32_t *InData_Program, uint32_t MAX_CNT);

e_tsip_err_t R_TSIP_Aes128EncryptDecryptInitSub(uint32_t *InData_Cmd, uint32_t *InData_KeyIndex, uint32_t *InData_IV);
void         R_TSIP_Aes128EncryptDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128EncryptDecryptFinalSub(void);
e_tsip_err_t R_TSIP_Aes256EncryptDecryptInitSub(uint32_t *InData_Cmd, uint32_t *InData_KeyIndex, uint32_t *InData_IV);
void         R_TSIP_Aes256EncryptDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256EncryptDecryptFinalSub(void);

e_tsip_err_t R_TSIP_Ghash(uint32_t *InData_HV, uint32_t *InData_IV, uint32_t *InData_Text, uint32_t *OutData_DataT,
        uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128GcmEncryptInitSub(uint32_t *InData_KeyIndex, uint32_t *InData_IV);
void         R_TSIP_Aes128GcmEncryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128GcmEncryptFinalSub(uint32_t *InData_Text, uint32_t *InData_DataALen,
        uint32_t *InData_TextLen, uint32_t *OutData_Text, uint32_t *OutData_DataT);
e_tsip_err_t R_TSIP_Aes128GcmDecryptInitSub(uint32_t *InData_KeyIndex, uint32_t *InData_IV);
void         R_TSIP_Aes128GcmDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128GcmDecryptFinalSub(uint32_t *InData_Text, uint32_t *InData_DataT, uint32_t *InData_DataALen,
        uint32_t *InData_TextLen, uint32_t *InData_DataTLen, uint32_t *OutData_Text);
void         R_TSIP_AesGcmEncryptDecryptUpdateTransitionSub(void);
void         R_TSIP_Aes128GcmEncryptUpdateAADSub(uint32_t *InData_DataA, uint32_t MAX_CNT);
void         R_TSIP_Aes128GcmDecryptUpdateAADSub(uint32_t *InData_DataA, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256GcmEncryptInitSub(uint32_t *InData_KeyIndex, uint32_t *InData_IV);
void         R_TSIP_Aes256GcmEncryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256GcmEncryptFinalSub(uint32_t *InData_Text, uint32_t *InData_DataALen,
        uint32_t *InData_TextLen, uint32_t *OutData_Text, uint32_t *OutData_DataT);
e_tsip_err_t R_TSIP_Aes256GcmDecryptInitSub(uint32_t *InData_KeyIndex, uint32_t *InData_IV);
void         R_TSIP_Aes256GcmDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256GcmDecryptFinalSub(uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataALen, uint32_t *InData_TextLen, uint32_t *InData_DataTLen, uint32_t *OutData_Text);
void         R_TSIP_Aes256GcmEncryptUpdateAADSub(uint32_t *InData_DataA, uint32_t MAX_CNT);
void         R_TSIP_Aes256GcmDecryptUpdateAADSub(uint32_t *InData_DataA, uint32_t MAX_CNT);

e_tsip_err_t R_TSIP_Aes128CcmEncryptInitSub(uint32_t *InData_KeyIndex, uint32_t *InData_IV, uint32_t *InData_Header,
        uint32_t Header_Len);
void R_TSIP_Aes128CcmEncryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128CcmEncryptFinalSub(uint32_t *InData_TextLen, uint32_t *InData_Text, uint32_t *OutData_Text,
        uint32_t *OutData_MAC);
e_tsip_err_t R_TSIP_Aes128CcmDecryptInitSub(uint32_t *InData_KeyIndex, uint32_t *InData_IV, uint32_t *InData_Header,
        uint32_t Header_Len);
void R_TSIP_Aes128CcmDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128CcmDecryptFinalSub(uint32_t *InData_Text, uint32_t *InData_TextLen, uint32_t *InData_MAC,
        uint32_t *InData_MACLength, uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Aes256CcmEncryptInitSub(uint32_t *InData_KeyIndex, uint32_t *InData_IV, uint32_t *InData_Header,
        uint32_t Header_Len);
void R_TSIP_Aes256CcmEncryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256CcmEncryptFinalSub(uint32_t *InData_TextLen, uint32_t *InData_Text, uint32_t *OutData_Text,
        uint32_t *OutData_MAC);
e_tsip_err_t R_TSIP_Aes256CcmDecryptInitSub(uint32_t *InData_KeyIndex, uint32_t *InData_IV, uint32_t *InData_Header,
        uint32_t Header_Len);
void R_TSIP_Aes256CcmDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256CcmDecryptFinalSub(uint32_t *InData_Text, uint32_t *InData_TextLen, uint32_t *InData_MAC,
        uint32_t *InData_MACLength, uint32_t *OutData_Text);

e_tsip_err_t R_TSIP_Aes128CmacInitSub(uint32_t *InData_KeyIndex);
void         R_TSIP_Aes128CmacUpdateSub(uint32_t *InData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128CmacFinalSub(uint32_t *InData_Cmd, uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataTLen, uint32_t *OutData_DataT);
e_tsip_err_t R_TSIP_Aes256CmacInitSub(uint32_t *InData_KeyIndex);
void         R_TSIP_Aes256CmacUpdateSub(uint32_t *InData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256CmacFinalSub(uint32_t *InData_Cmd, uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataTLen, uint32_t *OutData_DataT);

e_tsip_err_t R_TSIP_AESKeyWrapSub(uint32_t *InData_Cmd, uint32_t *InData_KeyIndex, uint32_t KEY_INDEX_SIZE, uint32_t
        *InData_WrappedKeyType, uint32_t *InData_WrappedKeyIndex, uint32_t WRAPPED_KEY_SIZE, uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_AESKeyUnwrapSub(uint32_t *InData_Cmd, uint32_t *InData_KeyIndex, uint32_t KEY_INDEX_SIZE,
        uint32_t *InData_WrappedKeyType, uint32_t *InData_Text, uint32_t WRAPPED_KEY_SIZE, uint32_t *OutData_KeyIndex);

void R_TSIP_SelfCheck1SubSub(void);
void R_TSIP_SelfCheck2SubSub(void);

void RX26T_function001(uint32_t ARG1, uint32_t ARG2, uint32_t ARG3, uint32_t ARG4);
void RX26T_function002(uint32_t ARG1, uint32_t ARG2, uint32_t ARG3, uint32_t ARG4);
void RX26T_function003(uint32_t ARG1, uint32_t ARG2, uint32_t ARG3, uint32_t ARG4);
void RX26T_function030(void);
void RX26T_function205(void);
void RX26T_function206(void);
void RX26T_function207(void);

void firm_mac_read(uint32_t *InData_Program);

/**********************************************************************************************************************
 * Function Name: change_endian_long
 * Description  : change endian
 * Arguments    : data -
 *                    input data
 * Return Value : the data changing endian
 *********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE(change_endian_long)
uint32_t change_endian_long(uint32_t data)
{
#if defined __LIT
    /* Casting is valid because it matches the type to the right side or argument. */
    return R_BSP_REVL(data);
#else
    return data;
#endif  /* defined __LIT */
}
/*********************************
 End of function change_endian_long
 **********************************/

#endif /* R_TSIP_RX_PRIVATE_HEADER_FILE */
