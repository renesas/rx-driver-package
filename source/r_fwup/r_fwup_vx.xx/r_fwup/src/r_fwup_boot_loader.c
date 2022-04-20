/**********************************************************************************************************************
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
 * Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name     : r_fwup_boot_loader.c
 * Description   : Functions for the Firmware update module.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           16.02.2021 1.00    First Release
 *           19.05.2021 1.01    Added support for RX72N,RX66T,RX130
 *           08.07.2021 1.02    Added support for RX671 and GCC
 *           10.08.2021 1.03    Added support for IAR
 *********************************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include "r_fwup_config.h"  /* Firmware update config definitions */
#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == 2) /* FWUP_IMPLEMENTATION_AFRTOS */
#include "aws_iot_ota_agent.h"
#endif
#include "r_smc_entry.h"
#include "r_flash_rx_if.h"
#include "r_sci_rx_if.h"
#include "r_sys_time_rx_if.h"
#include "r_fwup_if.h"
#include "r_fwup_private.h"
#if (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1)
#include "r_flash_spi_if.h"
#endif /* (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1) */

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_BOOTLOADER)
#include "r_sci_rx_pinset.h"

#include "base64_decode.h"
#include "code_signer_public_key.h"

/* tinycrypt */
#include "tinycrypt/sha256.h"
#include "tinycrypt/ecc.h"
#include "tinycrypt/ecc_dsa.h"
#include "tinycrypt/constants.h"

#define BOOT_LOADER_SCI_CONTROL_BLOCK_A         (0)
#define BOOT_LOADER_SCI_CONTROL_BLOCK_B         (1)
#define BOOT_LOADER_SCI_CONTROL_BLOCK_TOTAL_NUM (2)

#define BOOT_LOADER_SCI_RECEIVE_BUFFER_EMPTY    (0)
#define BOOT_LOADER_SCI_RECEIVE_BUFFER_FULL     (1)

#define FLASH_DF_TOTAL_BLOCK_SIZE               (FLASH_DF_BLOCK_INVALID - FLASH_DF_BLOCK_0)

#define INTEGRITY_CHECK_SCHEME_HASH_SHA256_STANDALONE      "hash-sha256"
#define INTEGRITY_CHECK_SCHEME_SIG_SHA256_ECDSA_STANDALONE "sig-sha256-ecdsa"

typedef struct st_load_const_data_ctrl_block
{
    uint32_t flash_buffer[FWUP_WRITE_BLOCK_SIZE / 4];
    uint32_t offset;
    uint32_t progress;
} st_load_const_data_ctrl_block_t;

#if (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1)
extern flash_spi_status_t fwup_flash_spi_open(void);
extern flash_spi_status_t fwup_flash_spi_close(void);
extern flash_spi_status_t fwup_flash_spi_erase(flash_spi_erase_info_t *);
extern flash_spi_status_t fwup_flash_spi_write(uint8_t *, uint32_t, uint32_t);
extern flash_spi_status_t fwup_flash_spi_read(uint32_t, uint32_t, uint8_t *);
extern flash_spi_status_t fwup_flash_spi_polling_erase(void);
extern flash_spi_status_t fwup_flash_spi_polling_write(void);
#endif /* (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1) */

const uint8_t g_code_signer_public_key[] = CODE_SIGNER_PUBLIC_KEY_PEM;
const uint32_t g_code_signer_public_key_length = sizeof(g_code_signer_public_key);

uint32_t g_error_count1 = 0;
uint32_t g_error_count2 = 0;
uint32_t g_rcv_count1 = 0;
uint32_t g_rcv_count2 = 0;

/* Handle storage. */
static st_sci_receive_control_block_t s_sci_receive_control_block;
static st_sci_buffer_control_t s_sci_buffer_control[BOOT_LOADER_SCI_CONTROL_BLOCK_TOTAL_NUM];

/* static int32_t secure_boot(void); */
static int32_t firm_block_read (uint32_t * firmware, uint32_t offset);
static int32_t const_data_block_read (uint32_t * const_data, uint32_t offset);
static void bank_swap_with_software_reset (void);
static void software_reset (void);
static const uint8_t * get_status_string (uint8_t status);
#if (FLASH_CFG_CODE_FLASH_BGO == 1)
static void my_flash_callback (void * event);
#endif

static st_fwup_control_block_t * sp_fwup_control_block_bank0 =
        (st_fwup_control_block_t *)BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS;
static st_fwup_control_block_t * sp_fwup_control_block_bank1 =
        (st_fwup_control_block_t *)BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS;
static st_load_fw_control_block_t s_load_fw_control_block;
static st_load_const_data_ctrl_block_t s_load_const_data_control_block;
static uint32_t s_block_buffer[BOOT_LOADER_FLASH_CF_BLOCK_SIZE / 4];
static uint32_t s_flash_error_code;

/* CODE CHECKER, this is OK as a comment precedes the cast. */
static const uint8_t * sp_boot_loader_magic_code = (uint8_t*) BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS;


/***********************************************************************************************************************
 * Function Name: R_FWUP_SecureBoot
 * Description  :
 * Arguments    :
 * Return Value :
 **********************************************************************************************************************/
int32_t R_FWUP_SecureBoot(void)
{
    flash_err_t              flash_api_error_code   = FLASH_SUCCESS;
    int32_t                  secure_boot_error_code = FWUP_IN_PROGRESS;
    e_state_monitoring_err_t monitoring_error_code  = MONI_SUCCESS;  // 20201Q_No5
#if (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1)
    flash_spi_status_t     flash_spi_error_code = FLASH_SPI_SUCCESS;
    flash_spi_erase_info_t flash_spi_erase_info;
    uint32_t               flash_spi_erase_total_size;
#endif /* (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1) */
#if (FWUP_FLASH_BANK_MODE == 0)
    uint32_t bank_info = 255;
#else
    uint32_t tmp_offset;
    uint8_t  loop_cnt;
#endif /* FWUP_FLASH_BANK_MODE */
#if (FLASH_CFG_CODE_FLASH_BGO == 1)
    flash_interrupt_config_t cb_func_info;
#endif /* (FLASH_CFG_CODE_FLASH_BGO == 1) */

    st_fwup_control_block_t * p_fwup_control_block_tmp =
            (st_fwup_control_block_t *)s_block_buffer;
    int32_t verification_result = -1;

    /* Check that the R_FWUP_Open has been executed. */
    if (false == g_is_opened)
    {
        R_SCI_PinSet_serial_term();
        (void)fwup_communication_open();
        fwup_update_status(FWUP_STATE_FATAL_ERROR);
        DEBUG_LOG("ERROR: Not Open.\r\n");
        secure_boot_error_code = FWUP_ERR_ALREADY_OPEN;
        return secure_boot_error_code;
    }

    if ((FWUP_FAIL != secure_boot_error_code) &&
        (FWUP_ERR_COMM != secure_boot_error_code) &&
        (FWUP_ERR_STATE_MONITORING != secure_boot_error_code))
    {

        /* 20201Q_No5 start */
        /* If state transit error has occured, return FWUP_FAIL to main function */
        if (fwup_state_monitoring_is_error() == STATE_MONITORING_ERROR)
        {
            DEBUG_LOG("Boot status has not changed for 1 minute.\r\n");
            DEBUG_LOG("system error.\r\n");
            fwup_update_status(FWUP_STATE_FATAL_ERROR);
            secure_boot_error_code = FWUP_ERR_STATE_MONITORING;
        }    /* 20201Q_No5 end */

        switch (fwup_get_status())
        {
            case FWUP_STATE_INITIALIZING:
                R_SCI_PinSet_serial_term();

                s_load_fw_control_block.progress = 0;
                s_load_fw_control_block.offset   = 0;

                /* startup system */
                DEBUG_LOG("-------------------------------------------------\r\n");
#if (BSP_MCU_RX65N)
                DEBUG_LOG("RX65N secure boot program\r\n");
#elif (BSP_MCU_RX66T)
                DEBUG_LOG("RX66T secure boot program\r\n");
#elif (BSP_MCU_RX671)
                DEBUG_LOG("RX671 secure boot program\r\n");
#elif (BSP_MCU_RX72N)
                DEBUG_LOG("RX72N secure boot program\r\n");
#elif (BSP_MCU_RX231)
                DEBUG_LOG("RX231 secure boot program\r\n");
#elif (BSP_MCU_RX130)
                DEBUG_LOG("RX130 secure boot program\r\n");
#endif
                DEBUG_LOG("-------------------------------------------------\r\n");

                DEBUG_LOG("Checking flash ROM status.\r\n");

                DEBUG_LOG2("bank 0 status = 0x%x [%s]\r\n", sp_fwup_control_block_bank0->image_flag,
                        get_status_string(sp_fwup_control_block_bank0->image_flag));
                DEBUG_LOG2("bank 1 status = 0x%x [%s]\r\n", sp_fwup_control_block_bank1->image_flag,
                        get_status_string(sp_fwup_control_block_bank1->image_flag));

#if (FWUP_FLASH_BANK_MODE == 0)
                fwup_flash_get_bank_info((flash_bank_t *)&bank_info);
                DEBUG_LOG2("bank info = %d. (start bank = %d)\r\n", bank_info, (bank_info ^ 0x01));
#endif  /* (FWUP_FLASH_BANK_MODE == 0) */

#if (FLASH_CFG_CODE_FLASH_BGO == 1)
                cb_func_info.pcallback = my_flash_callback;
                cb_func_info.int_priority = FLASH_INTERRUPT_PRIORITY;
                fwup_flash_set_callback((void *)&cb_func_info);
#endif  /* (FLASH_CFG_CODE_FLASH_BGO == 1) */

                /* 20201Q_No5 start */
                /* Set System-timer for check status */
                monitoring_error_code = fwup_state_monitoring_start();
                if (MONI_SUCCESS != monitoring_error_code)
                {
                    DEBUG_LOG2("R_SYS_TIME_RegisterPeriodicCallback() returns error. %d.\r\n", monitoring_error_code);
                    DEBUG_LOG("system error.\r\n");
                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                    secure_boot_error_code = FWUP_FAIL;
                    break;
                }    /* 20201Q_No5 end */

                fwup_update_status(FWUP_STATE_BANK1_CHECK);
                break;

            case FWUP_STATE_BANK1_CHECK:
        
                /* Dual mode */
                if (LIFECYCLE_STATE_TESTING == sp_fwup_control_block_bank1->image_flag)
                {
                
                    /* CODE CHECKER, this is OK as a comment precedes the cast. */
                    memcpy(s_block_buffer, (void*)BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS,
                            BOOT_LOADER_FLASH_CF_BLOCK_SIZE);

                    DEBUG_LOG2("integrity check scheme = %-.32s\r\n",
                            sp_fwup_control_block_bank1->signature_type);
                    DEBUG_LOG("bank1(temporary area) on code flash integrity check...");

                    /* Firmware verification for the signature type. */
                    if (!strcmp((const char*)sp_fwup_control_block_bank1->signature_type,
                            INTEGRITY_CHECK_SCHEME_HASH_SHA256_STANDALONE))
                    {
                        uint8_t hash_sha256[TC_SHA256_DIGEST_SIZE];

                        /* Hash message */
                        struct tc_sha256_state_struct x_ctx;
                        tc_sha256_init( &x_ctx);
                        tc_sha256_update( &x_ctx,
                                (uint8_t*) BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS
                                        + BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
                                (BOOT_LOADER_FLASH_CF_BLOCK_SIZE * BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER)
                                        - BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH);
                        tc_sha256_final(hash_sha256, &x_ctx);
                        verification_result = memcmp(sp_fwup_control_block_bank1->signature, hash_sha256,
                                sizeof(hash_sha256));
                    }
                    else if (!strcmp((const char*)sp_fwup_control_block_bank1->signature_type,
                            INTEGRITY_CHECK_SCHEME_SIG_SHA256_ECDSA_STANDALONE))
                    {

                        /* CODE CHECKER: This multi-line statement needs a comment to identify a cast is used and
                         * describes why the loss of sign (int8_t instead of uint8_t) is OK. As it is multi-line
                         * a comment on the actual line is required. */
                        verification_result = fwup_verification_sha256_ecdsa(
                                (const uint8_t*) BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS
                                        + BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
                                (BOOT_LOADER_FLASH_CF_BLOCK_SIZE * BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER)
                                        - BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
                                sp_fwup_control_block_bank1->signature,
                                sp_fwup_control_block_bank1->signature_size);
                    }
                    else
                    {
                        verification_result = -1;
                    }

                    if (0 == verification_result)
                    {
                        DEBUG_LOG("OK\r\n");
                        p_fwup_control_block_tmp->image_flag = LIFECYCLE_STATE_VALID;
                    }
                    else
                    {
                        DEBUG_LOG("NG\r\n");
                        p_fwup_control_block_tmp->image_flag = LIFECYCLE_STATE_INVALID;
                    }
                    DEBUG_LOG2("update LIFECYCLE_STATE from [%s] to [%s]\r\n",
                            get_status_string(sp_fwup_control_block_bank1->image_flag),
                            get_status_string(p_fwup_control_block_tmp->image_flag));
                    DEBUG_LOG("bank1(temporary area) block0 erase (to update LIFECYCLE_STATE)...");

                    flash_api_error_code = fwup_flash_erase(
                            (flash_block_address_t) BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS, 1);
                    if (FLASH_SUCCESS != flash_api_error_code)
                    {
                        DEBUG_LOG2("R_FLASH_Erase() returns error. %d.\r\n", s_flash_error_code);
                        DEBUG_LOG("system error.\r\n");
                        fwup_update_status(FWUP_STATE_FATAL_ERROR);
                        secure_boot_error_code = FWUP_FAIL;
                        break; // @suppress("Case break statement")
                    }
#if (FLASH_CFG_CODE_FLASH_BGO == 1)
                    fwup_update_status(FWUP_STATE_BANK1_UPDATE_LIFECYCLE_ERASE_WAIT);
#else
                    fwup_update_status(FWUP_STATE_BANK1_UPDATE_LIFECYCLE_ERASE_COMPLETE);
#endif  /* (FLASH_CFG_CODE_FLASH_BGO == 1) */
                }
#if (FWUP_FLASH_BANK_MODE == 1) /* Linear mode */
                else if (LIFECYCLE_STATE_VALID == sp_fwup_control_block_bank1->image_flag)
                {
                
                    /* CODE CHECKER, this is OK as a comment precedes the cast. */
                    memcpy(s_block_buffer, (void*)BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS,
                            BOOT_LOADER_FLASH_CF_BLOCK_SIZE);

                    DEBUG_LOG2("integrity check scheme = %-.32s\r\n",
                            sp_fwup_control_block_bank1->signature_type);
                    DEBUG_LOG("temporary area on code flash integrity check...");

                    /* Firmware verification for the signature type. */
                    if ( !strcmp((const char* )sp_fwup_control_block_bank1->signature_type,
                            INTEGRITY_CHECK_SCHEME_HASH_SHA256_STANDALONE))
                    {
                        uint8_t hash_sha256[TC_SHA256_DIGEST_SIZE];

                        /* Hash message */
                        struct tc_sha256_state_struct x_ctx;
                        tc_sha256_init( &x_ctx);
                        tc_sha256_update( &x_ctx,
                                (uint8_t*) BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS
                                        + BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
                                (BOOT_LOADER_FLASH_CF_BLOCK_SIZE * BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER)
                                        - BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH);
                        tc_sha256_final(hash_sha256, &x_ctx);
                        verification_result = memcmp(sp_fwup_control_block_bank1->signature, hash_sha256,
                                sizeof(hash_sha256));
                    }
                    else if ( !strcmp((const char* )sp_fwup_control_block_bank1->signature_type,
                            INTEGRITY_CHECK_SCHEME_SIG_SHA256_ECDSA_STANDALONE))
                    {
                        verification_result = fwup_verification_sha256_ecdsa(
                                (const uint8_t*) BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS
                                        + BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
                                (BOOT_LOADER_FLASH_CF_BLOCK_SIZE * BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER)
                                        - BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
                                sp_fwup_control_block_bank1->signature,
                                sp_fwup_control_block_bank1->signature_size);
                    }
                    else
                    {
                        verification_result = -1;
                    }

                    if (0 == verification_result)
                    {
                        DEBUG_LOG("OK\r\n");
                    }
                    else
                    {
                        DEBUG_LOG("NG\r\n");
                        DEBUG_LOG("erase bank1 temporary area ...");
                        flash_api_error_code = fwup_flash_erase(
                                (flash_block_address_t) BOOT_LOADER_UPDATE_TEMPORARY_AREA_ERASE_ADDRESS,
                                BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER);
                        if (FLASH_SUCCESS != flash_api_error_code)
                        {
                            DEBUG_LOG2("R_FLASH_Erase() returns error. %d.\r\n", s_flash_error_code);
                            DEBUG_LOG("system error.\r\n");
                            fwup_update_status(FWUP_STATE_FATAL_ERROR);
                            secure_boot_error_code = FWUP_FAIL;
                            break; // @suppress("Case break statement")
                        }
                        DEBUG_LOG("OK\r\n");
                        DEBUG_LOG("software reset...\r\n");
                        R_BSP_SoftwareDelay(3000, BSP_DELAY_MILLISECS);
                        software_reset();
                        while (1);
                    }

#if(FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 0)
                    DEBUG_LOG("erase bank0 execute area ...");
                    flash_api_error_code = fwup_flash_erase(
                            (flash_block_address_t) BOOT_LOADER_UPDATE_EXECUTE_AREA_ERASE_ADDRESS,
                            BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER);
                    if (FLASH_SUCCESS != flash_api_error_code)
                    {
                        DEBUG_LOG2("R_FLASH_Erase() returns error. %d.\r\n", s_flash_error_code);
                        DEBUG_LOG("system error.\r\n");
                        fwup_update_status(FWUP_STATE_FATAL_ERROR);
                        secure_boot_error_code = FWUP_FAIL;
                        break; // @suppress("Case break statement")
                    }
                    DEBUG_LOG("OK\r\n");

                    DEBUG_LOG("copy the temporary area to the execution area ...");
                
                    /* CODE CHECKER, this is OK as a comment precedes the cast. */
                    tmp_offset = (uint32_t) BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS;
                    for (loop_cnt = 0; BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER > loop_cnt; loop_cnt++)
                    {
                    
                        /* CODE CHECKER, this is OK as a comment precedes the cast. */
                        memcpy(s_block_buffer, (void* )tmp_offset, BOOT_LOADER_FLASH_CF_BLOCK_SIZE);
                        flash_api_error_code = fwup_flash_write((uint32_t) s_block_buffer,
                            (uint32_t) (BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS +
                                (BOOT_LOADER_FLASH_CF_BLOCK_SIZE * loop_cnt)), BOOT_LOADER_FLASH_CF_BLOCK_SIZE);
                        if (FLASH_SUCCESS != flash_api_error_code)
                        {
                            break;
                        }
                        tmp_offset = tmp_offset + BOOT_LOADER_FLASH_CF_BLOCK_SIZE;
                    }
                    if (FLASH_SUCCESS != flash_api_error_code)
                    {
                        DEBUG_LOG2("R_FLASH_Write() returns error. %d.\r\n", s_flash_error_code);
                        DEBUG_LOG("system error.\r\n");
                        fwup_update_status(FWUP_STATE_FATAL_ERROR);
                        secure_boot_error_code = FWUP_FAIL;
                        break;
                    }
                    DEBUG_LOG("OK\r\n");
#elif(FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1) /* (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 0) */

#endif /* (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 0) */

                    DEBUG_LOG2("integrity check scheme = %-.32s\r\n",
                            sp_fwup_control_block_bank0->signature_type);
                    DEBUG_LOG("execution area on code flash integrity check...");

                    /* Firmware verification for the signature type. */
                    if ( !strcmp((const char* )sp_fwup_control_block_bank0->signature_type,
                            INTEGRITY_CHECK_SCHEME_HASH_SHA256_STANDALONE))
                    {
                        uint8_t hash_sha256[TC_SHA256_DIGEST_SIZE];

                        /* Hash message */
                        struct tc_sha256_state_struct x_ctx;
                        tc_sha256_init( &x_ctx);
                        tc_sha256_update( &x_ctx,
                                (uint8_t*) BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS
                                        + BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
                                (BOOT_LOADER_FLASH_CF_BLOCK_SIZE * BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER)
                                        - BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH);
                        tc_sha256_final(hash_sha256, &x_ctx);
                        verification_result = memcmp(sp_fwup_control_block_bank0->signature, hash_sha256,
                                sizeof(hash_sha256));
                    }
                    else if ( !strcmp((const char* )sp_fwup_control_block_bank0->signature_type,
                            INTEGRITY_CHECK_SCHEME_SIG_SHA256_ECDSA_STANDALONE))
                    {
                        verification_result = fwup_verification_sha256_ecdsa(
                                (const uint8_t*) BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS
                                        + BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
                                (BOOT_LOADER_FLASH_CF_BLOCK_SIZE * BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER)
                                        - BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
                                sp_fwup_control_block_bank0->signature,
                                sp_fwup_control_block_bank0->signature_size);
                    }
                    else
                    {
                        verification_result = -1;
                    }

                    if (0 == verification_result)
                    {
                        DEBUG_LOG("OK\r\n");
                        DEBUG_LOG("erase bank1 temporary area ...");
                        flash_api_error_code = fwup_flash_erase(
                                (flash_block_address_t) BOOT_LOADER_UPDATE_TEMPORARY_AREA_ERASE_ADDRESS,
                                BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER);
                        if (FLASH_SUCCESS != flash_api_error_code)
                        {
                            DEBUG_LOG2("R_FLASH_Erase() returns error. %d.\r\n", s_flash_error_code);
                            DEBUG_LOG("system error.\r\n");
                            fwup_update_status(FWUP_STATE_FATAL_ERROR);
                            secure_boot_error_code = FWUP_FAIL;
                            break; // @suppress("Case break statement")
                        }
                        DEBUG_LOG("OK\r\n");
                    }
                    else
                    {
                        DEBUG_LOG("NG\r\n");
                        fwup_update_status(FWUP_STATE_FATAL_ERROR);
                        secure_boot_error_code = FWUP_FAIL;
                        break; // @suppress("Case break statement")
                    }

                    DEBUG_LOG("jump to user program\r\n");
                    R_BSP_SoftwareDelay(1000, BSP_DELAY_MILLISECS);
                    fwup_update_status(FWUP_STATE_BANK1_UPDATE_LIFECYCLE_ERASE_COMPLETE);
                    secure_boot_error_code = FWUP_SUCCESS;
                    break; // @suppress("Case break statement")
                }
                else if (LIFECYCLE_STATE_EOL == sp_fwup_control_block_bank1->image_flag)
                {
                    DEBUG_LOG("End Of Life process start.\r\n");

#if (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1)
                    flash_spi_error_code = fwup_flash_spi_polling_erase();
                    if(FLASH_SPI_SUCCESS_BUSY == flash_spi_error_code)
                    {
                        /* Check serail flash erase polling continuously */
                        fwup_update_status(FWUP_STATE_EOL_BANK1_ERASE_WAIT);
                    }
                    else if (FLASH_SPI_SUCCESS == flash_spi_error_code)
                    {
                        /* Serial flash erase finished */
                        if ((BOOT_LOADER_TOTAL_UPDATE_SIZE - flash_spi_erase_total_size) <
                            BOOT_LOADER_SERIAL_FLASH_BLOCK_SIZE)
                        {
                            /* last erase block of serial flash */
                            flash_spi_erase_total_size = BOOT_LOADER_TOTAL_UPDATE_SIZE;
                        }
                        else
                        {
                            flash_spi_erase_total_size += BOOT_LOADER_SERIAL_FLASH_BLOCK_SIZE;
                        }
                        if (flash_spi_erase_total_size != BOOT_LOADER_TOTAL_UPDATE_SIZE)
                        {
                            /* serial flash erase is not finished */
                            /* Set start address of serial flash */
                            flash_spi_erase_info.addr = BOOT_LOADER_SERIAL_FLASH_START_ADDRESS +
                                    flash_spi_erase_total_size;
                            flash_spi_erase_info.mode = FLASH_SPI_MODE_B64K_ERASE; /* Set serial flash erase mode */
                            flash_spi_error_code = fwup_flash_spi_erase(&flash_spi_erase_info);
                            if(FLASH_SPI_SUCCESS != flash_spi_error_code)
                            {
                                DEBUG_LOG2("R_FLASH_SPI_Erase() returns error. %d.\r\n", flash_spi_error_code);
                                DEBUG_LOG("system error.\r\n");
                                fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                secure_boot_error_code = FWUP_FAIL;
                                break; // @suppress("Case break statement")
                            }
                            fwup_update_status(FWUP_STATE_EOL_BANK1_ERASE_WAIT);
                        }
                        else
                        {
                            fwup_update_status(FWUP_STATE_EOL_BANK1_ERASE_COMPLETE);
                        }
                    }
                    else
                    {
                        DEBUG_LOG("NG\r\n");
                        DEBUG_LOG2("R_FLASH_SPI_Polling() returns error code. %d.\r\n", flash_spi_error_code);
                        DEBUG_LOG("system error.\r\n");
                        fwup_update_status(FWUP_STATE_FATAL_ERROR);
                        secure_boot_error_code = FWUP_FAIL;
                        break; // @suppress("Case break statement")
                    }
#else /* F(FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1) */
                    /* CODE CHECKER, this is OK as a comment precedes the cast. */
                    memcpy(s_block_buffer, (void*)BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS,
                            BOOT_LOADER_FLASH_CF_BLOCK_SIZE);
                    p_fwup_control_block_tmp->image_flag = LIFECYCLE_STATE_EOL;
                    DEBUG_LOG2("update LIFECYCLE_STATE from [%s] to [%s]\r\n",
                            get_status_string(sp_fwup_control_block_bank0->image_flag),
                            get_status_string(p_fwup_control_block_tmp->image_flag));
                    DEBUG_LOG("bank0(Excecute area) block0 write (to update LIFECYCLE_STATE)...");
                    flash_api_error_code = fwup_flash_erase(
                            (flash_block_address_t) BOOT_LOADER_UPDATE_EXECUTE_AREA_ERASE_ADDRESS,
                            BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER);
                    if (FLASH_SUCCESS != flash_api_error_code)
                    {
                        DEBUG_LOG("NG.\r\n");
                        DEBUG_LOG2("R_FLASH_Erase() returns error. %d.\r\n", s_flash_error_code);
                        DEBUG_LOG("system error.\r\n");
                        fwup_update_status(FWUP_STATE_FATAL_ERROR);
                        secure_boot_error_code = FWUP_FAIL;
                        break; // @suppress("Case break statement")
                    }

                    flash_api_error_code = fwup_flash_write((uint32_t) p_fwup_control_block_tmp,
                            (uint32_t) BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS, BOOT_LOADER_FLASH_CF_BLOCK_SIZE);
                    if (FLASH_SUCCESS == flash_api_error_code)
                    {
                        DEBUG_LOG("OK\r\n");
                        DEBUG_LOG2("bank 0 status = 0x%x [%s]\r\n", sp_fwup_control_block_bank0->image_flag,
                                get_status_string(sp_fwup_control_block_bank0->image_flag));
                        DEBUG_LOG2("bank 1 status = 0x%x [%s]\r\n", sp_fwup_control_block_bank1->image_flag,
                                get_status_string(sp_fwup_control_block_bank1->image_flag));
                    }
                    else
                    {
                        DEBUG_LOG2("R_FLASH_Write() returns error. %d.\r\n", s_flash_error_code);
                        DEBUG_LOG("system error.\r\n");
                        fwup_update_status(FWUP_STATE_FATAL_ERROR);
                        secure_boot_error_code = FWUP_FAIL;
                        break; // @suppress("Case break statement")
                    }

                    DEBUG_LOG("erase install area (data flash): ");
                    flash_api_error_code = fwup_flash_erase((flash_block_address_t) BOOT_LOADER_USER_CONST_DATA_LOW_ADDRESS,
                            BOOT_LOADER_UPDATE_CONST_DATA_TARGET_BLOCK_NUMBER);
                    if (FLASH_SUCCESS != flash_api_error_code)
                    {
                        DEBUG_LOG2("R_FLASH_Erase() returns error. %d.\r\n", s_flash_error_code);
                        DEBUG_LOG("system error.\r\n");
                        fwup_update_status(FWUP_STATE_FATAL_ERROR);
                        secure_boot_error_code = FWUP_FAIL;
                        break; // @suppress("Case break statement")
                    }

                    /* End of the EOL process, infinity loop on Bootloader */
                    DEBUG_LOG("OK\r\n");
                    DEBUG_LOG("End Of Life process finished.\r\n");
                    R_BSP_SoftwareDelay(1000, BSP_DELAY_MILLISECS);
                    secure_boot_error_code = FWUP_END_OF_LIFE;
                    break; // @suppress("Case break statement")

#endif /* F(FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1) */

                }
#endif /* (FWUP_FLASH_BANK_MODE == 1) */
                else
                {
                    if (LIFECYCLE_STATE_VALID == sp_fwup_control_block_bank0->image_flag)
                    {
                        fwup_update_status(FWUP_STATE_BANK0_UPDATE_CHECK);
                    }
                    else
                    {
                        fwup_update_status(FWUP_STATE_BANK0_CHECK);
                    }
                }
                break;

            case FWUP_STATE_BANK1_UPDATE_LIFECYCLE_ERASE_WAIT:

                /* this state will be update by flash callback */
                break;

            case FWUP_STATE_BANK1_UPDATE_LIFECYCLE_ERASE_COMPLETE:
                if (FLASH_SUCCESS == s_flash_error_code)
                {
                    DEBUG_LOG("OK\r\n");
                }
                else
                {
                    DEBUG_LOG2("R_FLASH_Erase() callback error. %d.\r\n", s_flash_error_code);
                    DEBUG_LOG("system error.\r\n");
                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                    secure_boot_error_code = FWUP_FAIL;
                    break; // @suppress("Case break statement")
                }
                DEBUG_LOG("bank1(temporary area) block0 write (to update LIFECYCLE_STATE)...");
                flash_api_error_code = fwup_flash_write((uint32_t) p_fwup_control_block_tmp,
                        (uint32_t) BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS, BOOT_LOADER_FLASH_CF_BLOCK_SIZE);
                if (FLASH_SUCCESS != flash_api_error_code)
                {
                    DEBUG_LOG2("R_FLASH_Write() returns error. %d.\r\n", s_flash_error_code);
                    DEBUG_LOG("system error.\r\n");
                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                    secure_boot_error_code = FWUP_FAIL;
                    break; // @suppress("Case break statement")
                }
#if (FLASH_CFG_CODE_FLASH_BGO == 1)
                fwup_update_status(FWUP_STATE_BANK1_UPDATE_LIFECYCLE_WRITE_WAIT);
#else
                fwup_update_status(FWUP_STATE_BANK1_UPDATE_LIFECYCLE_WRITE_COMPLETE);
#endif  /* (FLASH_CFG_CODE_FLASH_BGO == 1) */
                break;

            case FWUP_STATE_BANK1_UPDATE_LIFECYCLE_WRITE_WAIT:

                /* this state will be update by flash callback */
                break;

            case FWUP_STATE_BANK1_UPDATE_LIFECYCLE_WRITE_COMPLETE:
                if (FLASH_SUCCESS == s_flash_error_code)
                {
                    DEBUG_LOG("OK\r\n");
                }
                else
                {
                    DEBUG_LOG2("R_FLASH_Write() callback error. %d.\r\n", s_flash_error_code);
                    DEBUG_LOG("system error.\r\n");
                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                    secure_boot_error_code = FWUP_FAIL;
                    break; // @suppress("Case break statement")
                }

                if (LIFECYCLE_STATE_VALID == p_fwup_control_block_tmp->image_flag)
                {
                    DEBUG_LOG(
                            "swap bank...[FWUP_STATE_BANK1_UPDATE_LIFECYCLE_WRITE_COMPLETE and LIFECYCLE_STATE_VALID]\r\n");
                    R_BSP_SoftwareDelay(3000, BSP_DELAY_MILLISECS);
                    bank_swap_with_software_reset();
                    while (1)
                    {
                        ;
                    }
                }
                else
                {
                    DEBUG_LOG("software reset...\r\n");
                    R_BSP_SoftwareDelay(3000, BSP_DELAY_MILLISECS);
                    software_reset();
                    while (1)
                    {
                        ;
                    }
                }
                break;

            case FWUP_STATE_BANK0_CHECK:
            case FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_ERASE_WAIT:
            case FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_ERASE_COMPLETE:
            case FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_WRITE_WAIT:
            case FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_WRITE_COMPLETE:
            case FWUP_STATE_INSTALL_DATA_FLASH_ERASE_WAIT:
            case FWUP_STATE_INSTALL_DATA_FLASH_ERASE_COMPLETE:
            case FWUP_STATE_BANK0_INSTALL_CODE_FLASH_ERASE_WAIT:
            case FWUP_STATE_BANK0_INSTALL_CODE_FLASH_ERASE_COMPLETE:
            case FWUP_STATE_BANK0_INSTALL_CODE_FLASH_READ_WAIT:
            case FWUP_STATE_BANK0_INSTALL_CODE_FLASH_READ_COMPLETE:
            case FWUP_STATE_BANK0_INSTALL_CODE_FLASH_WRITE_WAIT:
            case FWUP_STATE_BANK0_INSTALL_CODE_FLASH_WRITE_COMPLETE:
            case FWUP_STATE_BANK0_INSTALL_SET_BOOT_PROTECT:
            case FWUP_STATE_INSTALL_DATA_FLASH_READ_WAIT:
            case FWUP_STATE_INSTALL_DATA_FLASH_READ_COMPLETE:
            case FWUP_STATE_INSTALL_DATA_FLASH_WRITE_WAIT:
            case FWUP_STATE_INSTALL_DATA_FLASH_WRITE_COMPLETE:
            case FWUP_STATE_BANK0_UPDATE_CHECK:
            case FWUP_STATE_BANK1_UPDATE_CODE_FLASH_ERASE_WAIT:
            case FWUP_STATE_BANK1_UPDATE_CODE_FLASH_ERASE_COMPLETE:
            case FWUP_STATE_EOL_BANK1_ERASE_WAIT:
            case FWUP_STATE_EOL_BANK1_ERASE_COMPLETE:
            case FWUP_STATE_EOL_BANK1_LIFECYCLE_WRITE_WAIT:
            case FWUP_STATE_EOL_BANK1_LIFECYCLE_WRITE_COMPLETE:
            case FWUP_STATE_EOL_DATA_FLASH_ERASE_WAIT:
            case FWUP_STATE_EOL_DATA_FLASH_ERASE_COMPLETE:
            case FWUP_STATE_FINALIZE:
                switch (sp_fwup_control_block_bank0->image_flag)
                {
                    case LIFECYCLE_STATE_BLANK:
                        switch (fwup_get_status())
                        {
                            case FWUP_STATE_BANK0_CHECK:
                                if (LIFECYCLE_STATE_BLANK == sp_fwup_control_block_bank1->image_flag)
                                {
                                    DEBUG_LOG("start installing user program.\r\n");
#if (FWUP_CFG_BOOT_PROTECT_ENABLE == 1)
                                    if (false == fwup_get_boot_protect())
                                    {
#if (FWUP_FLASH_BANK_MODE == 0) /* Dual mode */
                                        /* No Boot Protected */
                                        DEBUG_LOG("erase bank1 secure boot mirror area...");
                                        flash_api_error_code = fwup_flash_erase(BOOT_LOADER_MIRROR_ERASE_ADDRESS,
                                                BOOT_LOADER_MIRROR_BLOCK_NUMBER);
                                        if(FLASH_SUCCESS != flash_api_error_code)
                                        {
                                            DEBUG_LOG("NG\r\n");
                                            DEBUG_LOG2("R_FLASH_Erase() returns error code = %d.\r\n",
                                                    s_flash_error_code);
                                            fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                            secure_boot_error_code = FWUP_FAIL;
                                            break; // @suppress("Case break statement")
                                        }
                                        fwup_update_status(FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_ERASE_WAIT);
                                    }
                                    else
                                    {
                                        /* Already Boot Protected */
                                        /* Skip boot copy process */
                                        DEBUG_LOG("boot protected: skip copy secure boot from bank0 to bank1...");
                                        s_flash_error_code = FLASH_SUCCESS;
                                        fwup_update_status(FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_WRITE_COMPLETE);
#else /* Linear mode */
                                        /* No Boot Protected */
                                        /* Do not FLASH erase because the Bootloader is not copied */
                                        /* Skip to next status */
                                        fwup_update_status(FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_ERASE_COMPLETE);
                                    }
                                    else
                                    {
                                        /* Already Boot Protected */
                                        /* Skip boot copy process */
                                        DEBUG_LOG("already boot protected.\r\n");
                                        s_flash_error_code = FLASH_SUCCESS;
                                        fwup_update_status(FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_WRITE_COMPLETE);
#endif  /* (FWUP_FLASH_BANK_MODE == 0) */
                                    }
#else /* FWUP_CFG_BOOT_PROTECT_ENABLE == 1 */
#if (FWUP_FLASH_BANK_MODE == 0) /* Dual mode */
                                    DEBUG_LOG("erase bank1 secure boot mirror area...");
                                    flash_api_error_code
                                        = fwup_flash_erase((flash_block_address_t)BOOT_LOADER_MIRROR_ERASE_ADDRESS,
                                            (uint32_t)BOOT_LOADER_MIRROR_BLOCK_NUMBER);
                                    if (FLASH_SUCCESS != flash_api_error_code)
                                    {
                                        DEBUG_LOG("NG\r\n");
                                        DEBUG_LOG2("R_FLASH_Erase() returns error code = %d.\r\n", s_flash_error_code);
                                        fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                        secure_boot_error_code = FWUP_FAIL;
                                        break; // @suppress("Case break statement")
                                    }
#if (FLASH_CFG_DATA_FLASH_BGO == 1)
                                    fwup_update_status(FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_ERASE_WAIT);
#else  /* (FLASH_CFG_DATA_FLASH_BGO == 0) */
                                    fwup_update_status(FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_ERASE_COMPLETE);
#endif /* (FLASH_CFG_DATA_FLASH_BGO == 1) */
#else /* Linear mode */

                                    /* Do not FLASH erase because the Bootloader is not copied */
                                    /* Skip to next status */
                                    fwup_update_status(FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_ERASE_COMPLETE);
#endif /* (FWUP_FLASH_BANK_MODE) */
#endif /* (FWUP_CFG_BOOT_PROTECT_ENABLE == 1) */
                                }
#if (FWUP_FLASH_BANK_MODE == 0) /* Dual mode */
                                else if (LIFECYCLE_STATE_VALID == sp_fwup_control_block_bank1->image_flag)
                                {
                                    DEBUG_LOG("bank0(current) is blank, but bank1(previous) is still alive.\r\n");
                                    DEBUG_LOG("swap bank...");
                                    R_BSP_SoftwareDelay(3000, BSP_DELAY_MILLISECS);
                                    bank_swap_with_software_reset();
                                    while (1)
                                    {
                                        ;
                                    }
                                }
#endif /* (FWUP_FLASH_BANK_MODE) */
                                else
                                {
                                    DEBUG_LOG("user program installation failed.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                }
                                break;

                            case FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_ERASE_WAIT:

                                /* this state will be update by flash callback */
                                break;

                            case FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_ERASE_COMPLETE:
                                if (LIFECYCLE_STATE_INSTALLING != sp_fwup_control_block_bank1->image_flag)
                                {
                                    if (FLASH_SUCCESS == s_flash_error_code)
                                    {
#if (FWUP_FLASH_BANK_MODE == 0) /* Dual mode */
                                        DEBUG_LOG("OK\r\n");
#endif /* (FWUP_FLASH_BANK_MODE) */
                                    }
                                    else
                                    {
                                        DEBUG_LOG2("R_FLASH_Erase() callback error. %d.\r\n", s_flash_error_code);
                                        DEBUG_LOG("system error.\r\n");
                                        fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                        secure_boot_error_code = FWUP_FAIL;
                                        break; // @suppress("Case break statement")
                                    }
                                }
#if (FWUP_FLASH_BANK_MODE == 0) /* Dual mode */
                                DEBUG_LOG("copy secure boot from bank0 to bank1...");

                                flash_api_error_code = fwup_flash_write((uint32_t)BOOT_LOADER_LOW_ADDRESS,
                                    (uint32_t)BOOT_LOADER_MIRROR_LOW_ADDRESS,
                                    BOOT_LOADER_PGM_SIZE);
                                if (FLASH_SUCCESS != flash_api_error_code)
                                {
                                    DEBUG_LOG("NG\r\n");
                                    DEBUG_LOG2("R_FLASH_Write() returns error code = %d.\r\n", s_flash_error_code);
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break; // @suppress("Case break statement")
                                }
#if (FLASH_CFG_DATA_FLASH_BGO == 1)
                                fwup_update_status(FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_WRITE_WAIT);
#else  /* (FLASH_CFG_DATA_FLASH_BGO == 0) */
                                fwup_update_status(FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_WRITE_COMPLETE);
#endif /* (FLASH_CFG_DATA_FLASH_BGO == 1) */

#else /* Linear mode */
                                /* Do not FLASH write because the Bootloader is not copied */
                                /* Skip to next status */
                                fwup_update_status(FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_WRITE_COMPLETE);
#endif /* (FWUP_FLASH_BANK_MODE) */
                                break;

                            case FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_WRITE_WAIT:

                                /* this state will be update by flash callback */
                                break;

                            case FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_WRITE_COMPLETE:
                                if (FLASH_SUCCESS == s_flash_error_code)
                                {
#if (FWUP_FLASH_BANK_MODE == 0) /* Dual mode */
                                    DEBUG_LOG("OK\r\n");
#endif /* (FWUP_FLASH_BANK_MODE) */
                                }
                                else
                                {
                                    DEBUG_LOG2("R_FLASH_Write() callback error. %d.\r\n", s_flash_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break; // @suppress("Case break statement")
                                }
#if (FWUP_CFG_BOOT_PROTECT_ENABLE == 1)
                                if (false == fwup_get_boot_protect())
                                {
                                    flash_api_error_code = fwup_set_boot_protect();
                                    if (FLASH_SUCCESS != flash_api_error_code)
                                    {
                                        DEBUG_LOG2("R_FLASH_Control() returns error. %d.\r\n", flash_api_error_code);
                                        DEBUG_LOG("system error.\r\n");
                                        fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                        secure_boot_error_code = FWUP_FAIL;
                                        break; // @suppress("Case break statement")
                                    }
                                    else
                                    {
                                        DEBUG_LOG("set boot protect.\r\n");
                                    }
                                }
#endif
                                DEBUG_LOG("========== install user program phase ==========\r\n");
                                DEBUG_LOG("erase install area (data flash): ");
#if (FLASH_CFG_DATA_FLASH_BGO == 1)
                                flash_api_error_code = fwup_flash_erase(
                                    (flash_block_address_t)BOOT_LOADER_USER_CONST_DATA_LOW_ADDRESS,
                                    BOOT_LOADER_UPDATE_CONST_DATA_TARGET_BLOCK_NUMBER);
                                if (FLASH_SUCCESS != flash_api_error_code)
                                {
                                    DEBUG_LOG2("R_FLASH_Erase() returns error. %d.\r\n", s_flash_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break; // @suppress("Case break statement")
                                }
                                fwup_update_status(FWUP_STATE_INSTALL_DATA_FLASH_ERASE_WAIT);
#else  /* (FLASH_CFG_DATA_FLASH_BGO == 0) */
                                flash_api_error_code = fwup_flash_erase(
                                        (flash_block_address_t) BOOT_LOADER_USER_CONST_DATA_LOW_ADDRESS,
                                        BOOT_LOADER_UPDATE_CONST_DATA_TARGET_BLOCK_NUMBER);
                                if (FLASH_SUCCESS != flash_api_error_code)
                                {
                                    DEBUG_LOG2("R_FLASH_Erase() returns error. %d.\r\n", s_flash_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break; // @suppress("Case break statement")
                                }
                                fwup_update_status(FWUP_STATE_INSTALL_DATA_FLASH_ERASE_COMPLETE);
#endif  /* FLASH_CFG_DATA_FLASH_BGO */
                                break;

                            case FWUP_STATE_INSTALL_DATA_FLASH_ERASE_WAIT:

                                /* this state will be update by flash callback */
                                break;

                            case FWUP_STATE_INSTALL_DATA_FLASH_ERASE_COMPLETE:
                                if (FLASH_SUCCESS == s_flash_error_code)
                                {
                                    DEBUG_LOG("OK\r\n");
                                }
                                else
                                {
                                    DEBUG_LOG2("R_FLASH_Write() callback error. %d.\r\n", s_flash_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break; // @suppress("Case break statement")
                                }
                                DEBUG_LOG("erase install area (code flash): ");
#if (FLASH_CFG_CODE_FLASH_BGO == 1)
                                flash_api_error_code = fwup_flash_erase(
                                    (flash_block_address_t)BOOT_LOADER_UPDATE_TEMPORARY_AREA_ERASE_ADDRESS,
                                    BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER);
                                if (FLASH_SUCCESS != flash_api_error_code)
                                {
                                    DEBUG_LOG2("R_FLASH_Erase() returns error. %d.\r\n", s_flash_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break; // @suppress("Case break statement")
                                }
                                fwup_update_status(FWUP_STATE_BANK0_INSTALL_CODE_FLASH_ERASE_WAIT);
#else /* (FLASH_CFG_CODE_FLASH_BGO == 0) */
#if (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1)
                                flash_spi_erase_total_size = 0x0;
                                /* Set start address of serial flash */
                                flash_spi_erase_info.addr = BOOT_LOADER_SERIAL_FLASH_START_ADDRESS;
                                flash_spi_erase_info.mode = FLASH_SPI_MODE_B64K_ERASE; /* Set serial flash erase mode */
                                flash_spi_error_code = fwup_flash_spi_erase(&flash_spi_erase_info);
                                if(FLASH_SPI_SUCCESS != flash_spi_error_code)
                                {
                                    DEBUG_LOG2("R_FLASH_SPI_Erase() returns error. %d.\r\n", flash_spi_error_code);
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break;
                                }
                                fwup_update_status(FWUP_STATE_BANK0_INSTALL_CODE_FLASH_ERASE_WAIT);
#else /* (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 0) */
                                flash_api_error_code = fwup_flash_erase(
                                        (flash_block_address_t) BOOT_LOADER_UPDATE_TEMPORARY_AREA_ERASE_ADDRESS,
                                        BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER);
                                if (FLASH_SUCCESS != flash_api_error_code)
                                {
                                    DEBUG_LOG2("R_FLASH_Erase() returns error. %d.\r\n", s_flash_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break;
                                }
                                fwup_update_status(FWUP_STATE_BANK0_INSTALL_CODE_FLASH_ERASE_COMPLETE);
#endif /* FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER */
#endif  /* FLASH_CFG_CODE_FLASH_BGO == 1 */
                                break;

                            case FWUP_STATE_BANK0_INSTALL_CODE_FLASH_ERASE_WAIT:
#if (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1)
                                flash_spi_error_code = fwup_flash_spi_polling_erase();
                                if(FLASH_SPI_SUCCESS_BUSY == flash_spi_error_code)
                                {
                                    /* Check serail flash erase polling continuously */
                                    fwup_update_status(FWUP_STATE_BANK0_INSTALL_CODE_FLASH_ERASE_WAIT);
                                }
                                else if (FLASH_SPI_SUCCESS == flash_spi_error_code)
                                {
                                    /* Serial flash erase finished */
                                    if ((BOOT_LOADER_TOTAL_UPDATE_SIZE - flash_spi_erase_total_size) <
                                        BOOT_LOADER_SERIAL_FLASH_BLOCK_SIZE)
                                    {
                                        /* last erase block of serial flash */
                                        flash_spi_erase_total_size = BOOT_LOADER_TOTAL_UPDATE_SIZE;
                                    }
                                    else
                                    {
                                        flash_spi_erase_total_size += BOOT_LOADER_SERIAL_FLASH_BLOCK_SIZE;
                                    }
                                    if (flash_spi_erase_total_size != BOOT_LOADER_TOTAL_UPDATE_SIZE)
                                    {
                                        /* serial flash erase is not finished */
                                        /* Set start address of serial flash */
                                        flash_spi_erase_info.addr = BOOT_LOADER_SERIAL_FLASH_START_ADDRESS +
                                                                        flash_spi_erase_total_size;
                                        /* Set serial flash erase mode */
                                        flash_spi_erase_info.mode = FLASH_SPI_MODE_B64K_ERASE;
                                        flash_spi_error_code = fwup_flash_spi_erase(&flash_spi_erase_info);
                                        if(FLASH_SPI_SUCCESS != flash_spi_error_code)
                                        {
                                            DEBUG_LOG2("R_FLASH_SPI_Erase() returns error. %d.\r\n",
                                                    flash_spi_error_code);
                                            fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                            secure_boot_error_code = FWUP_FAIL;
                                            break; // @suppress("Case break statement")
                                        }
                                        fwup_update_status(FWUP_STATE_BANK0_INSTALL_CODE_FLASH_ERASE_WAIT);
                                    }
                                    else
                                    {
                                        fwup_update_status(FWUP_STATE_BANK0_INSTALL_CODE_FLASH_ERASE_COMPLETE);
                                    }
                                }
                                else
                                {
                                    DEBUG_LOG("NG\r\n");
                                    DEBUG_LOG2("R_FLASH_SPI_Polling() returns error code = %d.\r\n",
                                            flash_spi_error_code);
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break; // @suppress("Case break statement")
                                }
#else /* FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1 */
                                /* this state will be update by flash callback */
#endif /* FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1 */
                                break;

                            case FWUP_STATE_BANK0_INSTALL_CODE_FLASH_ERASE_COMPLETE:
                                if (FLASH_SUCCESS == s_flash_error_code)
                                {
                                    DEBUG_LOG("OK\r\n");
                                }
                                else
                                {
                                    DEBUG_LOG2("R_FLASH_Write() callback error. %d.\r\n", s_flash_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break; // @suppress("Case break statement")
                                }
                                s_sci_buffer_control[BOOT_LOADER_SCI_CONTROL_BLOCK_A].buffer_full_flag =
                                        BOOT_LOADER_SCI_RECEIVE_BUFFER_EMPTY;
                                s_sci_buffer_control[BOOT_LOADER_SCI_CONTROL_BLOCK_B].buffer_full_flag =
                                        BOOT_LOADER_SCI_RECEIVE_BUFFER_EMPTY;
                                s_sci_receive_control_block.p_sci_buffer_control =
                                        &s_sci_buffer_control[BOOT_LOADER_SCI_CONTROL_BLOCK_A];
                                s_sci_receive_control_block.current_state = BOOT_LOADER_SCI_CONTROL_BLOCK_A;
                                DEBUG_LOG2("send \"%s\" via UART.\r\n", INITIAL_FIRMWARE_FILE_NAME);
                                fwup_update_status(FWUP_STATE_BANK0_INSTALL_CODE_FLASH_READ_WAIT);
                                break;

                            case FWUP_STATE_BANK0_INSTALL_CODE_FLASH_READ_WAIT:

                                /* install code flash area */
                                if ( !firm_block_read(s_load_fw_control_block.flash_buffer,
                                        s_load_fw_control_block.offset))
                                {
#if (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1)
                                    flash_spi_error_code = fwup_flash_spi_write(
                                        (uint8_t)&s_load_fw_control_block.flash_buffer,
                                        (uint32_t)BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS +
                                            s_load_fw_control_block.offset,
                                            sizeof(s_load_fw_control_block.flash_buffer));
                                    if(FLASH_SPI_SUCCESS != flash_spi_error_code)
                                    {
                                        DEBUG_LOG("NG\r\n");
                                        DEBUG_LOG2("R_FLASH_SPI_Erase() returns error code = %d.\r\n",
                                                flash_spi_error_code);
                                        fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                        secure_boot_error_code = FWUP_FAIL;
                                        break; // @suppress("Case break statement")
                                    }
                                    fwup_update_status(FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_ERASE_WAIT);
#else /* (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 0) */
                                    flash_api_error_code = fwup_flash_write(
                                            (uint32_t) s_load_fw_control_block.flash_buffer,
                                            (uint32_t) BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS
                                                    + s_load_fw_control_block.offset,
                                            sizeof(s_load_fw_control_block.flash_buffer));
                                    if (FLASH_SUCCESS != flash_api_error_code)
                                    {
                                        DEBUG_LOG2("R_FLASH_Write() returns error. %d.\r\n", s_flash_error_code);
                                        DEBUG_LOG("system error.\r\n");
                                        fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                        secure_boot_error_code = FWUP_FAIL;
                                        break; // @suppress("Case break statement")
                                    }
#if (FLASH_CFG_CODE_FLASH_BGO == 1)
                                    fwup_update_status(FWUP_STATE_BANK0_INSTALL_CODE_FLASH_WRITE_WAIT);
#elif (FLASH_CFG_CODE_FLASH_BGO == 0)
                                    fwup_update_status(FWUP_STATE_BANK0_INSTALL_CODE_FLASH_WRITE_COMPLETE);
#endif  /* FLASH_CFG_CODE_FLASH_BGO */
#endif /* FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1 */
                                }
                                break;

                            case FWUP_STATE_BANK0_INSTALL_CODE_FLASH_WRITE_WAIT:
#if (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1)
                                flash_spi_error_code = fwup_flash_spi_polling_write();
                                if(FLASH_SPI_SUCCESS_BUSY == flash_spi_error_code)
                                {
                                    fwup_update_status(FWUP_STATE_BANK0_INSTALL_CODE_FLASH_WRITE_WAIT);
                                }
                                else if (FLASH_SPI_SUCCESS == flash_spi_error_code)
                                {
                                    fwup_update_status(FWUP_STATE_BANK0_INSTALL_CODE_FLASH_WRITE_COMPLETE);
                                }
                                else
                                {
                                    DEBUG_LOG("NG\r\n");
                                    DEBUG_LOG2("R_FLASH_SPI_Polling() returns error. %d.\r\n", flash_spi_error_code);
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break; // @suppress("Case break statement")
                                }
#else /* (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 0) */
                                /* this state will be update by flash callback */
#endif /* FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER */
                                break;

                            case FWUP_STATE_BANK0_INSTALL_CODE_FLASH_WRITE_COMPLETE:
                                if (FLASH_SUCCESS != s_flash_error_code)
                                {
                                    DEBUG_LOG2("R_FLASH_Write() callback error. %d.\r\n", s_flash_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break;
                                }

                                s_load_fw_control_block.offset += FWUP_WRITE_BLOCK_SIZE;
                                s_load_fw_control_block.progress =
                                        (uint32_t) (((float) ((s_load_fw_control_block.offset)
                                                / (float) ((BOOT_LOADER_FLASH_CF_BLOCK_SIZE
                                                        * BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER))) * 100));
                                DEBUG_LOG2("installing firmware...%u%%(%d/%dKB).\r",
                                        s_load_fw_control_block.progress,
                                        s_load_fw_control_block.offset / 1024,
                                        (BOOT_LOADER_FLASH_CF_BLOCK_SIZE *
                                                BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER / 1024));
                                if (s_load_fw_control_block.offset
                                        < (BOOT_LOADER_FLASH_CF_BLOCK_SIZE * BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER))
                                {

                                    /* one more loop */
                                    fwup_update_status(FWUP_STATE_BANK0_INSTALL_CODE_FLASH_READ_WAIT);
                                }
                                else if ((BOOT_LOADER_FLASH_CF_BLOCK_SIZE * BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER) ==
                                    s_load_fw_control_block.offset)
                                {
                                    DEBUG_LOG("\n");
                                    DEBUG_LOG("completed installing firmware.\r\n");
                                    s_load_const_data_control_block.offset = 0;
                                    fwup_update_status(FWUP_STATE_INSTALL_DATA_FLASH_READ_WAIT);
                                }
                                else
                                {
                                    DEBUG_LOG("\n");
                                    DEBUG_LOG("fatal error occurred.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                }
                                break;

                            case FWUP_STATE_INSTALL_DATA_FLASH_READ_WAIT:

                                /* install data flash area */
                                if ( !const_data_block_read(s_load_const_data_control_block.flash_buffer,
                                        s_load_const_data_control_block.offset))
                                {
                                    fwup_update_status(FWUP_STATE_INSTALL_DATA_FLASH_READ_COMPLETE);
                                }
                                break;

                            case FWUP_STATE_INSTALL_DATA_FLASH_READ_COMPLETE:
                                flash_api_error_code =
                                    fwup_flash_write((uint32_t)&s_load_const_data_control_block.flash_buffer
                                                                [(s_load_const_data_control_block.offset % 1024) / 4],
                                                    (uint32_t)BOOT_LOADER_USER_CONST_DATA_LOW_ADDRESS +
                                                    s_load_const_data_control_block.offset, FWUP_WRITE_BLOCK_SIZE);
                                if (FLASH_SUCCESS != flash_api_error_code)
                                {
                                    DEBUG_LOG2("R_FLASH_Write() returns error. %d.\r\n", s_flash_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break;
                                }
#if (FLASH_CFG_DATA_FLASH_BGO == 1)
                                fwup_update_status(FWUP_STATE_INSTALL_DATA_FLASH_WRITE_WAIT);
#else /* (FLASH_CFG_DATA_FLASH_BGO == 0) */
                                fwup_update_status(FWUP_STATE_INSTALL_DATA_FLASH_WRITE_COMPLETE);
#endif  /* (FLASH_CFG_DATA_FLASH_BGO == 1) */
                                break;

                            case FWUP_STATE_INSTALL_DATA_FLASH_WRITE_WAIT:

                                /* this state will be update by flash callback */
                                break;

                            case FWUP_STATE_INSTALL_DATA_FLASH_WRITE_COMPLETE:
                                if (FLASH_SUCCESS != s_flash_error_code)
                                {
                                    DEBUG_LOG2("R_FLASH_Write() callback error. %d.\r\n", s_flash_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break; // @suppress("Case break statement")
                                }

                                s_load_const_data_control_block.offset += FWUP_WRITE_BLOCK_SIZE;
                                s_load_const_data_control_block.progress =
                                        (uint32_t) (((float) ((s_load_const_data_control_block.offset)
                                                / (float) (BSP_DATA_FLASH_SIZE_BYTES)) * 100));
                                static uint32_t s_previous_offset = 0;
                                if ((s_load_const_data_control_block.offset / 1024) != s_previous_offset)
                                {
                                    DEBUG_LOG2("installing const data...%u%%(%d/%dKB).\r",
                                            s_load_const_data_control_block.progress,
                                            s_load_const_data_control_block.offset / 1024,
                                            BSP_DATA_FLASH_SIZE_BYTES/1024);
                                    s_previous_offset = s_load_const_data_control_block.offset / 1024;

                                    if (BSP_DATA_FLASH_SIZE_BYTES ==
                                        s_load_const_data_control_block.offset)
                                    {
                                        DEBUG_LOG("\n");
                                        DEBUG_LOG("completed installing const data.\r\n");

                                        DEBUG_LOG2("integrity check scheme = %-.32s\r\n",
                                                sp_fwup_control_block_bank1->signature_type);
                                        DEBUG_LOG("bank1(temporary area) on code flash integrity check...");

                                        /* Firmware verification for the signature type. */
#if (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1)
                                        //*************************************************************
                                        // CAUTION : Unfinished implementation.
                                        //   Signing verification of serial flash data by ECDSA.
                                        //*************************************************************
                                        verification_result = -1;
#else /* (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 0) */
                                        if (!strcmp((const char *)sp_fwup_control_block_bank1->signature_type,
                                                INTEGRITY_CHECK_SCHEME_HASH_SHA256_STANDALONE))
                                        {
                                            uint8_t hash_sha256[TC_SHA256_DIGEST_SIZE];

                                            /* Hash message */
                                            struct tc_sha256_state_struct x_ctx;
                                            tc_sha256_init( &x_ctx);
                                            tc_sha256_update( &x_ctx,
                                                    (uint8_t*) BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS
                                                            + BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
                                                    (BOOT_LOADER_FLASH_CF_BLOCK_SIZE
                                                            * BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER)
                                                            - BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH);
                                            tc_sha256_final(hash_sha256, &x_ctx);
                                            verification_result = memcmp(sp_fwup_control_block_bank1->signature,
                                                    hash_sha256, sizeof(hash_sha256));
                                        }
                                        else if (!strcmp((const char*)sp_fwup_control_block_bank1->signature_type,
                                                INTEGRITY_CHECK_SCHEME_SIG_SHA256_ECDSA_STANDALONE))
                                        {
                                            verification_result = fwup_verification_sha256_ecdsa(
                                                    (const uint8_t*) BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS
                                                            + BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
                                                    (BOOT_LOADER_FLASH_CF_BLOCK_SIZE
                                                            * BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER)
                                                            - BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
                                                    sp_fwup_control_block_bank1->signature,
                                                    sp_fwup_control_block_bank1->signature_size);
                                        }
                                        else
                                        {
                                            verification_result = -1;
                                        }
#endif /* F(FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1) */

                                        if (0 == verification_result)
                                        {
                                            DEBUG_LOG("OK\r\n");
                                            s_load_const_data_control_block.offset = 0;
                                            DEBUG_LOG("software reset...\r\n");
                                            R_BSP_SoftwareDelay(3000, BSP_DELAY_MILLISECS);
                                            software_reset();
                                        }
                                        else
                                        {
                                            DEBUG_LOG("NG\r\n");
                                            fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                            secure_boot_error_code = FWUP_FAIL;
                                        }

                                    }
                                    else
                                    {
                                        fwup_update_status(FWUP_STATE_INSTALL_DATA_FLASH_READ_WAIT);
                                    }
                                }
                                else if (s_load_const_data_control_block.offset < (BSP_DATA_FLASH_SIZE_BYTES))
                                {

                                    /* one more loop */
                                    fwup_update_status(FWUP_STATE_INSTALL_DATA_FLASH_READ_COMPLETE);
                                }
                                else
                                {
                                    DEBUG_LOG("\n");
                                    DEBUG_LOG("fatal error occurred.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                }
                                break;
                            default:
                                break;
                        }
                    break;
                    case LIFECYCLE_STATE_TESTING:
                        DEBUG_LOG("illegal status\r\n");
                        DEBUG_LOG("swap bank...");
                        R_BSP_SoftwareDelay(3000, BSP_DELAY_MILLISECS);
                        bank_swap_with_software_reset();
                        while (1)
                        {
                            ;
                        }
                        break;

                    case LIFECYCLE_STATE_INSTALLING:
                        switch (fwup_get_status())
                        {
                            case FWUP_STATE_BANK0_CHECK:
                                DEBUG_LOG2("integrity check scheme = %-.32s\r\n",
                                        sp_fwup_control_block_bank0->signature_type);
                                DEBUG_LOG("bank0(execute area) on code flash integrity check...");

                                /* Firmware verification for the signature type. */
                                if (!strcmp((const char *)sp_fwup_control_block_bank0->signature_type,
                                        INTEGRITY_CHECK_SCHEME_HASH_SHA256_STANDALONE))
                                {

                                    /* Hash message */
                                    uint8_t hash_sha256[TC_SHA256_DIGEST_SIZE];
                                    struct tc_sha256_state_struct x_ctx;
                                    tc_sha256_init( &x_ctx);
                                    tc_sha256_update( &x_ctx,
                                            (uint8_t*) BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS
                                                    + BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
                                            (BOOT_LOADER_FLASH_CF_BLOCK_SIZE * BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER)
                                                    - BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH);
                                    tc_sha256_final(hash_sha256, &x_ctx);
                                    verification_result = memcmp(sp_fwup_control_block_bank0->signature,
                                            hash_sha256, sizeof(hash_sha256));
                                }
                                else if (!strcmp((const char *)sp_fwup_control_block_bank0->signature_type,
                                        INTEGRITY_CHECK_SCHEME_SIG_SHA256_ECDSA_STANDALONE))
                                {
                                    verification_result = fwup_verification_sha256_ecdsa(
                                            (const uint8_t*) BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS
                                                    + BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
                                            (BOOT_LOADER_FLASH_CF_BLOCK_SIZE * BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER)
                                                    - BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
                                            sp_fwup_control_block_bank0->signature,
                                            sp_fwup_control_block_bank0->signature_size);
                                }
                                else
                                {
                                    verification_result = -1;
                                }

                                if (0 == verification_result)
                                {
                                    DEBUG_LOG("OK\r\n");

                                    /* CODE CHECKER, this is OK as a comment precedes the cast. */
                                    if (!strncmp((const char *)sp_boot_loader_magic_code,
                                            BOOT_LOADER_MAGIC_CODE, BOOT_LOADER_MAGIC_CODE_LENGTH))
                                    {
                                        fwup_update_status(FWUP_STATE_BANK0_INSTALL_SET_BOOT_PROTECT);
                                    }
                                    else
                                    {
#if (FWUP_CFG_BOOT_PROTECT_ENABLE == 1)
                                        if (false == fwup_get_boot_protect())
                                        {
                                            /* No Boot Protected */
#if (FWUP_FLASH_BANK_MODE == 0) // Dual mode
                                            DEBUG_LOG("erase bank1 secure boot mirror area...");
                                            flash_api_error_code =
                                                fwup_flash_erase(BOOT_LOADER_MIRROR_ERASE_ADDRESS,
                                                            BOOT_LOADER_MIRROR_BLOCK_NUMBER);
                                            if(FLASH_SUCCESS != flash_api_error_code)
                                            {
                                                DEBUG_LOG("NG\r\n");
                                                DEBUG_LOG2("R_FLASH_Erase() returns error code = %d.\r\n",
                                                    s_flash_error_code);
                                                fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                                secure_boot_error_code = FWUP_FAIL;
                                                break; // @suppress("Case break statement")
                                            }
                                            fwup_update_status(FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_ERASE_WAIT);
#else /* Linear mode */
                                            /* Do not FLASH erase because the Bootloader is not copied */
                                            /* Skip to next status */
                                            fwup_update_status(FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_ERASE_COMPLETE);
#endif  /* FWUP_FLASH_BANK_MODE */
                                        }
                                        else
                                        {
                                            /* Already Boot Protected */
                                            /* Skip boot copy process */
                                            DEBUG_LOG("already boot protected.\r\n");
                                            s_flash_error_code = FLASH_SUCCESS;
                                            fwup_update_status(FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_WRITE_COMPLETE);
                                        }
#else /* FWUP_CFG_BOOT_PROTECT_ENABLE == 1 */
#if (FWUP_FLASH_BANK_MODE == 0) /* Dual mode */
                                        DEBUG_LOG("erase bank1 secure boot mirror area...");
                                        flash_api_error_code
                                            = fwup_flash_erase((flash_block_address_t)BOOT_LOADER_MIRROR_ERASE_ADDRESS,
                                                    (uint32_t)BOOT_LOADER_MIRROR_BLOCK_NUMBER);
                                        if (FLASH_SUCCESS != flash_api_error_code)
                                        {
                                            DEBUG_LOG("NG\r\n");
                                            DEBUG_LOG2("R_FLASH_Erase() returns error code = %d.\r\n",
                                                    s_flash_error_code);
                                            fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                            secure_boot_error_code = FWUP_FAIL;
                                            break; // @suppress("Case break statement")
                                        }
                                        fwup_update_status(FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_ERASE_WAIT);
#else /* Linear mode */
                                        /* Do not FLASH erase because the Bootloader is not copied */
                                        /* Skip to next status */
                                        fwup_update_status(FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_ERASE_COMPLETE);
#endif  /* FWUP_FLASH_BANK_MODE */
#endif  /* (FWUP_CFG_BOOT_PROTECT_ENABLE == 1) */
                                    }
                                }
                                else
                                {
                                    DEBUG_LOG("NG.\r\n");
                                    DEBUG_LOG("Code flash is completely broken.\r\n");
                                    DEBUG_LOG("Please erase all code flash.\r\n");
                                    DEBUG_LOG("And, write secure boot using debugger.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                }
                                break;

                            case FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_ERASE_WAIT:

                                /* this state will be update by flash callback */
                                break;

                            case FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_ERASE_COMPLETE:
                                if (FLASH_SUCCESS == s_flash_error_code)
                                {
#if (FWUP_FLASH_BANK_MODE == 0) /* Dual mode */
                                    DEBUG_LOG("OK\r\n");
#endif /* (FWUP_FLASH_BANK_MODE) */
                                }
                                else
                                {
                                    DEBUG_LOG2("R_FLASH_Erase() callback error. %d.\r\n", s_flash_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break; // @suppress("Case break statement")
                                }
#if (FWUP_FLASH_BANK_MODE == 0) /* Dual mode */
                                DEBUG_LOG("copy secure boot from bank0 to bank1...");
                                flash_api_error_code = fwup_flash_write((uint32_t)BOOT_LOADER_LOW_ADDRESS,
                                    (uint32_t)BOOT_LOADER_MIRROR_LOW_ADDRESS,
                                    BOOT_LOADER_PGM_SIZE);
                                if (FLASH_SUCCESS != flash_api_error_code)
                                {
                                    DEBUG_LOG("NG\r\n");
                                    DEBUG_LOG2("R_FLASH_Write() returns error code = %d.\r\n", s_flash_error_code);
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break; // @suppress("Case break statement")
                                }
                                fwup_update_status(FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_WRITE_WAIT);
#else /* Linear mode */
                                /* Do not FLASH write because the Bootloader is not copied */
                                /* Skip to next status */
                                fwup_update_status(FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_WRITE_COMPLETE);
#endif /* (FWUP_FLASH_BANK_MODE) */
                                break;

                            case FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_WRITE_WAIT:

                                /* this state will be update by flash callback */
                                break;

                            case FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_WRITE_COMPLETE:
                                if (FLASH_SUCCESS == s_flash_error_code)
                                {
#if (FWUP_FLASH_BANK_MODE == 0) /* Dual mode */
                                    DEBUG_LOG("OK\r\n");
#endif /* (FWUP_FLASH_BANK_MODE) */
                                    fwup_update_status(FWUP_STATE_BANK0_INSTALL_SET_BOOT_PROTECT);
                                }
                                else
                                {
                                    DEBUG_LOG2("R_FLASH_Write() callback error. %d.\r\n", s_flash_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                }
                                break;

                            case FWUP_STATE_BANK0_INSTALL_SET_BOOT_PROTECT:
#if (FWUP_CFG_BOOT_PROTECT_ENABLE == 1)
                                if (false == fwup_get_boot_protect())
                                {
                                    flash_api_error_code = fwup_set_boot_protect();
                                    if (FLASH_SUCCESS != flash_api_error_code)
                                    {
                                        DEBUG_LOG2("R_FLASH_Control() returns error. %d.\r\n", flash_api_error_code);
                                        DEBUG_LOG("system error.\r\n");
                                        fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                        secure_boot_error_code = FWUP_FAIL;
                                        break; // @suppress("Case break statement")
                                    }
                                    else
                                    {
                                        DEBUG_LOG("set boot protect.\r\n");
                                    }
                                }
#endif
                                fwup_update_status(FWUP_STATE_FINALIZE);
                                break;

                            case FWUP_STATE_FINALIZE:
                                DEBUG_LOG("jump to user program\r\n");
                                R_BSP_SoftwareDelay(1000, BSP_DELAY_MILLISECS);
                                secure_boot_error_code = FWUP_SUCCESS;
                                break;

                            default:
                                break;
                        }
                        break;
                    case LIFECYCLE_STATE_VALID:
                        switch (fwup_get_status())
                        {
                            case FWUP_STATE_BANK0_UPDATE_CHECK:
                                DEBUG_LOG2("integrity check scheme = %-.32s\r\n",
                                        sp_fwup_control_block_bank0->signature_type);
                                DEBUG_LOG("bank0(execute area) on code flash integrity check...");

                                /* Firmware verification for the signature type. */
                                if (!strcmp((const char*)sp_fwup_control_block_bank0->signature_type,
                                        INTEGRITY_CHECK_SCHEME_HASH_SHA256_STANDALONE))
                                {

                                    /* Hash message */
                                    uint8_t hash_sha256[TC_SHA256_DIGEST_SIZE];
                                    struct tc_sha256_state_struct x_ctx;
                                    tc_sha256_init( &x_ctx);
                                    tc_sha256_update( &x_ctx,
                                            (uint8_t*) BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS
                                                    + BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
                                            (BOOT_LOADER_FLASH_CF_BLOCK_SIZE * BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER)
                                                    - BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH);
                                    tc_sha256_final(hash_sha256, &x_ctx);
                                    verification_result = memcmp(sp_fwup_control_block_bank0->signature,
                                            hash_sha256, sizeof(hash_sha256));
                                }
                                else if (!strcmp((const char*)sp_fwup_control_block_bank0->signature_type,
                                        INTEGRITY_CHECK_SCHEME_SIG_SHA256_ECDSA_STANDALONE))
                                {
                                    verification_result = fwup_verification_sha256_ecdsa(
                                            (const uint8_t*) BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS
                                                    + BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
                                            (BOOT_LOADER_FLASH_CF_BLOCK_SIZE * BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER)
                                                    - BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
                                            sp_fwup_control_block_bank0->signature,
                                            sp_fwup_control_block_bank0->signature_size);
                                }
                                else
                                {
                                    verification_result = -1;
                                }

                                if (0 == verification_result)
                                {
                                    DEBUG_LOG("OK\r\n");
                                    if (LIFECYCLE_STATE_BLANK != sp_fwup_control_block_bank1->image_flag)
                                    {
                                        DEBUG_LOG("erase install area (code flash): ");
#if (FLASH_CFG_CODE_FLASH_BGO == 1)
                                        flash_api_error_code
                                            = fwup_flash_erase(
                                                (flash_block_address_t)BOOT_LOADER_UPDATE_TEMPORARY_AREA_ERASE_ADDRESS,
                                                BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER);
                                        if (FLASH_SUCCESS != flash_api_error_code)
                                        {
                                            DEBUG_LOG2("R_FLASH_Erase() returns error. %d.\r\n", s_flash_error_code);
                                            DEBUG_LOG("system error.\r\n");
                                            fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                            secure_boot_error_code = FWUP_FAIL;
                                            break;
                                        }
                                        fwup_update_status(FWUP_STATE_BANK1_UPDATE_CODE_FLASH_ERASE_WAIT);
#else /* (FLASH_CFG_CODE_FLASH_BGO == 0) */
#if (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1)
                                        flash_spi_erase_total_size = 0x0;
                                        /* Set start address of serial flash */
                                        flash_spi_erase_info.addr = BOOT_LOADER_SERIAL_FLASH_START_ADDRESS;
                                        /* Set serial flash erase mode */
                                        flash_spi_erase_info.mode = FLASH_SPI_MODE_B64K_ERASE;
                                        flash_spi_error_code = fwup_flash_spi_erase(&flash_spi_erase_info);
                                        if(FLASH_SPI_SUCCESS != flash_spi_error_code)
                                        {
                                            DEBUG_LOG2("R_FLASH_SPI_Erase() returns error. %d.\r\n",
                                                    flash_spi_error_code);
                                            DEBUG_LOG("system error.\r\n");
                                            fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                            secure_boot_error_code = FWUP_FAIL;
                                            break; // @suppress("Case break statement")
                                        }
                                        fwup_update_status(FWUP_STATE_BANK1_UPDATE_CODE_FLASH_ERASE_WAIT);
#else /* (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 0) */
                                        flash_api_error_code = fwup_flash_erase(
                                                (flash_block_address_t) BOOT_LOADER_UPDATE_TEMPORARY_AREA_ERASE_ADDRESS,
                                                BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER);
                                        if (FLASH_SUCCESS != flash_api_error_code)
                                        {
                                            DEBUG_LOG2("R_FLASH_Erase() returns error. %d.\r\n", s_flash_error_code);
                                            DEBUG_LOG("system error.\r\n");
                                            fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                            secure_boot_error_code = FWUP_FAIL;
                                            break; // @suppress("Case break statement")
                                        }
                                        fwup_update_status(FWUP_STATE_BANK1_UPDATE_CODE_FLASH_ERASE_COMPLETE);
#endif /* FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER */
#endif  /* FLASH_CFG_CODE_FLASH_BGO == 1 */
                                    }
                                    else
                                    {
                                        fwup_update_status(FWUP_STATE_FINALIZE);
                                    }
                                }
                                else
                                {
                                    DEBUG_LOG("NG.\r\n");
                                    DEBUG_LOG("Code flash is completely broken.\r\n");
                                    DEBUG_LOG("Please erase all code flash.\r\n");
                                    DEBUG_LOG("And, write secure boot using debugger.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                }
                                break;

                            case FWUP_STATE_BANK1_UPDATE_CODE_FLASH_ERASE_WAIT:
#if (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1)
                                flash_spi_error_code = fwup_flash_spi_polling_erase();
                                if(FLASH_SPI_SUCCESS_BUSY == flash_spi_error_code)
                                {
                                    /* Check serial flash erase polling continuously */
                                    fwup_update_status(FWUP_STATE_BANK0_INSTALL_CODE_FLASH_ERASE_WAIT);
                                }
                                else if (FLASH_SPI_SUCCESS == flash_spi_error_code)
                                {
                                    /* Serial flash erase finished */
                                    if ((BOOT_LOADER_TOTAL_UPDATE_SIZE - flash_spi_erase_total_size) <
                                            BOOT_LOADER_SERIAL_FLASH_BLOCK_SIZE)
                                    {
                                        /* last erase block of serial flash */
                                        flash_spi_erase_total_size = BOOT_LOADER_TOTAL_UPDATE_SIZE;
                                    }
                                    else
                                    {
                                        flash_spi_erase_total_size += BOOT_LOADER_SERIAL_FLASH_BLOCK_SIZE;
                                    }
                                    if (flash_spi_erase_total_size != BOOT_LOADER_TOTAL_UPDATE_SIZE)
                                    {
                                        /* serial flash erase is not finished */
                                        /* Set start address of serial flash */
                                        flash_spi_erase_info.addr =
                                                BOOT_LOADER_SERIAL_FLASH_START_ADDRESS + flash_spi_erase_total_size;
                                        /* Set serial flash erase mode */
                                        flash_spi_erase_info.mode = FLASH_SPI_MODE_B64K_ERASE;
                                        flash_spi_error_code = fwup_flash_spi_erase(&flash_spi_erase_info);
                                        if(FLASH_SPI_SUCCESS != flash_spi_error_code)
                                        {
                                            DEBUG_LOG2("R_FLASH_SPI_Erase() returns error. %d.\r\n",
                                                    flash_spi_error_code);
                                            fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                            secure_boot_error_code = FWUP_FAIL;
                                            break; // @suppress("Case break statement")
                                        }
                                        fwup_update_status(FWUP_STATE_BANK1_UPDATE_CODE_FLASH_ERASE_WAIT);
                                    }
                                    else
                                    {
                                        fwup_update_status(FWUP_STATE_BANK1_UPDATE_CODE_FLASH_ERASE_COMPLETE);
                                    }
                                }
                                else
                                {
                                    DEBUG_LOG("NG\r\n");
                                    DEBUG_LOG2("R_FLASH_SPI_Polling() returns error code = %d.\r\n",
                                            flash_spi_error_code);
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break; // @suppress("Case break statement")
                                }
#else /* F(FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1) */
                                /* this state will be update by flash callback */
#endif /* F(FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1) */
                                break;

                            case FWUP_STATE_BANK1_UPDATE_CODE_FLASH_ERASE_COMPLETE:
                                if (FLASH_SUCCESS == s_flash_error_code)
                                {
                                    DEBUG_LOG("OK\r\n");
                                    fwup_update_status(FWUP_STATE_FINALIZE);
                                }
                                else
                                {
                                    DEBUG_LOG2("R_FLASH_Erase() callback error. %d.\r\n", s_flash_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                }

                                break;

                            case FWUP_STATE_FINALIZE:
                                DEBUG_LOG("jump to user program\r\n");
                                R_BSP_SoftwareDelay(1000, BSP_DELAY_MILLISECS);
                                secure_boot_error_code = FWUP_SUCCESS;
                                break;

                            default:
                                break;
                        }
                        break;
                    case LIFECYCLE_STATE_EOL:
                        switch (fwup_get_status())
                        {
                            case FWUP_STATE_BANK0_CHECK:
                                if (LIFECYCLE_STATE_EOL == sp_fwup_control_block_bank1->image_flag)
                                {

                                    /* The status of bank1 is already EOL */
                                    DEBUG_LOG("check bank1 status: ");
                                    s_flash_error_code = FLASH_SUCCESS;
                                    fwup_update_status(FWUP_STATE_EOL_DATA_FLASH_ERASE_COMPLETE);
                                }
                                else
                                {

                                    /* The status of bank1 is NOT EOL */
                                    DEBUG_LOG("erase install area (code flash): ");
#if (FLASH_CFG_CODE_FLASH_BGO == 1)
                                    flash_api_error_code
                                        = fwup_flash_erase(
                                            (flash_block_address_t)BOOT_LOADER_UPDATE_TEMPORARY_AREA_ERASE_ADDRESS,
                                            BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER);
                                    if (FLASH_SUCCESS != flash_api_error_code)
                                    {
                                        DEBUG_LOG("NG.\r\n");
                                        DEBUG_LOG2("R_FLASH_Erase() returns error. %d.\r\n", s_flash_error_code);
                                        DEBUG_LOG("system error.\r\n");
                                        fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                        secure_boot_error_code = FWUP_FAIL;
                                        break;
                                    }
                                    fwup_update_status(FWUP_STATE_EOL_BANK1_ERASE_WAIT);
#else
#if (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1)
                                    flash_spi_erase_total_size = 0x0;
                                    /* Set start address of serial flash */
                                    flash_spi_erase_info.addr = BOOT_LOADER_SERIAL_FLASH_START_ADDRESS;
                                    /* Set serial flash erase mode */
                                    flash_spi_erase_info.mode = FLASH_SPI_MODE_B64K_ERASE;
                                    flash_spi_error_code = fwup_flash_spi_erase(&flash_spi_erase_info);
                                    if(FLASH_SPI_SUCCESS != flash_spi_error_code)
                                    {
                                        DEBUG_LOG2("R_FLASH_SPI_Erase() returns error. %d.\r\n", flash_spi_error_code);
                                        DEBUG_LOG("system error.\r\n");
                                        fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                        secure_boot_error_code = FWUP_FAIL;
                                        break;
                                    }
                                    fwup_update_status(FWUP_STATE_EOL_BANK1_ERASE_WAIT);
#else /* (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 0) */
                                    flash_api_error_code = fwup_flash_erase(
                                            (flash_block_address_t) BOOT_LOADER_UPDATE_TEMPORARY_AREA_ERASE_ADDRESS,
                                            BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER);
                                    if (FLASH_SUCCESS != flash_api_error_code)
                                    {
                                        DEBUG_LOG("NG.\r\n");
                                        DEBUG_LOG2("R_FLASH_Erase() returns error. %d.\r\n", s_flash_error_code);
                                        DEBUG_LOG("system error.\r\n");
                                        fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                        secure_boot_error_code = FWUP_FAIL;
                                        break;
                                    }
                                    fwup_update_status(FWUP_STATE_EOL_BANK1_ERASE_COMPLETE);
#endif /* FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER */
#endif  /* (FLASH_CFG_CODE_FLASH_BGO == 1) */
                                }
                            break;

                            case FWUP_STATE_EOL_BANK1_ERASE_WAIT:
#if (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1)
                                flash_spi_error_code = fwup_flash_spi_polling_erase();
                                if(FLASH_SPI_SUCCESS_BUSY == flash_spi_error_code)
                                {
                                    /* Check serail flash erase polling continuously */
                                    fwup_update_status(FWUP_STATE_EOL_BANK1_ERASE_WAIT);
                                }
                                else if (FLASH_SPI_SUCCESS == flash_spi_error_code)
                                {
                                    /* Serial flash erase finished */
                                    if ((BOOT_LOADER_TOTAL_UPDATE_SIZE - flash_spi_erase_total_size) <
                                            BOOT_LOADER_SERIAL_FLASH_BLOCK_SIZE)
                                    {
                                        /* last erase block of serial flash */
                                        flash_spi_erase_total_size = BOOT_LOADER_TOTAL_UPDATE_SIZE;
                                    }
                                    else
                                    {
                                        flash_spi_erase_total_size += BOOT_LOADER_SERIAL_FLASH_BLOCK_SIZE;
                                    }
                                    if (flash_spi_erase_total_size != BOOT_LOADER_TOTAL_UPDATE_SIZE)
                                    {
                                        /* serial flash erase is not finished */
                                        /* Set start address of serial flash */
                                        flash_spi_erase_info.addr = BOOT_LOADER_SERIAL_FLASH_START_ADDRESS +
                                            flash_spi_erase_total_size;
                                        /* Set serial flash erase mode */
                                        flash_spi_erase_info.mode = FLASH_SPI_MODE_B64K_ERASE;
                                        flash_spi_error_code = fwup_flash_spi_erase(&flash_spi_erase_info);
                                        if(FLASH_SPI_SUCCESS != flash_spi_error_code)
                                        {
                                            DEBUG_LOG2("R_FLASH_SPI_Erase() returns error. %d.\r\n",
                                                    flash_spi_error_code);
                                            DEBUG_LOG("system error.\r\n");
                                            fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                            secure_boot_error_code = FWUP_FAIL;
                                            break;
                                        }
                                        fwup_update_status(FWUP_STATE_EOL_BANK1_ERASE_WAIT);
                                    }
                                    else
                                    {
                                        fwup_update_status(FWUP_STATE_EOL_BANK1_ERASE_COMPLETE);
                                    }
                                }
                                else
                                {
                                    DEBUG_LOG("NG\r\n");
                                    DEBUG_LOG2("R_FLASH_SPI_Polling() returns error code. %d.\r\n",
                                            flash_spi_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break;
                                }
#else /* F(FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1) */
                                /* this state will be update by flash callback */
#endif /* F(FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1) */
                            break;

                            case FWUP_STATE_EOL_BANK1_ERASE_COMPLETE:
                                if (FLASH_SUCCESS == s_flash_error_code)
                                {
                                    DEBUG_LOG("OK\r\n");
                                }
                                else
                                {
                                    DEBUG_LOG("NG.\r\n");
                                    DEBUG_LOG2("R_FLASH_Erase() callback error. %d.\r\n", s_flash_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break;
                                }

                                /* CODE CHECKER, this is OK as a comment precedes the cast. */
                                memcpy(s_block_buffer, (void*)BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS,
                                        BOOT_LOADER_FLASH_CF_BLOCK_SIZE);

                                p_fwup_control_block_tmp->image_flag = LIFECYCLE_STATE_EOL;
                                DEBUG_LOG2("update LIFECYCLE_STATE from [%s] to [%s]\r\n",
                                        get_status_string(sp_fwup_control_block_bank1->image_flag),
                                        get_status_string(p_fwup_control_block_tmp->image_flag));
                                DEBUG_LOG("bank1(temporary area) block0 write (to update LIFECYCLE_STATE)...");
#if (FLASH_CFG_CODE_FLASH_BGO == 1)
                                flash_api_error_code
                                    = fwup_flash_write((uint32_t)p_fwup_control_block_tmp,
                                        (uint32_t)BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS,
                                        BOOT_LOADER_FLASH_CF_BLOCK_SIZE);
                                if (FLASH_SUCCESS != flash_api_error_code)
                                {
                                    DEBUG_LOG2("R_FLASH_Write() returns error. %d.\r\n", s_flash_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break;
                                }
                                fwup_update_status(FWUP_STATE_EOL_BANK1_LIFECYCLE_WRITE_WAIT);
#else /* (FLASH_CFG_CODE_FLASH_BGO == 0) */
#if (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1)
                                    flash_spi_error_code =
                                        fwup_flash_spi_write((uint8_t)&p_fwup_control_block_tmp,
                                            (uint32_t)BOOT_LOADER_SERIAL_FLASH_START_ADDRESS,
                                            BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH);
                                    if(FLASH_SPI_SUCCESS != flash_spi_error_code)
                                    {
                                        DEBUG_LOG("NG\r\n");
                                        DEBUG_LOG2("R_FLASH_SPI_Erase() returns error code = %d.\r\n",
                                            flash_spi_error_code);
                                        fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                        secure_boot_error_code = FWUP_FAIL;
                                        break;
                                    }
                                    fwup_update_status(FWUP_STATE_EOL_BANK1_LIFECYCLE_WRITE_WAIT);
#else /* (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 0) */
                                flash_api_error_code = fwup_flash_write((uint32_t) p_fwup_control_block_tmp,
                                        (uint32_t) BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS,
                                        BOOT_LOADER_FLASH_CF_BLOCK_SIZE);
                                if (FLASH_SUCCESS != flash_api_error_code)
                                {
                                    DEBUG_LOG2("R_FLASH_Write() returns error. %d.\r\n", s_flash_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break;
                                }
                                fwup_update_status(FWUP_STATE_EOL_BANK1_LIFECYCLE_WRITE_COMPLETE);
#endif /* FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER */
#endif  /* (FLASH_CFG_CODE_FLASH_BGO == 1) */
                            break;

                            case FWUP_STATE_EOL_BANK1_LIFECYCLE_WRITE_WAIT:
#if (FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1)
                                flash_spi_error_code = fwup_flash_spi_polling_write();
                                if(FLASH_SPI_SUCCESS_BUSY == flash_spi_error_code)
                                {
                                    fwup_update_status(FWUP_STATE_EOL_BANK1_LIFECYCLE_WRITE_WAIT);
                                }
                                else if (FLASH_SPI_SUCCESS == flash_spi_error_code)
                                {
                                    fwup_update_status(FWUP_STATE_EOL_BANK1_LIFECYCLE_WRITE_COMPLETE);
                                }
                                else
                                {
                                    DEBUG_LOG("NG\r\n");
                                    DEBUG_LOG2("R_FLASH_SPI_Polling() returns error code. %d.\r\n",
                                            flash_spi_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break;
                                }
#else
                                /* this state will be update by flash callback */
#endif /* F(FWUP_CFG_USE_SERIAL_FLASH_FOR_BUFFER == 1) */
                            break;

                            case FWUP_STATE_EOL_BANK1_LIFECYCLE_WRITE_COMPLETE:
                                if (FLASH_SUCCESS == s_flash_error_code)
                                {
                                    DEBUG_LOG("OK\r\n");
                                    DEBUG_LOG2("bank 0 status = 0x%x [%s]\r\n",
                                            sp_fwup_control_block_bank0->image_flag,
                                            get_status_string(sp_fwup_control_block_bank0->image_flag));
                                    DEBUG_LOG2("bank 1 status = 0x%x [%s]\r\n",
                                            sp_fwup_control_block_bank1->image_flag,
                                            get_status_string(sp_fwup_control_block_bank1->image_flag));
                                }
                                else
                                {
                                    DEBUG_LOG2("R_FLASH_Write() callback error. %d.\r\n", s_flash_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break;
                                }

                                /* Erase Data Flash */
                                DEBUG_LOG("erase install area (data flash): ");
                                flash_api_error_code = fwup_flash_erase(
                                        (flash_block_address_t) BOOT_LOADER_USER_CONST_DATA_LOW_ADDRESS,
                                        BOOT_LOADER_UPDATE_CONST_DATA_TARGET_BLOCK_NUMBER);
                                if (FLASH_SUCCESS != flash_api_error_code)
                                {
                                    DEBUG_LOG2("R_FLASH_Erase() returns error. %d.\r\n", s_flash_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break;
                                }
#if (FLASH_CFG_DATA_FLASH_BGO == 1)
                                fwup_update_status(FWUP_STATE_EOL_DATA_FLASH_ERASE_WAIT);
#else
                                fwup_update_status(FWUP_STATE_EOL_DATA_FLASH_ERASE_COMPLETE);
#endif  /* (FLASH_CFG_DATA_FLASH_BGO == 1) */
                            break;

                            case FWUP_STATE_EOL_DATA_FLASH_ERASE_WAIT:

                                /* this state will be update by flash callback */
                            break;

                            case FWUP_STATE_EOL_DATA_FLASH_ERASE_COMPLETE:
                                if (FLASH_SUCCESS == s_flash_error_code)
                                {
                                    DEBUG_LOG("OK\r\n");
                                }
                                else
                                {
                                    DEBUG_LOG2("R_FLASH_Erase() callback error. %d.\r\n", s_flash_error_code);
                                    DEBUG_LOG("system error.\r\n");
                                    fwup_update_status(FWUP_STATE_FATAL_ERROR);
                                    secure_boot_error_code = FWUP_FAIL;
                                    break;
                                }

                                /* End of the EOL process, infinity loop on Bootloader */
                                DEBUG_LOG("End Of Life process finished.\r\n");
                                R_BSP_SoftwareDelay(1000, BSP_DELAY_MILLISECS);
                                secure_boot_error_code = FWUP_END_OF_LIFE;
                            break;
                            default:
                                R_BSP_NOP();  /* Don't run this part.*/
                            break;
                        }
                    break;

                    default:
                        DEBUG_LOG2("illegal flash rom status code 0x%x.\r\n",
                                sp_fwup_control_block_bank0->image_flag);
                        DEBUG_LOG("Code flash is completely broken.\r\n");
                        DEBUG_LOG("Please erase all code flash.\r\n");
                        DEBUG_LOG("And, write secure boot using debugger.\r\n");
                        fwup_update_status(FWUP_STATE_FATAL_ERROR);
                        secure_boot_error_code = FWUP_FAIL;
                    break;
                }
                break;

            default:
            break;
        }
    }
    
    /* Close System-timer if boot process will end */
    if ((FWUP_SUCCESS == secure_boot_error_code) ||
        (FWUP_FAIL == secure_boot_error_code) ||
        (FWUP_END_OF_LIFE == secure_boot_error_code))
    {
        monitoring_error_code = fwup_state_monitoring_close();
        fwup_communication_close();
        if (MONI_SUCCESS != monitoring_error_code)
        {
            DEBUG_LOG2("R_SYS_TIME_Close() returns error. %d.\r\n", monitoring_error_code);
            DEBUG_LOG("system error.\r\n");
            fwup_update_status(FWUP_STATE_FATAL_ERROR);
            secure_boot_error_code = FWUP_FAIL;
        }
    }
    return secure_boot_error_code;
}
/**********************************************************************************************************************
 End of function R_FWUP_SecureBoot
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_FWUP_ExecuteFirmware
 * Description  :
 * Arguments    :
 * Return Value :
 **********************************************************************************************************************/
void R_FWUP_ExecuteFirmware(void)
{
#if (BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100)
    uint32_t addr;

    /* stop all interrupt completely */
    R_BSP_SET_PSW(0);
    addr = *(uint32_t*) USER_RESET_VECTOR_ADDRESS; /* CODE CHECKER, this is OK as a comment aligns with the cast */
    ((void (*) ()) addr)();
#else
    /* Fix me for other MCU family */
#endif /* BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100 */
}
/**********************************************************************************************************************
 End of function R_FWUP_ExecuteFirmware
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: software_reset
 * Description  :
 * Arguments    :
 * Return Value :
 **********************************************************************************************************************/
static void software_reset(void)
{

    /* stop all interrupt completely */
    R_BSP_SET_PSW(0);
    fwup_interrupts_disable();
    fwup_register_protect_disable();
    R_BSP_SoftwareReset();
    while (1)
    {
        R_BSP_NOP(); /* software reset */
    }
}
/**********************************************************************************************************************
 End of function software_reset
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: bank_swap_with_software_reset
 * Description  :
 * Arguments    :
 * Return Value :
 **********************************************************************************************************************/
static void bank_swap_with_software_reset(void)
{

    /* stop all interrupt completely */
    R_BSP_SET_PSW(0);
    fwup_interrupts_disable();
    fwup_flash_set_bank_toggle();
    fwup_register_protect_disable();
    R_BSP_SoftwareReset();
    while (1)
    {
        R_BSP_NOP(); /* software reset */
    }
}
/**********************************************************************************************************************
 End of function bank_swap_with_software_reset
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: firm_block_read
 * Description  :
 * Arguments    :
 * Return Value :
 **********************************************************************************************************************/
static int32_t firm_block_read(uint32_t *firmware, uint32_t offset)
{
    int32_t error_code = -1;
    if (BOOT_LOADER_SCI_RECEIVE_BUFFER_FULL == s_sci_buffer_control[BOOT_LOADER_SCI_CONTROL_BLOCK_A].buffer_full_flag)
    {
        memcpy(firmware, s_sci_buffer_control[BOOT_LOADER_SCI_CONTROL_BLOCK_A].buffer, FWUP_WRITE_BLOCK_SIZE);
        s_sci_buffer_control[BOOT_LOADER_SCI_CONTROL_BLOCK_A].buffer_full_flag = BOOT_LOADER_SCI_RECEIVE_BUFFER_EMPTY;
        error_code = 0;
    }
    else if (BOOT_LOADER_SCI_RECEIVE_BUFFER_FULL
            == s_sci_buffer_control[BOOT_LOADER_SCI_CONTROL_BLOCK_B].buffer_full_flag)
    {
        memcpy(firmware, s_sci_buffer_control[BOOT_LOADER_SCI_CONTROL_BLOCK_B].buffer, FWUP_WRITE_BLOCK_SIZE);
        s_sci_buffer_control[BOOT_LOADER_SCI_CONTROL_BLOCK_B].buffer_full_flag = BOOT_LOADER_SCI_RECEIVE_BUFFER_EMPTY;
        error_code = 0;
    }
    else
    {
        error_code = -1;
    }
    return error_code;
}
/**********************************************************************************************************************
 End of function firm_block_read
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: const_data_block_read
 * Description  :
 * Arguments    :
 * Return Value :
 **********************************************************************************************************************/
static int32_t const_data_block_read(uint32_t *const_data, uint32_t offset)
{
    int32_t error_code = -1;
    if (BOOT_LOADER_SCI_RECEIVE_BUFFER_FULL == s_sci_buffer_control[BOOT_LOADER_SCI_CONTROL_BLOCK_A].buffer_full_flag)
    {
        memcpy(const_data, s_sci_buffer_control[BOOT_LOADER_SCI_CONTROL_BLOCK_A].buffer, FWUP_WRITE_BLOCK_SIZE);
        s_sci_buffer_control[BOOT_LOADER_SCI_CONTROL_BLOCK_A].buffer_full_flag = BOOT_LOADER_SCI_RECEIVE_BUFFER_EMPTY;
        error_code = 0;
    }
    else if (BOOT_LOADER_SCI_RECEIVE_BUFFER_FULL
            == s_sci_buffer_control[BOOT_LOADER_SCI_CONTROL_BLOCK_B].buffer_full_flag)
    {
        memcpy(const_data, s_sci_buffer_control[BOOT_LOADER_SCI_CONTROL_BLOCK_B].buffer, FWUP_WRITE_BLOCK_SIZE);
        s_sci_buffer_control[BOOT_LOADER_SCI_CONTROL_BLOCK_B].buffer_full_flag = BOOT_LOADER_SCI_RECEIVE_BUFFER_EMPTY;
        error_code = 0;
    }
    else
    {
        error_code = -1;
    }
    return error_code;
}
/**********************************************************************************************************************
 End of function const_data_block_read
 *********************************************************************************************************************/

/*****************************************************************************
 * Function Name: my_sci_callback
 * Description  : This is a template for an SCI Async Mode callback function.
 * Arguments    : pArgs -
 *                pointer to sci_cb_p_args_t structure cast to a void. Structure
 *                contains event and associated data.
 * Return Value : none
 ******************************************************************************/
void my_sci_callback(void *pArgs)
{
    sci_cb_args_t * p_args;

    p_args = (sci_cb_args_t*) pArgs;     /* CODE CHECKER, this is OK as a comment aligns with the cast */

    switch (p_args->event)
    {
    case SCI_EVT_RX_CHAR:

    	/* From RXI interrupt; received character data is in p_args->byte */
        if (s_sci_receive_control_block.p_sci_buffer_control->buffer_occupied_byte_size <
                (sizeof(s_sci_receive_control_block.p_sci_buffer_control->buffer)))
        {
            R_SCI_Receive(p_args->hdl,
                    &s_sci_receive_control_block.p_sci_buffer_control->
                    buffer[s_sci_receive_control_block.p_sci_buffer_control->buffer_occupied_byte_size++],
                    1);
            if ((sizeof(s_sci_receive_control_block.p_sci_buffer_control->buffer)) ==
                    s_sci_receive_control_block.p_sci_buffer_control->buffer_occupied_byte_size)
            {
#if (FLASH_CFG_CODE_FLASH_BGO == 0)
#if (BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100)  /* for RTS control */
                FWUP_CFG_PORT_SYMBOL.PODR.BIT.FWUP_CFG_BIT_SYMBOL = 1;
                FWUP_CFG_PORT_SYMBOL.PDR.BIT.FWUP_CFG_BIT_SYMBOL = 1;
                FWUP_CFG_PORT_SYMBOL.PMR.BIT.FWUP_CFG_BIT_SYMBOL = 0;
#endif /* BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100 */
#endif /* FLASH_CFG_CODE_FLASH_BGO == 0 */
                s_sci_receive_control_block.p_sci_buffer_control->buffer_occupied_byte_size = 0;
                s_sci_receive_control_block.p_sci_buffer_control->buffer_full_flag = BOOT_LOADER_SCI_RECEIVE_BUFFER_FULL;
                s_sci_receive_control_block.total_byte_size += FWUP_WRITE_BLOCK_SIZE;
                if (BOOT_LOADER_SCI_CONTROL_BLOCK_A == s_sci_receive_control_block.current_state)
                {
                    s_sci_receive_control_block.current_state = BOOT_LOADER_SCI_CONTROL_BLOCK_B;
                    s_sci_receive_control_block.p_sci_buffer_control
                            = &s_sci_buffer_control[BOOT_LOADER_SCI_CONTROL_BLOCK_B];
                }
                else
                {
                    s_sci_receive_control_block.current_state = BOOT_LOADER_SCI_CONTROL_BLOCK_A;
                    s_sci_receive_control_block.p_sci_buffer_control
                            = &s_sci_buffer_control[BOOT_LOADER_SCI_CONTROL_BLOCK_A];
                }
                g_rcv_count1++;
            }
            g_rcv_count2++;
        }
        break;
    case SCI_EVT_RXBUF_OVFL:

        /* From RXI interrupt; rx queue is full; 'lost' data is in p_args->byte
         You will need to increase buffer size or reduce baud rate */
        R_BSP_NOP();
        g_error_count1++;
        break;
    case SCI_EVT_OVFL_ERR:

        /* From receiver overflow error interrupt; error data is in p_args->byte
         Error condition is cleared in calling interrupt routine */
        R_BSP_NOP();
        g_error_count2++;
        break;
    case SCI_EVT_FRAMING_ERR:

        /* From receiver framing error interrupt; error data is in p_args->byte
         Error condition is cleared in calling interrupt routine */
        R_BSP_NOP();
        break;
    case SCI_EVT_PARITY_ERR:

        /* From receiver parity error interrupt; error data is in p_args->byte
         Error condition is cleared in calling interrupt routine */
        R_BSP_NOP();
        break;
    default:
        R_BSP_NOP(); /* Do nothing */
        break;
    }
}
/**********************************************************************************************************************
 End of function my_sci_callback
 *********************************************************************************************************************/

#if (FLASH_CFG_CODE_FLASH_BGO == 1)
/***********************************************************************************************************************
* Function Name: my_flash_callback
* Description  :
* Arguments    :
* Return Value :
***********************************************************************************************************************/
static void my_flash_callback(void *event)
{
    flash_int_cb_args_t * p_event = event;

    s_flash_error_code = FLASH_ERR_FAILURE;

    if ((FLASH_INT_EVENT_WRITE_COMPLETE == p_event->event) || (FLASH_INT_EVENT_ERASE_COMPLETE == p_event->event))
    {
        s_flash_error_code = FLASH_SUCCESS;
    }

    switch (fwup_get_status())
    {
        case FWUP_STATE_BANK1_UPDATE_LIFECYCLE_ERASE_WAIT:
            fwup_update_status(FWUP_STATE_BANK1_UPDATE_LIFECYCLE_ERASE_COMPLETE);
            break;
        case FWUP_STATE_BANK1_UPDATE_LIFECYCLE_WRITE_WAIT:
            fwup_update_status(FWUP_STATE_BANK1_UPDATE_LIFECYCLE_WRITE_COMPLETE);
            break;
        case FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_ERASE_WAIT:
            fwup_update_status(FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_ERASE_COMPLETE);
            break;
        case FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_WRITE_WAIT:
            fwup_update_status(FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_WRITE_COMPLETE);
            break;
        case FWUP_STATE_INSTALL_DATA_FLASH_ERASE_WAIT:
            fwup_update_status(FWUP_STATE_INSTALL_DATA_FLASH_ERASE_COMPLETE);
            break;
        case FWUP_STATE_BANK0_INSTALL_CODE_FLASH_ERASE_WAIT:
            fwup_update_status(FWUP_STATE_BANK0_INSTALL_CODE_FLASH_ERASE_COMPLETE);
            break;
        case FWUP_STATE_BANK0_INSTALL_CODE_FLASH_WRITE_WAIT:
            fwup_update_status(FWUP_STATE_BANK0_INSTALL_CODE_FLASH_WRITE_COMPLETE);
            break;
        case FWUP_STATE_INSTALL_DATA_FLASH_WRITE_WAIT:
            fwup_update_status(FWUP_STATE_INSTALL_DATA_FLASH_WRITE_COMPLETE);
            break;
        case FWUP_STATE_BANK1_UPDATE_CODE_FLASH_ERASE_WAIT:
            fwup_update_status(FWUP_STATE_BANK1_UPDATE_CODE_FLASH_ERASE_COMPLETE);
            break;
        case FWUP_STATE_EOL_BANK1_ERASE_WAIT:
            fwup_update_status(FWUP_STATE_EOL_BANK1_ERASE_COMPLETE);
            break;
        case FWUP_STATE_EOL_BANK1_LIFECYCLE_WRITE_WAIT:
            fwup_update_status(FWUP_STATE_EOL_BANK1_LIFECYCLE_WRITE_COMPLETE);
            break;
        case FWUP_STATE_EOL_DATA_FLASH_ERASE_WAIT:
            fwup_update_status(FWUP_STATE_EOL_DATA_FLASH_ERASE_COMPLETE);
            break;
        default:
            break;
    }
}
/**********************************************************************************************************************
 End of function my_flash_callback
 *********************************************************************************************************************/
#endif /* FLASH_CFG_CODE_FLASH_BGO == 1 */

/***********************************************************************************************************************
* Function Name: get_status_string
* Description  :
* Arguments    :
* Return Value :
***********************************************************************************************************************/
static const uint8_t* get_status_string(uint8_t status)
{
    static const uint8_t s_status_string[][64] =
    {
    { "LIFECYCLE_STATE_BLANK" },
    { "LIFECYCLE_STATE_TESTING" },
    { "LIFECYCLE_STATE_INSTALLING" },
    { "LIFECYCLE_STATE_VALID" },
    { "LIFECYCLE_STATE_INVALID" },
    { "LIFECYCLE_STATE_EOL" },
    { "LIFECYCLE_STATE_UNKNOWN" } };
    const uint8_t * p_tmp;

    switch (status)
    {
    case LIFECYCLE_STATE_BLANK:
        p_tmp = s_status_string[0];
        break;
    case LIFECYCLE_STATE_TESTING:
        p_tmp = s_status_string[1];
        break;
    case LIFECYCLE_STATE_INSTALLING:
        p_tmp = s_status_string[2];
        break;
    case LIFECYCLE_STATE_VALID:
        p_tmp = s_status_string[3];
        break;
    case LIFECYCLE_STATE_INVALID:
        p_tmp = s_status_string[4];
        break;
    case LIFECYCLE_STATE_EOL:
        p_tmp = s_status_string[5];
        break;
    default:
        p_tmp = s_status_string[6];
        break;
    }
    return p_tmp;
}
/**********************************************************************************************************************
 End of function get_status_string
 *********************************************************************************************************************/
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_BOOTLOADER */
