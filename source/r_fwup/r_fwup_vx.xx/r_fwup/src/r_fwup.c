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
 * File Name     : r_fwup.c
 * Description   : Functions for the Firmware update module.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           16.02.2021 1.00    First Release
 *           19.05.2021 1.01    Added support for RX72N,RX66T,RX130
 *           08.07.2021 1.02    Added support for RX671 and GCC
 *           10.08.2021 1.03    Added support for IAR
 *           25.03.2022 1.04    Change the supported FreeRTOS version
 *                              Select data area from DF/CF
 *                              Added support for RX140-256KB
 *********************************************************************************************************************/

/* C Runtime includes. */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "r_fwup_config.h"  /* Firmware update config definitions */

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == 0) /* Bootloader */
#include "r_smc_entry.h"
#include "r_flash_rx_if.h"
#include "r_sys_time_rx_if.h"

#if (FWUP_CFG_COMMUNICATION_FUNCTION == 0) /* FWUP_COMMUNICATION_SCI */
#include "r_sci_rx_if.h"
#include "r_sci_rx_pinset.h"
#endif

#include "base64_decode.h"
#include "code_signer_public_key.h"

/* tinycrypt */
#include "tinycrypt/sha256.h"
#include "tinycrypt/ecc.h"
#include "tinycrypt/ecc_dsa.h"
#include "tinycrypt/constants.h"

#elif (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == 1) /* FWUP_IMPLEMENTATION_NONEOS */
#include "r_smc_entry.h"
#include "r_flash_rx_if.h"
#include "r_sys_time_rx_if.h"

#if (FWUP_CFG_COMMUNICATION_FUNCTION == 0) /* FWUP_COMMUNICATION_SCI */
#include "r_sci_rx_if.h"
#include "r_sci_rx_pinset.h"
#endif

#include "base64_decode.h"
#include "code_signer_public_key.h"

/* tinycrypt */
#include "tinycrypt/sha256.h"
#include "tinycrypt/ecc.h"
#include "tinycrypt/ecc_dsa.h"
#include "tinycrypt/constants.h"

#elif (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == 2) /* FWUP_IMPLEMENTATION_AFRTOS */
/* Amazon FreeRTOS include. */
#include "FreeRTOS.h"
#include "ota_platform_interface.h"
#ifdef AMAZON_FREERTOS_ENABLE_UNIT_TESTS
#include "aws_ota_pal_test_access_define.h"
#endif

#include "iot_crypto.h"
#include "core_pkcs11.h"
#include "ota_demo_config.h"
#include "ota_platform_interface.h"

/* Renesas RX Driver Package include */
#include "platform.h"
#include "r_flash_rx_if.h"
#include "r_sys_time_rx_if.h"
#endif

#include "r_fwup_if.h"
#include "r_fwup_private.h"

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS)
const uint8_t g_code_signer_public_key[] = CODE_SIGNER_PUBLIC_KEY_PEM;
const uint32_t g_code_signer_public_key_length = sizeof(g_code_signer_public_key);
bool g_is_opened = false;
#elif (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS)
/* Specify the OTA signature algorithm we support on this platform. */
/* const char cOTA_JSON_FileSignatureKey[ OTA_FILE_SIG_KEY_STR_MAX_LENGTH ] = "sig-sha1-rsa"; */   /* FIX ME. */
const char OTA_JsonFileSignatureKey[ OTA_FILE_SIG_KEY_STR_MAX_LENGTH ] = "sig-sha256-ecdsa";   /* FIX ME. */
#else
bool g_is_opened = false;
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS */


#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS)
OtaPalStatus_t R_FWUP_CreateFileForRx( OtaFileContext_t * const pFileContext );
OtaPalStatus_t R_FWUP_Abort( OtaFileContext_t * const pFileContext );
int16_t R_FWUP_WriteBlock( OtaFileContext_t * const pFileContext,
                           uint32_t ulOffset,
                           uint8_t * const pData,
                           uint32_t ulBlockSize );
OtaPalStatus_t R_FWUP_CloseFile( OtaFileContext_t * const pFileContext );
OtaPalStatus_t R_FWUP_CheckFileSignature( OtaFileContext_t * const pFileContext );
uint8_t * R_FWUP_ReadAndAssumeCertificate( const uint8_t * const pucCertName, uint32_t * const ulSignerCertSize );
OtaPalStatus_t R_FWUP_ResetDevice( OtaFileContext_t * const pFileContext );
OtaPalStatus_t R_FWUP_ActivateNewImage( OtaFileContext_t * const pFileContext );
OtaPalStatus_t R_FWUP_SetPlatformImageState( OtaFileContext_t * const pFileContext,
                                             OtaImageState_t eState );
OtaPalImageState_t R_FWUP_GetPlatformImageState( OtaFileContext_t * const pFileContext );
int32_t fwup_verification_sha256_ecdsa (const uint8_t * pucData, uint32_t ulSize,
                                            const uint8_t * pucSignature, uint32_t ulSignatureSize);

static flash_err_t ota_flashing_task( uint8_t * block, uint32_t ulOffset, uint32_t length );
static int32_t ota_context_validate( OtaFileContext_t * pFileContext );
static void ota_context_close( OtaFileContext_t * pFileContext );
static void ota_flashing_callback( void *event );
static void ota_header_flashing_callback( void *event );
static int s_vsnprintf_safe (int8_t * s, size_t n, const int8_t * format, va_list arg);
static int s_snprintf_safe (int8_t * s,size_t n, const int8_t * format, ...);
static void s_log_printf_common (uint8_t usLoggingLevel, const int8_t * pcFormat, va_list args);
static void s_log_printf_error (const int8_t * pcFormat, ...);
static void s_log_printf_warn (const int8_t * pcFormat, ...);
static void s_log_printf_info (const int8_t * pcFormat, ...);
static void s_log_printf_info_nolf (const int8_t * pcFormat, ...);
static void s_log_printf_info_continue (const int8_t * pcFormat, ...);
static void s_log_printf_debug (const int8_t * pcFormat, ...);
#elif (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS)
static void ota_flashing_task (void * pvParameters);
static CK_RV prvGetCertificateHandle (CK_FUNCTION_LIST_PTR pxFunctionList,
                                    CK_SESSION_HANDLE xSession,
                                    const char * pcLabelName,
                                    CK_OBJECT_HANDLE_PTR pxCertHandle);
static CK_RV prvGetCertificate (const char * pcLabelName,
                                uint8_t ** ppucData,
                                uint32_t * pulDataSize);
static int32_t ota_context_validate (OtaFileContext_t * pFileContext);
static int32_t ota_context_update_user_firmware_header (OtaFileContext_t * pFileContext);
static void ota_context_close (OtaFileContext_t * pFileContext);
static void ota_flashing_callback (void * event);
static void ota_header_flashing_callback (void * event);
#else
    /* Fix me for other OS environment */
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS */

/* Abstraction function. */
#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS)
static void fwup_communication_callback (void * pArgs);
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS */
static void fwup_state_monitoring_callback (void);
#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_BOOTLOADER)
#if (FWUP_CFG_BOOT_PROTECT_ENABLE == 1)
#if (FLASH_TYPE == FLASH_TYPE_4 || FLASH_TYPE == FLASH_TYPE_1)
static flash_err_t fwup_flash_accesswindow_set(flash_access_window_config_t * addr);
#endif /* FLASH_TYPE */
#endif /* FWUP_CFG_BOOT_PROTECT_ENABLE */
#else  /* Setting other than Bootloader */
static void fwup_software_delay_ms (uint32_t delay);
#endif  /* FWUP_IMPLEMENTATION_BOOTLOADER */

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS)
static st_fragmented_block_list_t *fragmented_flash_block_list_insert (st_fragmented_block_list_t * head,
                                                                        uint32_t offset, uint8_t * binary,
                                                                        uint32_t length);
static st_fragmented_block_list_t *fragmented_flash_block_list_delete (st_fragmented_block_list_t * head,
                                                                        uint32_t offset);
static st_fragmented_block_list_t *fragmented_flash_block_list_print (st_fragmented_block_list_t * head);
static st_fragmented_block_list_t *fragmented_flash_block_list_assemble (st_fragmented_block_list_t * head,
                                                                        st_flash_block_t * flash_block);

static volatile st_load_fw_control_block_t s_load_fw_control_block;
static st_fragmented_block_list_t * fragmented_flash_block_list;
static st_fwup_control_block_t * firmware_update_control_block_bank0 =
    (st_fwup_control_block_t *)BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS;
static volatile uint32_t gs_header_flashing_task;
#elif (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS)
    /* Fix me for other OS environment */
static volatile st_load_fw_control_block_t s_load_fw_control_block;
static volatile uint32_t gs_header_flashing_task;
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS */

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_BOOTLOADER)
static sci_hdl_t s_fwup_communication_handle;

#elif (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS)
static st_fwup_control_block_t * sp_fwup_control_block_bank1 =
    (st_fwup_control_block_t*)BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS;
static OtaFileContext_t s_file_context;
static sci_hdl_t s_fwup_communication_handle;
static st_sci_receive_control_block_t s_sci_receive_control_block;
static st_sci_buffer_control_t s_sci_buffer_control[FWUP_SCI_CONTROL_BLOCK_TOTAL_NUM];
#elif (FWUP_ENV_COMMUNICATION_FUNCTION == FWUP_COMM_ETHER_AFRTOS)
static QueueHandle_t xQueue;
static TaskHandle_t xTask;
static xSemaphoreHandle xSemaphoreFlashig;
static xSemaphoreHandle xSemaphoreWriteBlock;
static st_packet_block_for_queue_t packet_block_for_queue1;
static st_packet_block_for_queue_t packet_block_for_queue2;
#else
    /* Fix me for other OS environment */
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_BOOTLOADER */

static e_fwup_state_t s_fwup_state = FWUP_STATE_INITIALIZING;
static st_state_monitoring_t s_state_transit;

/*-----------------------------------------------------------*/
#if ((FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_BOOTLOADER) || (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS))
/***********************************************************************************************************************
 * Function Name: R_FWUP_Open
 *******************************************************************************************************************//**
 * @brief   Firmware Update module Initialization Processing.
 * @retval  FWUP_SUCCESS                Firmware Update module was opened successfully.
 * @retval  FWUP_ERR_ALREADY_OPEN       Firmware Update module is in use by another process.
 * @retval  FWUP_ERR_LESS_MEMORY        The size of the RAM area used by the Firmware Update module is insufficient.
 * @retval  FWUP_ERR_IMAGE_STATE        Platform image status not suitable for firmware update.
 * @retval  FWUP_ERR_FLASH              Detect error of r_flash_rx module.
 * @retval  FWUP_ERR_COMM               Detect error of communication module.
 * @retval  FWUP_ERR_STATE_MONITORING   Detect error of state monitoring module.
 * @details This function provides setting Firmware Update module initialization.
 *          When this function is executed, Firmware Update module is ready to be used.
 */
fwup_err_t R_FWUP_Open(void)
{
    fwup_err_t ret = FWUP_SUCCESS;
    e_comm_err_t comm_api_error_code = COMM_SUCCESS;
    e_state_monitoring_err_t state_monitoring_api_error_code = MONI_SUCCESS;
#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_BOOTLOADER)
    flash_err_t flash_api_error_code = FLASH_SUCCESS;
#elif (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS)
    OtaPalStatus_t ota_error_code;
#endif  /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT */

    /* Check that the R_FWUP_Open has been executed. */
    if (true == g_is_opened)
    {
        return FWUP_ERR_ALREADY_OPEN;
    }

    /* Initialization of Communication module. */
    comm_api_error_code = fwup_communication_open();
    if (COMM_SUCCESS != comm_api_error_code)
    {
        return FWUP_ERR_COMM;
    }

    /* Set up the configuration of System-timer for check the status transition. */
    state_monitoring_api_error_code = fwup_state_monitoring_open();
    if (MONI_SUCCESS != state_monitoring_api_error_code)
    {
        comm_api_error_code = fwup_communication_close();  // Closing the Communication module.
        return FWUP_ERR_STATE_MONITORING;
    }

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_BOOTLOADER)
    /* Initialization of Flash module. */
    flash_api_error_code = fwup_flash_open();
    if (FLASH_SUCCESS != flash_api_error_code)
    {
        comm_api_error_code             = fwup_communication_close();       // Closing the Communication module.
        state_monitoring_api_error_code = fwup_state_monitoring_close();    // Closing the State monitoring module.
        return FWUP_ERR_FLASH;
    }

    fwup_update_status(FWUP_STATE_INITIALIZING);
#elif (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS)
    /* Initialization of Flash module. */
    ota_error_code = R_FWUP_CreateFileForRx( &s_file_context );
    if ( OtaPalSuccess != ota_error_code )
    {
        comm_api_error_code             = fwup_communication_close();       // Closing the Communication module.
        state_monitoring_api_error_code = fwup_state_monitoring_close();    // Closing the State monitoring module.
        return FWUP_ERR_FLASH;
    }

    /* Initialize receive buffer */
    s_sci_buffer_control[FWUP_SCI_CONTROL_BLOCK_A].buffer_full_flag = FWUP_SCI_RECEIVE_BUFFER_EMPTY;
    s_sci_buffer_control[FWUP_SCI_CONTROL_BLOCK_B].buffer_full_flag = FWUP_SCI_RECEIVE_BUFFER_EMPTY;
    s_sci_receive_control_block.p_sci_buffer_control                = & s_sci_buffer_control[FWUP_SCI_CONTROL_BLOCK_A];
    s_sci_receive_control_block.current_state                       = FWUP_SCI_CONTROL_BLOCK_A;

    fwup_update_status(FWUP_STATE_DATA_RECEIVE_START);
#endif  /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT */
    g_is_opened = true;
    return ret;
}
/**********************************************************************************************************************
 End of function R_FWUP_Open
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_FWUP_Close
 *******************************************************************************************************************//**
 * @brief   Firmware Update module Close Processing.
 * @retval  FWUP_SUCCESS                Firmware Update module was closed successfully.
 * @retval  FWUP_ERR_NOT_OPEN           Firmware Update module is not open.
 * @retval  FWUP_ERR_FLASH              Detect error of r_flash_rx module.
 * @retval  FWUP_ERR_COMM               Detect error of communication module.
 * @retval  FWUP_ERR_STATE_MONITORING   Detect error of state monitoring module.
 * @details This function provides close resouces of Firmware Update module.
 */
fwup_err_t R_FWUP_Close(void)
{
    fwup_err_t ret = FWUP_SUCCESS;

    /* Check that the R_FWUP_Open has not been executed. */
    if (false == g_is_opened)
    {
        return FWUP_ERR_NOT_OPEN;
    }
    g_is_opened = false;
    return ret;
}
/**********************************************************************************************************************
 End of function R_FWUP_Close
 *********************************************************************************************************************/
#endif  /* (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_BOOTLOADER) || (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS) */

/***********************************************************************************************************************
 * Function Name: R_FWUP_GetVersion
 *******************************************************************************************************************//**
 * @brief   Returns the current version of the FWUP FIT module.
 * @return  Version of the FWUP FIT module.
 * @details This function will return the version of the currently installed FIT module.
 *          The version number is encoded where the top 2 bytes are the major version number
 *          and the bottom 2 bytes are the minor version number.
 *          For example, Version 4.25 would be returned as 0x00040019.
 */
uint32_t R_FWUP_GetVersion(void)
{
    /* These version macros are defined in r_flash_if.h. */
    return ((((uint32_t)FWUP_VERSION_MAJOR) << 16) | (uint32_t)FWUP_VERSION_MINOR);
}
/**********************************************************************************************************************
 End of function R_FWUP_GetVersion
 *********************************************************************************************************************/

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS)
/***********************************************************************************************************************
 * Function Name: R_FWUP_Operation
 *******************************************************************************************************************//**
 * @brief   Operation of Firmware update module for OS-less environment.
 * @retval  FWUP_SUCCESS                Firmware update completed successfully.
 * @retval  FWUP_FAIL                   Firmware update error occurred.
 * @retval  FWUP_IN_PROGRESS            Firmware update in progress.
 * @retval  FWUP_ERR_NOT_OPEN           Firmware Update module is not open.
 * @retval  FWUP_ERR_IMAGE_STATE        Platform image status not suitable for firmware update.
 * @retval  FWUP_ERR_STATE_MONITORING   The firmware update status has not changed for a certain period of time.
 * @details Operation of Firmware update module for OS-less environment.
 *          - Obtaining firmware data for update from the set communication path
 *          - Programming to flash
 *          - Signature verification
 *          If the return value is "FWUP_IN_PROGRESS", the firmware update is in progress, so call this function again.
 *          If the return value is "FWUP_SUCCESS", the firmware update is complete. The process is transferred to the
 *          updated firmware by performing a software reset.
 *          If the return value is "FWUP_FAIL", it means that the firmware update has failed.
 *          Cancel the error and call this function again.
 */
fwup_err_t R_FWUP_Operation(void)
{
    fwup_err_t ret = FWUP_IN_PROGRESS;
    e_state_monitoring_err_t state_monitoring_api_error_code = MONI_SUCCESS;
    flash_interrupt_config_t cb_func_info;
    uint8_t firm_data[FWUP_WRITE_BLOCK_SIZE];
    int32_t i_bytes_written;
    bool write_flag = false;
    st_fwup_control_block_t * p_block_header;
    Sig256_t p_sig;

    /* Check that the Firmware update module has been opened. */
    if (false == g_is_opened)
    {
        fwup_update_status(FWUP_STATE_FATAL_ERROR);
        LogError( ( "ERROR: R_FWUP_Open is not called." ) );
        return FWUP_ERR_NOT_OPEN;
    }

    /* Check State transit monitoring flag */
    if (fwup_state_monitoring_is_error() == STATE_MONITORING_ERROR)
    {
        fwup_update_status(FWUP_STATE_FATAL_ERROR);
        LogError( ( "ERROR: Transit state did not change for more than 1 min." ) );
        return FWUP_ERR_STATE_MONITORING;
    }

    if (fwup_get_status() == FWUP_STATE_DATA_RECEIVE_START)
    {
        /* Check the Platform image state */
        if (R_FWUP_GetPlatformImageState(&s_file_context) != OtaPalImageStateValid)
        {
            fwup_update_status(FWUP_STATE_FATAL_ERROR);
            LogError(("ERROR: Illegal Image State."));
            return FWUP_ERR_IMAGE_STATE;
        }

        /* Start the state monitoring */
        state_monitoring_api_error_code = fwup_state_monitoring_start();
        if (MONI_SUCCESS == state_monitoring_api_error_code)
        {
            fwup_update_status(FWUP_STATE_DATA_RECEIVE);
            LogInfo(("-------------------------------------------------"));
            LogInfo(("Firmware update user program"));
            LogInfo(("-------------------------------------------------"));

            /* Erase Temporary area */
#if (FLASH_CFG_CODE_FLASH_BGO == 1)
            cb_func_info.pcallback    = ota_header_flashing_callback;
            cb_func_info.int_priority = FLASH_INTERRUPT_PRIORITY;
            fwup_flash_set_callback((void *)&cb_func_info);
            gs_header_flashing_task = OTA_FLASHING_IN_PROGRESS;
#endif /* FLASH_CFG_CODE_FLASH_BGO == 1 */

            if (fwup_flash_erase((flash_block_address_t)BOOT_LOADER_UPDATE_TEMPORARY_AREA_ERASE_ADDRESS,
                                    BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER) == FLASH_SUCCESS)
            {
#if (FLASH_CFG_CODE_FLASH_BGO == 1)
                while (OTA_FLASHING_IN_PROGRESS == gs_header_flashing_task)
                {
                    ;
                }
#endif /* FLASH_CFG_CODE_FLASH_BGO == 1 */
                fwup_flash_close();
                fwup_flash_open();
#if (FLASH_CFG_CODE_FLASH_BGO == 1)
                cb_func_info.pcallback    = ota_flashing_callback;
                cb_func_info.int_priority = FLASH_INTERRUPT_PRIORITY;
                fwup_flash_set_callback(&cb_func_info);
#endif /* FLASH_CFG_CODE_FLASH_BGO == 1 */

                LogInfo(("Send Update firmware via UART."));
            }
            else
            {
                LogError(("ERROR - R_FLASH_Erase() returns error."));
                return FWUP_ERR_FLASH;
            }
        }
        else
        {
            return FWUP_ERR_STATE_MONITORING;
        }
    }

    switch (fwup_get_status())
    {
        case FWUP_STATE_INITIALIZING:

            /* Firmware Update module is not open */
            fwup_update_status(FWUP_STATE_FATAL_ERROR);
            LogError(("R_FWUP_Open is not called."));
            ret = FWUP_ERR_NOT_OPEN;
            break;
        case FWUP_STATE_DATA_RECEIVE_START:

            /* Obtaining firmware data for update from the communication module */
            ret = FWUP_IN_PROGRESS;
            break;
        case FWUP_STATE_DATA_RECEIVE:

            /* Write firmware data to Flash */
            if (FWUP_SCI_RECEIVE_BUFFER_FULL == s_sci_buffer_control[FWUP_SCI_CONTROL_BLOCK_A].buffer_full_flag)
            {
                memcpy(firm_data, s_sci_buffer_control[FWUP_SCI_CONTROL_BLOCK_A].buffer, FWUP_WRITE_BLOCK_SIZE);
                s_sci_buffer_control[FWUP_SCI_CONTROL_BLOCK_A].buffer_full_flag = FWUP_SCI_RECEIVE_BUFFER_EMPTY;
                write_flag = true;
            }
            else if (FWUP_SCI_RECEIVE_BUFFER_FULL == s_sci_buffer_control[FWUP_SCI_CONTROL_BLOCK_B].buffer_full_flag)
            {
                memcpy(firm_data, s_sci_buffer_control[FWUP_SCI_CONTROL_BLOCK_B].buffer, FWUP_WRITE_BLOCK_SIZE);
                s_sci_buffer_control[FWUP_SCI_CONTROL_BLOCK_B].buffer_full_flag = FWUP_SCI_RECEIVE_BUFFER_EMPTY;
                write_flag = true;
            }
            else
            {
                ;  /* Do nothing */
            }

            if (true == write_flag)
            {
                uint32_t u_offset;
                fwup_update_status(FWUP_STATE_FLASH_WRITE_WAIT);
                u_offset        = s_load_fw_control_block.total_image_length;
                i_bytes_written = R_FWUP_WriteBlock(&s_file_context, s_load_fw_control_block.total_image_length,
                                                    firm_data, FWUP_WRITE_BLOCK_SIZE);
                if (i_bytes_written < 0)
                {
                    LogError(("ERROR: (%d) writing file block", i_bytes_written));
                    ret = FWUP_FAIL;
                }
                else
                {
                	LogInfoNoLF(("Flash Write: Address = 0x%X, length = %dbyte ... ",
                	        ((uint32_t)BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS + u_offset),
                	        FWUP_WRITE_BLOCK_SIZE));
#if (FLASH_CFG_DATA_FLASH_BGO == 0)
                    fwup_update_status(FWUP_STATE_FLASH_WRITE_COMPLETE);
#endif  /* (FLASH_CFG_DATA_FLASH_BGO == 0) */
                    ret = FWUP_IN_PROGRESS;
                }
            }
            break;
        case FWUP_STATE_FLASH_WRITE_WAIT:

            /* Waiting for writing to Flash */
            ret = FWUP_IN_PROGRESS;
            break;
        case FWUP_STATE_FLASH_WRITE_COMPLETE:
            LogInfoContinue(("OK"));

            /* Writing to Flash completed */
            if (BOOT_LOADER_TOTAL_UPDATE_SIZE == s_load_fw_control_block.total_image_length)
            {
                fwup_update_status(FWUP_STATE_CHECK_SIGNATURE);
                LogInfo(("Firmware update to Flash is complete."));
            }
            else
            {
                fwup_update_status(FWUP_STATE_DATA_RECEIVE);
            }
            ret = FWUP_IN_PROGRESS;
            break;
        case FWUP_STATE_CHECK_SIGNATURE:

            /* Update Signature information to OTA_FileContext */
            p_block_header = (st_fwup_control_block_t *)sp_fwup_control_block_bank1;
            p_sig.size     = p_block_header->signature_size;
            memcpy(p_sig.data, p_block_header->signature, kOTA_MaxSignatureSize);
            s_file_context.pSignature = &p_sig;

            /* Perform signature verification, and close process */
            if (kOTA_Err_None == R_FWUP_CloseFile(&s_file_context))
            {
                LogInfo(("Check signature of update firmware is complete."));
                R_FWUP_SetPlatformImageState(&s_file_context, OtaImageStateTesting);
                fwup_update_status(FWUP_STATE_FINALIZE);
                ret = FWUP_SUCCESS;
            }
            else
            {
                LogInfo(("Check signature of update firmware was failed."));
                fwup_update_status(FWUP_STATE_FATAL_ERROR);
                ret = FWUP_FAIL;
            }
            break;
        case FWUP_STATE_FINALIZE:

            /* Already Firmware update completed */
            ret = FWUP_SUCCESS;
            break;
        default:
            break;
    }
    return ret;
}
/**********************************************************************************************************************
 End of function R_FWUP_Operation
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_FWUP_SetEndOfLife
 *******************************************************************************************************************//**
 * @brief   Start End Of Life (EOL) process.
 * @retval  FWUP_SUCCESS                Firmware update completed successfully.
 * @retval  FWUP_ERR_NOT_OPEN           Firmware Update module is not open.
 * @retval  FWUP_ERR_IMAGE_STATE        Platform image status not suitable for EOL process.
 * @retval  FWUP_ERR_FLASH              Detect error of r_flash_rx module.
 * @retval  FWUP_ERR_COMM               Detect error of communication module.
 * @details Start End Of Life (EOL) process.
 */
fwup_err_t R_FWUP_SetEndOfLife(void)
{
    fwup_err_t                ret = FWUP_SUCCESS;
    uint8_t                   block[BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH];
    st_fwup_control_block_t * p_block_header;
    uint32_t                  length;
    flash_err_t               flash_err;

    if (false == g_is_opened)
    {
        fwup_update_status(FWUP_STATE_FATAL_ERROR);
        LogError(("R_FWUP_Open is not called."));
        return FWUP_ERR_NOT_OPEN;
    }

    LogInfo(("-------------------------------------------------"));
    LogInfo(("End Of Life (EOL) process of user program"));
    LogInfo(("-------------------------------------------------"));

#if (FLASH_CFG_DATA_FLASH_BGO == 1)
    flash_interrupt_config_t cb_func_info;

    cb_func_info.pcallback    = ota_header_flashing_callback;
    cb_func_info.int_priority = FLASH_INTERRUPT_PRIORITY;
    flash_err                 = fwup_flash_set_callback((void *)&cb_func_info);

    if (FLASH_SUCCESS == flash_err)
    {
        gs_header_flashing_task = OTA_FLASHING_IN_PROGRESS;

        if (fwup_flash_erase((flash_block_address_t)BOOT_LOADER_UPDATE_TEMPORARY_AREA_ERASE_ADDRESS,
                BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER) == FLASH_SUCCESS)
        {
            while (OTA_FLASHING_IN_PROGRESS == gs_header_flashing_task)
            {
                ;
            }
            LogInfo(("erase install area (code flash):OK"));
        }
        else
        {
            LogInfo(("erase install area (code flash):NG"));
            ret = FWUP_ERR_FLASH;
        }
    }
    else
    {
        ret = FWUP_ERR_FLASH;
    }
#else
    if (fwup_flash_erase((flash_block_address_t)BOOT_LOADER_UPDATE_TEMPORARY_AREA_ERASE_ADDRESS,
            BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER) != FLASH_SUCCESS)
    {
        LogInfo(("erase install area (code flash):NG"));
        ret = FWUP_ERR_FLASH;
    }
    else
    {
        LogInfo(("erase install area (code flash):OK"));
    }
#endif /* FLASH_CFG_DATA_FLASH_BGO == 1 */

    if (FWUP_SUCCESS == ret)
    {
        /* Set EOL to image flag of bank1. */
        memcpy(block, (const void *)sp_fwup_control_block_bank1, BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH);
        p_block_header             = (st_fwup_control_block_t *)block;
        p_block_header->image_flag = LIFECYCLE_STATE_EOL;
        length                     = BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH;

#if (FLASH_CFG_DATA_FLASH_BGO == 1)
        /* Write new image_flag and new signature to Header. */
        gs_header_flashing_task = OTA_FLASHING_IN_PROGRESS;
        flash_err               = fwup_flash_write((uint32_t)block,
                                    (uint32_t)BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS,
                                    length);

        if (FLASH_SUCCESS != flash_err)
        {
            ret = FWUP_ERR_FLASH;
        }
        else
        {
            while (OTA_FLASHING_IN_PROGRESS == gs_header_flashing_task)
            {
                ;
            }
            LogInfo(("update bank1 LIFECYCLE_STATE to [LIFECYCLE_STATE_EOL]"));
            s_load_fw_control_block.eSavedAgentState = OtaImageState_EOL;
        }
#else
        /* Write new image_flag and new signature to Header. */
        flash_err = fwup_flash_write((uint32_t)block, (uint32_t)BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS, length);
        if(FLASH_SUCCESS != flash_err)
        {
            ret = FWUP_ERR_FLASH;
        }
        else
        {
            LogInfo(("update bank1 LIFECYCLE_STATE to [LIFECYCLE_STATE_EOL]"));
            s_load_fw_control_block.eSavedAgentState = OtaImageState_EOL;
        }
#endif /* FLASH_CFG_DATA_FLASH_BGO == 1 */
    }
    return ret;
}
/**********************************************************************************************************************
 End of function R_FWUP_SetEndOfLife
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_FWUP_SoftwareReset
 *******************************************************************************************************************//**
 * @brief   Execute a software reset.
 * @details Execute a software reset.
 */
void R_FWUP_SoftwareReset(void)
{
    (void)R_FWUP_ActivateNewImage(&s_file_context);
}
/**********************************************************************************************************************
 End of function R_FWUP_SoftwareReset
 *********************************************************************************************************************/

#endif  /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS */
/*-----------------------------------------------------------*/

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT != FWUP_IMPLEMENTATION_BOOTLOADER)
OtaPalStatus_t R_FWUP_CreateFileForRx( OtaFileContext_t * const pFileContext )
{
    LogDebug( ( "R_FWUP_CreateFileForRx is called." ) );
    LogDebug( ( "Compiled in [%s] [%s].", __DATE__, __TIME__ ) );
    OtaPalMainStatus_t eResult = OtaPalUninitialized;

    if (NULL != pFileContext)
    {
#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS)
#if (FLASH_CFG_CODE_FLASH_BGO == 1)
        fwup_flash_close();
#endif
        if (fwup_flash_open() == FLASH_SUCCESS)
        {
            s_load_fw_control_block.OtaFileContext     = pFileContext;
            s_load_fw_control_block.total_image_length = 0;
            s_load_fw_control_block.eSavedAgentState   = OtaImageStateUnknown;
            LogInfo(("Receive file created."));
            pFileContext->pFile                        = (uint8_t *) &s_load_fw_control_block;
            eResult                                    = OtaPalSuccess;
        }
        else
        {
            eResult = OtaPalRxFileCreateFailed;
            LogError(("ERROR - R_FLASH_Open() returns error."));
        }
#elif (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS)
        flash_interrupt_config_t cb_func_info;

        if( pFileContext->pFilePath != NULL )
        {
            /* create task/queue/semaphore for flashing */
            xQueue = xQueueCreate( otaconfigMAX_NUM_BLOCKS_REQUEST, sizeof( st_packet_block_for_queue_t ) );
            xTaskCreate( ota_flashing_task, "OTA_FLASHING_TASK",
                         configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES, &xTask );
            xSemaphoreFlashig = xSemaphoreCreateBinary();
            xSemaphoreGive( xSemaphoreFlashig );
            xSemaphoreWriteBlock = xSemaphoreCreateMutex();
            xSemaphoreGive( xSemaphoreWriteBlock );
            fragmented_flash_block_list = NULL;

            fwup_flash_close();
            if(fwup_flash_open() == FLASH_SUCCESS)
            {
                cb_func_info.pcallback = ota_header_flashing_callback;
                cb_func_info.int_priority = FLASH_INTERRUPT_PRIORITY;
                fwup_flash_set_callback((void *)&cb_func_info);
                gs_header_flashing_task = OTA_FLASHING_IN_PROGRESS;
                if (fwup_flash_erase((flash_block_address_t)BOOT_LOADER_UPDATE_TEMPORARY_AREA_ERASE_ADDRESS,
                                    BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER) != FLASH_SUCCESS)
                {
                    eResult = OtaPalRxFileCreateFailed;
                    LogError( ( "Failed to erase the flash: R_FLASH_Erase() returns error." ) );
                }
                else
                {
                    while( OTA_FLASHING_IN_PROGRESS == gs_header_flashing_task )
                    {
                    }
                    fwup_flash_close();
                    fwup_flash_open();
                    cb_func_info.pcallback = ota_flashing_callback;
                    cb_func_info.int_priority = FLASH_INTERRUPT_PRIORITY;
                    fwup_flash_set_callback(&cb_func_info);
                    s_load_fw_control_block.OtaFileContext = pFileContext;
                    s_load_fw_control_block.total_image_length = 0;
                    s_load_fw_control_block.eSavedAgentState = OtaImageStateUnknown;
                    LogInfo( ( "Receive file created." ) );
                    pFileContext->pFile = ( uint8_t * ) &s_load_fw_control_block;
                    fwup_state_monitoring_open();  /* Open State monitoring module */
                    fwup_state_monitoring_start();  /* Start State monitoring module */
                    fwup_update_status(FWUP_STATE_DATA_RECEIVE_START); /* Update the firmware update status */
                    eResult = OtaPalSuccess;
                }
            }
            else
            {
                eResult = OtaPalRxFileCreateFailed;
                LogError( ( "Failed to create a file: R_FLASH_Open() returns error." ) );
            }
        }
        else
        {
            eResult = OtaPalRxFileCreateFailed;
            LogError( ( "Failed to create a file: Invalid file context provided." ) );
        }
#else /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS */
    /* Fix me for other OS environment */
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS */
    }
    else
    {
        eResult = OtaPalRxFileCreateFailed;
        LogError(("Failed to create a file: Invalid file context provided."));
    }

    return OTA_PAL_COMBINE_ERR(eResult, 0);
}
/*-----------------------------------------------------------*/

OtaPalStatus_t R_FWUP_Abort( OtaFileContext_t * const pFileContext )
{
    LogDebug( ( "R_FWUP_Abort is called." ) );

    OtaPalMainStatus_t eResult = OtaPalSuccess;

    if (ota_context_validate(pFileContext) == 0)
    {
        eResult = OtaPalFileClose;
    }

    if (OtaPalSuccess == eResult)
    {
#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS)
        /* delete task/queue for flashing */
        if( NULL != xTask )
        {
            vTaskDelete( xTask );
            xTask = NULL;
        }

        if( NULL != xQueue )
        {
            vQueueDelete( xQueue );
            xQueue = NULL;
        }

        if( NULL != xSemaphoreFlashig )
        {
            vSemaphoreDelete( xSemaphoreFlashig );
            xSemaphoreFlashig = NULL;
        }

        if( NULL != xSemaphoreWriteBlock )
        {
            vSemaphoreDelete( xSemaphoreWriteBlock );
            xSemaphoreWriteBlock = NULL;
        }
#else
    /* Fix me for other OS environment */
#endif  /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS */

        fwup_flash_close();
    }

    ota_context_close(pFileContext);
    return OTA_PAL_COMBINE_ERR( eResult, 0 );
}
/*-----------------------------------------------------------*/

/* Write a block of data to the specified file. */
int16_t R_FWUP_WriteBlock( OtaFileContext_t * const pFileContext,
                            uint32_t ulOffset,
                            uint8_t * const pData,
                            uint32_t ulBlockSize )
{
    int16_t sNumBytesWritten = R_OTA_ERR_QUEUE_SEND_FAIL;

    LogDebug( ( "R_FWUP_WriteBlock is called." ) );

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS)
    st_flash_block_t flash_block;
    static uint8_t flash_block_array[FLASH_CF_MIN_PGM_SIZE];
    uint8_t *packet_buffer;

    /* Check State transit monitoring flag */
    if (STATE_MONITORING_ERROR == fwup_state_monitoring_is_error())
    {
        fwup_update_status(FWUP_STATE_FATAL_ERROR);
        LogError( ( "ERROR: Transit state did not change for more than 1 min." ) );
        return sNumBytesWritten;
    }
    xSemaphoreTake( xSemaphoreWriteBlock, portMAX_DELAY );

    if( ( !( ulOffset % FLASH_CF_MIN_PGM_SIZE ) ) && ( !( ulBlockSize % FLASH_CF_MIN_PGM_SIZE ) ) )
    {
        fwup_update_status(FWUP_STATE_FLASH_WRITE_WAIT);   /* Update the firmware update status */
        packet_buffer = pvPortMalloc( ulBlockSize );
        memcpy( packet_buffer, pData, ulBlockSize );
        packet_block_for_queue1.p_packet = packet_buffer;
        packet_block_for_queue1.ulOffset = ulOffset;
        packet_block_for_queue1.length = ulBlockSize;

        if( xQueueSend( xQueue, &packet_block_for_queue1, NULL ) == pdPASS )
        {
            sNumBytesWritten = ( int16_t ) ulBlockSize;
        }
        else
        {
            vPortFree( packet_block_for_queue1.p_packet );
            LogError( ( "Failed to write a block: OTA flashing queue send error." ) );
            sNumBytesWritten = R_OTA_ERR_QUEUE_SEND_FAIL;
        }
    }
    else
    {
        flash_block.p_binary = flash_block_array;

        fragmented_flash_block_list = fragmented_flash_block_list_insert( fragmented_flash_block_list,
                                                                            ulOffset, pData, ulBlockSize );

        if( fragmented_flash_block_list != NULL )
        {
            while( 1 )
            {
                fragmented_flash_block_list = fragmented_flash_block_list_assemble( fragmented_flash_block_list,
                                                                                    &flash_block );

                if( flash_block.p_binary != NULL )
                {
                    fwup_update_status(FWUP_STATE_FLASH_WRITE_WAIT);   /* Update the firmware update status */
                    packet_buffer = pvPortMalloc( flash_block.length );
                    memcpy( packet_buffer, flash_block.p_binary, flash_block.length );
                    packet_block_for_queue1.p_packet = packet_buffer;
                    packet_block_for_queue1.ulOffset = flash_block.offset;
                    packet_block_for_queue1.length = flash_block.length;

                    if( xQueueSend( xQueue, &packet_block_for_queue1, NULL ) != pdPASS )
                    {
                        vPortFree( packet_block_for_queue1.p_packet );
                        LogError( ( "Failed to write a block: OTA flashing queue send error." ) );
                        sNumBytesWritten = R_OTA_ERR_QUEUE_SEND_FAIL;
                        break;
                    }
                }
                else
                {
                    sNumBytesWritten = ( int16_t ) ulBlockSize;
                    break;
                }
            }

            /*----------- finalize phase ----------*/
            fragmented_flash_block_list_print( fragmented_flash_block_list );
        }
        else
        {
            sNumBytesWritten = ( int16_t ) ulBlockSize;
        }
    }

    xSemaphoreGive( xSemaphoreWriteBlock );
#elif (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS)
    if (ota_flashing_task(pData, ulOffset, ulBlockSize) == FLASH_SUCCESS)
    {
        sNumBytesWritten = (int16_t) ulBlockSize;
    }
    else
    {
        LogInfo(("OTA flashing queue send error."));
        sNumBytesWritten = R_OTA_ERR_QUEUE_SEND_FAIL;
    }
#else
    /* Fix me for other OS environment */
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS */

    return sNumBytesWritten;
}
/*-----------------------------------------------------------*/

OtaPalStatus_t R_FWUP_CloseFile( OtaFileContext_t * const pFileContext )
{
    OtaPalMainStatus_t eResult = OtaPalSuccess;

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS)
    /* Check State transit monitoring flag */
    if (STATE_MONITORING_ERROR == fwup_state_monitoring_is_error())
    {
        fwup_update_status(FWUP_STATE_FATAL_ERROR);
        LogError( ( "ERROR: Transit state did not change for more than 1 min." ) );
        return OtaPalFileClose;
    }
    else
    {
        fwup_update_status(FWUP_STATE_CHECK_SIGNATURE);   /* Update the firmware update status */
    }
#endif
    if (ota_context_validate(pFileContext) == 0)
    {
        eResult = OtaPalFileClose;
    }
    if( OtaPalSuccess == eResult )
    {
        if (NULL != pFileContext->pSignature)
        {
            eResult = OTA_PAL_MAIN_ERR(R_FWUP_CheckFileSignature(pFileContext));
        }
        else
        {
            eResult = OtaPalSignatureCheckFailed;
        }
    }
#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS)
    fwup_flash_close();
#elif (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS)
    if( OtaPalSuccess == eResult )
    {
        /* Update the user firmware header. */
        if( ota_context_update_user_firmware_header( pFileContext ) == R_OTA_ERR_NONE )
        {
            LogInfo( ( "User firmware image header updated." ) );
        }
        else
        {
            LogError( ( "Failed to update the firmware image header." ) );
            eResult = OtaPalFileClose;
        }

        /* delete task/queue for flashing */
        if( NULL != xTask )
        {
            vTaskDelete( xTask );
            xTask = NULL;
        }

        if( NULL != xQueue )
        {
            do
            {
                if( errQUEUE_FULL == xQueueReceive( xQueue, &packet_block_for_queue2, 0 ) )
                {
                    break;
                }
                else
                {
                    vPortFree( packet_block_for_queue2.p_packet );
                }
            }
            while( 1 );

            vQueueDelete( xQueue );
            xQueue = NULL;
        }

        if( NULL != xSemaphoreFlashig )
        {
            vSemaphoreDelete( xSemaphoreFlashig );
            xSemaphoreFlashig = NULL;
        }

        if( NULL != xSemaphoreWriteBlock )
        {
            vSemaphoreDelete( xSemaphoreWriteBlock );
            xSemaphoreWriteBlock = NULL;
        }

        /* Check State transit monitoring flag */
        if (STATE_MONITORING_ERROR == fwup_state_monitoring_is_error())
        {
            fwup_update_status(FWUP_STATE_FATAL_ERROR);
            LogError( ( "ERROR: Transit state did not change for more than 1 min." ) );
            eResult = OtaPalFileClose;
        }
        else
        {
            fwup_update_status(FWUP_STATE_FINALIZE);   /* Update the firmware update status */
        }
        fwup_flash_close();
        fwup_state_monitoring_close();
    }
    else
    {
        s_load_fw_control_block.eSavedAgentState = OtaImageStateRejected;
    }
#else /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS */
    /* Fix me for other OS environment */
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS */

    ota_context_close( pFileContext );
    return OTA_PAL_COMBINE_ERR( eResult, 0 );
}
/*-----------------------------------------------------------*/

OtaPalStatus_t R_FWUP_CheckFileSignature( OtaFileContext_t * const pFileContext )
{
    OtaPalMainStatus_t eResult = OtaPalUninitialized;

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS)
    int32_t verification_result = -1;

    /* Firmware verification for the signature type of bank 1. */
    verification_result = fwup_verification_sha256_ecdsa(
            (const uint8_t *)BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS + BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
            (BOOT_LOADER_FLASH_CF_BLOCK_SIZE * BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER) -
                BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
            sp_fwup_control_block_bank1->signature,
            sp_fwup_control_block_bank1->signature_size);

    if (0 == verification_result)
    {
        eResult = OtaPalSuccess;
    }
    else
    {
        eResult = OtaPalSignatureCheckFailed;
    }

#elif (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS)
    uint32_t ulSignerCertSize;
    void * pvSigVerifyContext;
    uint8_t * pucSignerCert = NULL;
    uint32_t flash_aligned_address = 0;
    uint8_t assembled_flash_buffer[FLASH_CF_MIN_PGM_SIZE];
    flash_err_t flash_err;
    flash_interrupt_config_t cb_func_info;

    if ( fragmented_flash_block_list != NULL )
    {
        st_fragmented_block_list_t *tmp = fragmented_flash_block_list;
        do
        {
            /* Read one page from flash memory. */
            flash_aligned_address = ((tmp->content.offset & OTA_FLASH_MIN_PGM_SIZE_MASK) +
                                        (uint32_t)BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS +
                                            BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH);
            memcpy( ( uint8_t * ) assembled_flash_buffer, ( uint8_t * ) flash_aligned_address, FLASH_CF_MIN_PGM_SIZE);
            /* Replace length bytes from offset. */
            memcpy( &assembled_flash_buffer[ tmp->content.offset ], tmp->content.p_binary, tmp->content.length);
            /* Flashing memory. */
            xSemaphoreTake( xSemaphoreFlashig, portMAX_DELAY );
            fwup_flash_close();
            fwup_flash_open();
            cb_func_info.pcallback = ota_header_flashing_callback;
            cb_func_info.int_priority = FLASH_INTERRUPT_PRIORITY;
            fwup_flash_set_callback((void *)&cb_func_info);
            gs_header_flashing_task = OTA_FLASHING_IN_PROGRESS;
            flash_err = fwup_flash_write( ( uint32_t )assembled_flash_buffer,
                                        ( uint32_t )flash_aligned_address, FLASH_CF_MIN_PGM_SIZE);
            if( flash_err != FLASH_SUCCESS )
            {
                R_BSP_NOP();  /* When an error occurs, consider an error notification method according to the system. */
            }
            while( OTA_FLASHING_IN_PROGRESS == gs_header_flashing_task )
            {
            }

            xSemaphoreGive( xSemaphoreFlashig );
            s_load_fw_control_block.total_image_length += tmp->content.length;
            tmp = fragmented_flash_block_list_delete( tmp, tmp->content.offset );
        }
        while( tmp != NULL );
    }

    /* Verify an ECDSA-SHA256 signature. */
    if( CRYPTO_SignatureVerificationStart( &pvSigVerifyContext, cryptoASYMMETRIC_ALGORITHM_ECDSA,
                                            cryptoHASH_ALGORITHM_SHA256 ) == pdFALSE )
    {
        return OTA_PAL_COMBINE_ERR( OtaPalSignatureCheckFailed, 0 );
    }
    else
    {
        LogInfo( ( "Started %s signature verification, file: %s",
                    OTA_JsonFileSignatureKey, ( const char * ) pFileContext->pCertFilepath ) );
        pucSignerCert = R_FWUP_ReadAndAssumeCertificate( pFileContext->pCertFilepath, &ulSignerCertSize );

        if( pucSignerCert == NULL )
        {
            eResult = OtaPalBadSignerCert;
        }
        else
        {
            CRYPTO_SignatureVerificationUpdate( pvSigVerifyContext,
                                                ( const uint8_t * ) BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS +
                                                BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH,
                                                s_load_fw_control_block.total_image_length );

            if( CRYPTO_SignatureVerificationFinal( pvSigVerifyContext, ( char * ) pucSignerCert, ulSignerCertSize,
                                                    pFileContext->pSignature->data, pFileContext->pSignature->size ) == pdFALSE )
            {
                LogError( ( "Finished %s signature verification, but signature verification failed",
                            OTA_JsonFileSignatureKey ) );
                eResult = OtaPalSignatureCheckFailed;
            }
            else
            {
                LogInfo( ( "Finished %s signature verification, signature verification passed",
                            OTA_JsonFileSignatureKey ) );
                eResult = OtaPalSuccess;
            }
        }
    }

    /* Free the signer certificate that we now own after R_FWUP_ReadAndAssumeCertificate(). */
    if( pucSignerCert != NULL )
    {
        vPortFree( pucSignerCert );
    }
#else /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS */
    /* Fix me for other OS environment */
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS */
    return OTA_PAL_COMBINE_ERR( eResult, 0 );
}
/*-----------------------------------------------------------*/
#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS)
uint8_t * R_FWUP_ReadAndAssumeCertificate( const uint8_t * const pucCertName,
                                                    uint32_t * const ulSignerCertSize )
{
    uint8_t * pucCertData;
    uint32_t ulCertSize;
    uint8_t * pucSignerCert = NULL;
    CK_RV xResult;

    xResult = prvGetCertificate( ( const char * ) pucCertName, &pucSignerCert, ulSignerCertSize );

    if( ( xResult == CKR_OK ) && ( pucSignerCert != NULL ) )
    {
        LogInfo( ( "R_FWUP_ReadAndAssumeCertificate using cert with label: %s", ( const char * ) pucCertName ) );
    }
    else
    {
        LogWarn( ( "No such certificate file: %s. Using certificate defined by the otapalconfigCODE_SIGNING_CERTIFICATE macro instead",
                   ( const char * ) pucCertName ) );

        /* Allocate memory for the signer certificate plus a terminating
           zero so we can copy it and return to the caller. */
        ulCertSize = sizeof( otapalconfigCODE_SIGNING_CERTIFICATE );
        
        /*lint !e9029 !e9079 !e838 malloc proto requires void*. */
        pucSignerCert = pvPortMalloc( ulCertSize + 1 );
        
        /*lint !e9005 we don't modify the cert but it could be set by PKCS11 so it's not const. */
        pucCertData = ( uint8_t * ) otapalconfigCODE_SIGNING_CERTIFICATE;

        if( pucSignerCert != NULL )
        {
            memcpy( pucSignerCert, pucCertData, ulCertSize );
            /* The crypto code requires the terminating zero to be part of the length so add 1 to the size. */
            pucSignerCert[ ulCertSize ] = 0U;
            *ulSignerCertSize = ulCertSize + 1U;
        }
        else
        {
            LogError( ( "Failed to store the certificate in dynamic memory: pvPortMalloc failed to allocate a buffer of size %d", ulCertSize ) );
        }
    }

    return pucSignerCert;
}
#else /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS */
    /* Fix me for other OS environment */
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS */

/*-----------------------------------------------------------*/

OtaPalStatus_t R_FWUP_ResetDevice( OtaFileContext_t * const pFileContext )
{
    ( void ) pFileContext;

    LogInfo( ( "Resetting the device." ) );
#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS)
    if ((OtaImageState_EOL == s_load_fw_control_block.eSavedAgentState) && (FLASH_CFG_DATA_FLASH_BGO == 1))
    {
        /* If the status is rejected, aborted, or error, swap bank and return to the previous image.
           Then the boot loader will start and erase the image that failed to update. */
        LogInfo(("Swap bank..."));
        R_BSP_SET_PSW(0);
        fwup_interrupts_disable();
        fwup_flash_close();
        fwup_flash_open();
        fwup_flash_set_bank_toggle();
        fwup_register_protect_disable();
        fwup_software_delay_ms(5000);
        R_BSP_SoftwareReset();
        while (1)
        {
            ;   /* software reset */
        }
    }
    else
    {
        /* Software reset issued (Not swap bank) */
        R_BSP_SET_PSW(0);
        fwup_interrupts_disable();
        fwup_register_protect_disable();
        fwup_software_delay_ms(5000);
        R_BSP_SoftwareReset();
        while (1)
        {
            ;    /* software reset */
        }
    }
#else  /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS */
    if( ( OtaImageStateAccepted == s_load_fw_control_block.eSavedAgentState ) ||
        ( OtaImageStateTesting == s_load_fw_control_block.eSavedAgentState ) )
    {
        /* Software reset issued (Not swap bank) */
        R_BSP_SET_PSW(0);
        fwup_interrupts_disable();
        fwup_register_protect_disable();
        R_BSP_SoftwareReset();

        while( 1 ) /* software reset */
        {
        }
    }
    else
    {
        /* If the status is rejected, aborted, or error, swap bank and return to the previous image.
         * Then the boot loader will start and erase the image that failed to update. */
        R_BSP_SET_PSW(0);
        fwup_interrupts_disable();
        fwup_flash_set_bank_toggle();
        fwup_register_protect_disable();
        R_BSP_SoftwareReset();

        while( 1 ) /* software reset */
        {
        }
    }

#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS */

    /* We shouldn't actually get here if the board supports the auto reset.
     * But, it doesn't hurt anything if we do although someone will need to
     * reset the device for the new image to boot. */
    return OTA_PAL_COMBINE_ERR( OtaPalSuccess, 0 );
}
/*-----------------------------------------------------------*/

OtaPalStatus_t R_FWUP_ActivateNewImage( OtaFileContext_t * const pFileContext )
{
    LogInfo( ( "Changing the Startup Bank" ) );

    /* reset for self testing */
#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS)
    fwup_software_delay_ms(5000);
#elif (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS)
    vTaskDelay( 5000 );
#else
    /* Fix me for other OS environment */
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS */
    R_FWUP_ResetDevice( pFileContext );    /* no return from this function */

    return OTA_PAL_COMBINE_ERR( OtaPalSuccess, 0 );
}
/*-----------------------------------------------------------*/

OtaPalStatus_t R_FWUP_SetPlatformImageState( OtaFileContext_t * const pFileContext,
                                             OtaImageState_t eState )
{
    flash_interrupt_config_t cb_func_info;

    LogDebug( ( "R_FWUP_SetPlatformImageState is called." ) );

    OtaPalMainStatus_t eResult = OtaPalUninitialized;

    /* Save the image state to eSavedAgentState. */
    if (OtaImageStateTesting == s_load_fw_control_block.eSavedAgentState)
    {
        switch ( eState )
        {
            case OtaImageStateAccepted:
                fwup_flash_close();
                fwup_flash_open();
#if (FLASH_CFG_CODE_FLASH_BGO == 1)
                cb_func_info.pcallback    = ota_header_flashing_callback;
                cb_func_info.int_priority = FLASH_INTERRUPT_PRIORITY;
                fwup_flash_set_callback((void *)&cb_func_info);
                gs_header_flashing_task = OTA_FLASHING_IN_PROGRESS;
#endif /* FLASH_CFG_CODE_FLASH_BGO == 1 */
                if (fwup_flash_erase((flash_block_address_t)BOOT_LOADER_UPDATE_TEMPORARY_AREA_ERASE_ADDRESS,
                                    BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER) == FLASH_SUCCESS)
                {
#if (FLASH_CFG_CODE_FLASH_BGO == 1)
                    while (OTA_FLASHING_IN_PROGRESS == gs_header_flashing_task)
                    {
                    }
#endif /* FLASH_CFG_CODE_FLASH_BGO == 1 */
                    LogInfo( ( "Erase install area (code flash):OK" ) );
                    LogInfo( ( "Accepted and committed final image." ) );
                    eResult = OtaPalSuccess;
                }
                else
                {
                    LogError( ( "Erase install area (code flash):NG" ) );
                    LogError( ( "Accepted final image but commit failed." ) );
                    eResult = OtaPalCommitFailed;
                }
                break;

            case OtaImageStateRejected:
                LogInfo( ( "Rejected image." ) );
                eResult = OtaPalSuccess;
                break;

            case OtaImageStateAborted:
                LogInfo( ( "Aborted image." ) );
                eResult = OtaPalSuccess;
                break;

            case OtaImageStateTesting:
                LogInfo( ( "Testing." ) );
                eResult = OtaPalSuccess;
                break;

            default:
                LogError( ( "Unknown state received %d.", ( int32_t ) eState ) );
                eResult = OtaPalBadImageState;
                break;
        }
    }
    else
    {
        switch ( eState )
        {
        case OtaImageStateAccepted:
            LogError( ( "Not in commit pending so can not mark image valid (%d).", ( int32_t ) eState ) );
            eResult = OtaPalCommitFailed;
            break;

        case OtaImageStateRejected:
            LogInfo( ( "Rejected image." ) );
            eResult = OtaPalSuccess;
            break;

        case OtaImageStateAborted:
            LogInfo( ( "Aborted image." ) );
            eResult = OtaPalSuccess;
            break;

        case OtaImageStateTesting:
            LogInfo( ( "Testing." ) );
            eResult = OtaPalSuccess;
            break;

        default:
            LogError( ( "Unknown state received %d.", ( int32_t ) eState ) );
            eResult = OtaPalBadImageState;
            break;
        }
    }

    s_load_fw_control_block.eSavedAgentState = eState;

    return OTA_PAL_COMBINE_ERR( eResult, 0 );
}
/*-----------------------------------------------------------*/

OtaPalImageState_t R_FWUP_GetPlatformImageState( OtaFileContext_t * const pFileContext )
{
    ( void ) pFileContext;

    LogDebug( ( "R_FWUP_GetPlatformImageState is called." ) );

    OtaPalImageState_t ePalState = OtaPalImageStateUnknown;

    switch ( s_load_fw_control_block.eSavedAgentState )
    {
    case OtaImageStateTesting:
        ePalState = OtaPalImageStatePendingCommit;
        break;

    case OtaImageStateAccepted:
        ePalState = OtaPalImageStateValid;
        break;

    case OtaImageStateUnknown: /* Uninitialized image state, assume a factory image */
        ePalState = OtaPalImageStateValid;
        break;

    case OtaImageStateRejected:
    case OtaImageStateAborted:
    default:
        ePalState = OtaPalImageStateInvalid;
        break;
    }

    LogDebug( ( "Function called is R_FWUP_GetPlatformImageState: Platform State is [%d]", ePalState ) );
    return ePalState; /*lint !e64 !e480 !e481 I/O calls and return type are used per design. */
}
/*-----------------------------------------------------------*/

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS)
/* Provide access to private members for testing. */

static CK_RV prvGetCertificateHandle( CK_FUNCTION_LIST_PTR pxFunctionList,
                                        CK_SESSION_HANDLE xSession,
                                        const char * pcLabelName,
                                        CK_OBJECT_HANDLE_PTR pxCertHandle )
{
    CK_ATTRIBUTE xTemplate;
    CK_RV xResult = CKR_OK;
    CK_ULONG ulCount = 0;
    CK_BBOOL xFindInit = CK_FALSE;

    /* Get the certificate handle. */
    if( 0 == xResult )
    {
        xTemplate.type = CKA_LABEL;
        xTemplate.ulValueLen = strlen( pcLabelName ) + 1;
        xTemplate.pValue = ( char * ) pcLabelName;
        xResult = pxFunctionList->C_FindObjectsInit( xSession, &xTemplate, 1 );
    }

    if( 0 == xResult )
    {
        xFindInit = CK_TRUE;
        xResult = pxFunctionList->C_FindObjects( xSession,
                                                ( CK_OBJECT_HANDLE_PTR ) pxCertHandle,
                                                1,
                                                &ulCount );
    }

    if( CK_TRUE == xFindInit )
    {
        xResult = pxFunctionList->C_FindObjectsFinal( xSession );
    }

    return xResult;
}

/* Note that this function mallocs a buffer for the certificate to reside in,
 * and it is the responsibility of the caller to free the buffer. */
static CK_RV prvGetCertificate( const char * pcLabelName,
                                uint8_t ** ppucData,
                                uint32_t * pulDataSize )
{
    /* Find the certificate */
    CK_OBJECT_HANDLE xHandle;
    CK_RV xResult;
    CK_FUNCTION_LIST_PTR xFunctionList;
    CK_SLOT_ID xSlotId;
    CK_ULONG xCount = 1;
    CK_SESSION_HANDLE xSession;
    CK_ATTRIBUTE xTemplate = { 0 };
    uint8_t * pucCert = NULL;
    CK_BBOOL xSessionOpen = CK_FALSE;

    xResult = C_GetFunctionList( &xFunctionList );

    if( CKR_OK == xResult )
    {
        xResult = xFunctionList->C_Initialize( NULL );
    }

    if( ( CKR_OK == xResult ) || ( CKR_CRYPTOKI_ALREADY_INITIALIZED == xResult ) )
    {
        xResult = xFunctionList->C_GetSlotList( CK_TRUE, &xSlotId, &xCount );
    }

    if( CKR_OK == xResult )
    {
        xResult = xFunctionList->C_OpenSession( xSlotId, CKF_SERIAL_SESSION, NULL, NULL, &xSession );
    }

    if( CKR_OK == xResult )
    {
        xSessionOpen = CK_TRUE;
        xResult = prvGetCertificateHandle( xFunctionList, xSession, pcLabelName, &xHandle );
    }

    if( ( xHandle != 0 ) && ( xResult == CKR_OK ) ) /* 0 is an invalid handle */
    {
        /* Get the length of the certificate */
        xTemplate.type = CKA_VALUE;
        xTemplate.pValue = NULL;
        xResult = xFunctionList->C_GetAttributeValue( xSession, xHandle, &xTemplate, xCount );

        if( xResult == CKR_OK )
        {
            pucCert = pvPortMalloc( xTemplate.ulValueLen );
        }

        if( ( xResult == CKR_OK ) && ( pucCert == NULL ) )
        {
            xResult = CKR_HOST_MEMORY;
        }

        if( xResult == CKR_OK )
        {
            xTemplate.pValue = pucCert;
            xResult = xFunctionList->C_GetAttributeValue( xSession, xHandle, &xTemplate, xCount );

            if( xResult == CKR_OK )
            {
                *ppucData = pucCert;
                *pulDataSize = xTemplate.ulValueLen;
            }
            else
            {
                vPortFree( pucCert );
            }
        }
    }
    else /* Certificate was not found. */
    {
        *ppucData = NULL;
        *pulDataSize = 0;
    }

    if( xSessionOpen == CK_TRUE )
    {
        ( void ) xFunctionList->C_CloseSession( xSession );
    }

    return xResult;
}
#else /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS */
    /* Fix me for other OS environment */
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS */

static int32_t ota_context_validate( OtaFileContext_t * pFileContext )
{
    return( NULL != pFileContext );
}

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS)
static int32_t ota_context_update_user_firmware_header( OtaFileContext_t * pFileContext )
{
    int32_t ret = R_OTA_ERR_INVALID_CONTEXT;
    uint8_t block[BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH];
    st_fwup_control_block_t * p_block_header;
    uint32_t length;
    flash_err_t flash_err;
    flash_interrupt_config_t cb_func_info;
    uint8_t *source_pointer, *destination_pointer;
    uint8_t data_length = 0;

    memcpy(block, (const void *)firmware_update_control_block_bank0, BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH);
    p_block_header = (st_fwup_control_block_t *)block;

    /* Update image flag. */
    p_block_header->image_flag = LIFECYCLE_STATE_TESTING;

    /* Update signature type. */
    memcpy( p_block_header->signature_type, OTA_JsonFileSignatureKey, sizeof( OTA_JsonFileSignatureKey ) );

    /* Parse the signature and extract ECDSA-SHA256 signature data. */
    source_pointer = pFileContext->pSignature->data;
    destination_pointer = p_block_header->signature;
    data_length = *( source_pointer + 1 ) + OTA_SIGUNATURE_SEQUENCE_INFO_LENGTH;
    memset( destination_pointer, 0, sizeof( destination_pointer ) );

    if( OTA_SIGUNATURE_SEQUENCE_TOP_VALUE == *source_pointer )
    {
        source_pointer += OTA_SIGUNATURE_SEQUENCE_INFO_LENGTH;

        while( 1 )
        {
            if( OTA_SIGUNATURE_INTEGER_VALUE == *source_pointer )
            {
                source_pointer++;

                if( OTA_SIGUNATURE_INCLUDE_NEGATIVE_NUMBER_VALUE == *source_pointer )
                {
                    source_pointer += OTA_SIGUNATURE_SKIP;
                }
                else
                {
                    source_pointer++;
                }

                memcpy( destination_pointer, source_pointer, OTA_SIGUNATURE_DATA_HALF_LENGTH );
                source_pointer += OTA_SIGUNATURE_DATA_HALF_LENGTH;
                destination_pointer += OTA_SIGUNATURE_DATA_HALF_LENGTH;

                if( ( source_pointer - pFileContext->pSignature->data ) == data_length )
                {
                    ret = R_OTA_ERR_NONE;
                    break;
                }
            }
            else
            {
                /* parsing error */
                break;
            }
        }
    }

    if (R_OTA_ERR_NONE == ret)
    {
        fwup_flash_close();
        fwup_flash_open();
        cb_func_info.pcallback = ota_header_flashing_callback;
        cb_func_info.int_priority = FLASH_INTERRUPT_PRIORITY;
        fwup_flash_set_callback((void *)&cb_func_info);
        gs_header_flashing_task = OTA_FLASHING_IN_PROGRESS;
        length = BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH;
        flash_err = fwup_flash_write( ( uint32_t ) block, ( uint32_t ) BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS, length);

        if(flash_err != FLASH_SUCCESS)
        {
            R_BSP_NOP();  /* When an error occurs, consider an error notification method according to the system. */
        }
        while( OTA_FLASHING_IN_PROGRESS == gs_header_flashing_task )
        {
        }
    }

    return ret;
}
#else /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS */
    /* Fix me for other OS environment */
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS */

static void ota_context_close( OtaFileContext_t * pFileContext )
{
    if( NULL != pFileContext )
    {
        pFileContext->pFile = NULL;
    }
}

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS)
static st_fragmented_block_list_t * fragmented_flash_block_list_insert( st_fragmented_block_list_t * head,
                                                                         uint32_t offset,
                                                                         uint8_t * binary,
                                                                         uint32_t length )
{
    st_fragmented_block_list_t *tmp, *current, *previous, *new;

    new = pvPortMalloc( sizeof( st_flash_block_t ) );
    new->content.p_binary = pvPortMalloc( length );

    if( ( new != NULL ) && ( new->content.p_binary != NULL ) )
    {
        memcpy( new->content.p_binary, binary, length );
        new->content.offset = offset;
        new->content.length = length;
        new->next = NULL;

        /* new head would be returned when head would be specified as NULL. */
        if( head == NULL )
        {
            tmp = new;
        }
        else
        {
            /* search the list to insert new node */
            current = head;

            while( 1 )
            {
                if( ( new->content.offset < current->content.offset ) || ( current == NULL ) )
                {
                    break;
                }

                previous = current;
                current = current->next;
            }

            /* insert the searched node when current != head */
            if( current != head )
            {
                previous->next = new;
                new->next = current;
                tmp = head;
            }
            else
            {
                new->next = current;
                tmp = new;
            }
        }
    }
    else
    {
        tmp = NULL;
    }
    return tmp;
}

static st_fragmented_block_list_t * fragmented_flash_block_list_delete( st_fragmented_block_list_t * head,
                                                                         uint32_t offset )
{
    st_fragmented_block_list_t *tmp = head, *previous = NULL;

    if( head != NULL )
    {
        while( 1 )
        {
            if( tmp->content.offset == offset )
            {
                break;
            }

            if( tmp->next == NULL )
            {
                tmp = NULL;
                break;
            }

            previous = tmp;
            tmp = tmp->next;
        }

        if( tmp != NULL )
        {
            /* delete target exists on not head, remove specified and return head in this case */
            if( previous != NULL )
            {
                previous->next = tmp->next;
                vPortFree( tmp->content.p_binary );
                vPortFree( tmp );
                tmp = head;
            }
            else
            {
                /* delete target exists on head with subsequent data,
                   remove head and return specified (as new head) in this case */
                if( head->next != NULL )
                {
                    tmp = head->next;
                }
                /* delete target exists on head without subsequent data,
                   remove head and return NULL in this case */
                else
                {
                    tmp = NULL;
                }

                vPortFree( head->content.p_binary );
                vPortFree( head );
            }
        }
    }

    return tmp;
}

static st_fragmented_block_list_t *fragmented_flash_block_list_print(st_fragmented_block_list_t *head)
{
    st_fragmented_block_list_t *tmp = head;
    uint32_t total_heap_length, total_list_count;

    total_heap_length = 0;
    total_list_count = 0;

    if( head != NULL )
    {
        while( 1 )
        {
            total_heap_length += sizeof( st_fragmented_block_list_t );
            total_heap_length += tmp->content.length;
            total_list_count++;

            if( tmp->next == NULL )
            {
                break;
            }

            tmp = tmp->next;
        }
    }

    LogDebug( ( "FRAGMENTED_FLASH_BLOCK_LIST: total_heap_length = [%d], total_list_count = [%d].", total_heap_length, total_list_count ) );

    return tmp;
}

static st_fragmented_block_list_t * fragmented_flash_block_list_assemble(st_fragmented_block_list_t * head,
                                                                         st_flash_block_t * flash_block)
{
    st_fragmented_block_list_t *tmp = head;
    st_fragmented_block_list_t *flash_block_candidate[FLASH_CF_MIN_PGM_SIZE];
    uint32_t assembled_length = 0;
    uint32_t fragmented_length = 0;
    uint32_t loop_counter = 0;
    uint32_t index = 0;

    /* search aligned FLASH_CF_MIN_PGM_SIZE top offset */
    while( 1 )
    {
        if( !( tmp->content.offset % FLASH_CF_MIN_PGM_SIZE ) )
        {
            /* extract continuous flash_block candidate */
            assembled_length = 0;
            loop_counter = 0;
            while( 1 )
            {
                if ( ( tmp != NULL ) && ( assembled_length < FLASH_CF_MIN_PGM_SIZE ) )
                {
                    if( loop_counter < FLASH_CF_MIN_PGM_SIZE )
                    {
                        if( ( tmp->content.offset + tmp->content.length ) >
                            ( ( tmp->content.offset & OTA_FLASH_MIN_PGM_SIZE_MASK) + FLASH_CF_MIN_PGM_SIZE ) )
                        {
                            fragmented_length = ( FLASH_CF_MIN_PGM_SIZE - assembled_length );
                            assembled_length += fragmented_length;
                            flash_block_candidate[loop_counter] = tmp;
                            tmp->next = fragmented_flash_block_list_insert( tmp->next, tmp->content.offset +
                                            fragmented_length, &tmp->content.p_binary[fragmented_length],
                                            tmp->content.length - fragmented_length );
                            tmp->content.length = fragmented_length;
                        }
                        else
                        {
                            assembled_length += tmp->content.length;
                            flash_block_candidate[loop_counter] = tmp;
                        }
                    }
                    else
                    {
                        break;
                    }
                    tmp = tmp->next;
                    loop_counter++;
                }
                else
                {
                    break;
                }
            }
        }

        /* break if found completed flash_block_candidate or found end of list */
        if( ( assembled_length == FLASH_CF_MIN_PGM_SIZE ) || ( tmp == NULL ) )
        {
            break;
        }
        /* search next candidate */
        else
        {
            tmp = tmp->next;
        }
    }

    /* assemble flash_block */
    if( assembled_length == FLASH_CF_MIN_PGM_SIZE )
    {
        tmp = head;
        /* remove flash_block from list */
        flash_block->offset = flash_block_candidate[ 0 ]->content.offset;
        flash_block->length = assembled_length;

        for( uint32_t i = 0; i < loop_counter; i++ )
        {
            memcpy( &flash_block->p_binary[ index ], flash_block_candidate[ i ]->content.p_binary,
                flash_block_candidate[ i ]->content.length );
            index = flash_block_candidate[ i ]->content.length;
            tmp = fragmented_flash_block_list_delete( tmp, flash_block_candidate[ i ]->content.offset );
        }
    }
    else
    {
        tmp = head;
        flash_block->offset = NULL;
        flash_block->p_binary = NULL;
        flash_block->length = NULL;
    }
    return tmp;
}
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS */

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS)
static flash_err_t ota_flashing_task(uint8_t *p_packet, uint32_t ulOffset, uint32_t length)
{
    flash_err_t    flash_err;
    static uint8_t s_block[FWUP_WRITE_BLOCK_SIZE];

    memcpy(s_block, p_packet, length);
    flash_err = fwup_flash_write((uint32_t)s_block,
                                    (uint32_t)(ulOffset + (uint32_t)BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS),
                                    length);
    if (FWUP_WRITE_BLOCK_SIZE != length)
    {
        R_BSP_NOP();
    }
    if (FLASH_SUCCESS != flash_err)
    {
        R_BSP_NOP();  /* When an error occurs, consider an error notification method according to the system. */
    }
    s_load_fw_control_block.total_image_length += length;

    return flash_err;
}
#elif (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS)
static void ota_flashing_task( void * pvParameters )
{
    flash_err_t flash_err;
    static uint8_t block[ ( 1 << otaconfigLOG2_FILE_BLOCK_SIZE ) ];
    static uint32_t ulOffset;
    static uint32_t length;

    while( 1 )
    {
        xQueueReceive( xQueue, &packet_block_for_queue2, portMAX_DELAY );
        xSemaphoreTake( xSemaphoreFlashig, portMAX_DELAY );
        memcpy( block, packet_block_for_queue2.p_packet, packet_block_for_queue2.length );
        ulOffset = packet_block_for_queue2.ulOffset;
        length = packet_block_for_queue2.length;
        flash_err = fwup_flash_write( ( uint32_t ) block, ( ulOffset +
                                    ( uint32_t ) BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS +
                                    ( uint32_t ) BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH ), length );
        if( packet_block_for_queue2.length != 1024 )
        {
            R_BSP_NOP();
        }
        if( flash_err != FLASH_SUCCESS )
        {
            R_BSP_NOP();  /* When an error occurs, consider an error notification method according to the system. */
        }

        s_load_fw_control_block.total_image_length += length;
        vPortFree(packet_block_for_queue2.p_packet);
    }
}
#else
    /* Fix me for other OS environment */
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS */

static void ota_flashing_callback( void *event )
{
#if (FLASH_CFG_CODE_FLASH_BGO == 1)
    flash_int_cb_args_t * p_event = event;

    if ( ( FLASH_INT_EVENT_WRITE_COMPLETE != p_event->event ) ||
        ( FLASH_INT_EVENT_ERASE_COMPLETE == p_event->event ) )
    {
        R_BSP_NOP(); /* trap */
    }
    if ((FLASH_INT_EVENT_WRITE_COMPLETE == p_event->event) &&
        (fwup_get_status()              == FWUP_STATE_FLASH_WRITE_WAIT))
    {
        fwup_update_status(FWUP_STATE_FLASH_WRITE_COMPLETE);  /* Update the firmware update status */
    }
#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS)
    static portBASE_TYPE xHigherPriorityTaskWoken;
    xSemaphoreGiveFromISR( xSemaphoreFlashig, &xHigherPriorityTaskWoken );
#else
    /* Fix me for other OS environment */
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS */
#endif /* FLASH_CFG_CODE_FLASH_BGO */
}

static void ota_header_flashing_callback( void *event )
{
#if (FLASH_CFG_CODE_FLASH_BGO == 1)
    flash_int_cb_args_t * p_event = event;

    gs_header_flashing_task = OTA_FLASHING_COMPLETE;

    if ( ( FLASH_INT_EVENT_WRITE_COMPLETE != p_event->event ) ||
        ( FLASH_INT_EVENT_ERASE_COMPLETE == p_event->event ) )
    {
        R_BSP_NOP(); /* trap */
    }
#endif /* FLASH_CFG_CODE_FLASH_BGO */
}
#endif  /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT != FWUP_IMPLEMENTATION_BOOTLOADER */

/***********************************************************************************************************************
 * Function Name: fwup_flash_open
 *******************************************************************************************************************//**
 * @brief   The function initializes the Flash module.
 *          This function must be called before calling any other API functions.
 * @retval  FLASH_SUCCESS          Flash module initialized successfully.
 * @retval  FLASH_ERR_BUSY         A different flash process is being executed, try again later.
 * @retval  FLASH_ERR_ALREADY_OPEN Open() called twice without an intermediate Close().
 * @details This function initializes the Flash module.
 *          This is a function for abstracting the flash access.
 *          Note that this function must be called before any other API function.
 */
flash_err_t fwup_flash_open(void)
{
    flash_err_t ret = FLASH_SUCCESS;

#if (BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100)
    ret = R_FLASH_Open();
#else
    /* Fix me for other flash control library */
#endif /* BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100 */
    return ret;
}
/**********************************************************************************************************************
 End of function fwup_flash_open
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: fwup_flash_close
 *******************************************************************************************************************//**
 * @brief   The function closes the Flash module.
 * @retval  FLASH_SUCCESS  Flash module closed successfully.
 * @retval  FLASH_ERR_BUSY A different flash process is being executed, try again later.
 * @details This function closes the Flash module.
 *          It disables the flash interrupts (if enabled) and sets the driver to an uninitialized state.
 *          This is a function for abstracting the flash access.
 */
flash_err_t fwup_flash_close(void)
{
    flash_err_t ret = FLASH_SUCCESS;

#if (BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100)
    ret = R_FLASH_Close();
#else
    /* Fix me for other flash control library */
#endif /* BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100 */
    return ret;
}
/**********************************************************************************************************************
 End of function fwup_flash_close
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: fwup_flash_set_callback
 *******************************************************************************************************************//**
 * @brief         Register the callback function.
 * @param[in]     cb_func_info    Address of Callback function.
 * @retval        FLASH_SUCCESS         Operation successful.
 *                                      (in non-blocking mode, this means the operations was started successfully)
 * @retval        FLASH_ERR_ADDRESS     Address is an invalid Code/Data Flash block start address.
 * @retval        FLASH_ERR_NULL_PTR    pcfg was NULL for a command that expects a configuration structure.
 * @retval        FLASH_ERR_BUSY        A different flash process is being executed or the module is not initialized.
 * @retval        FLASH_ERR_CMD_LOCKED  The flash control circuit was in a command locked state and was reset.
 * @retval        FLASH_ERR_ACCESSW     Access window error: Incorrect area specified.
 * @retval        FLASH_ERR_PARAM       Invalid command.
 * @details       Register the callback function.
 *                This is a function for abstracting the flash access.
 */
flash_err_t fwup_flash_set_callback(flash_interrupt_config_t *cb_func_info)
{
    flash_err_t ret = FLASH_SUCCESS;

#if (FLASH_CFG_CODE_FLASH_BGO == 1)
    ret = R_FLASH_Control(FLASH_CMD_SET_BGO_CALLBACK, (void *)cb_func_info);
#else
    /* Fix me for other flash control library */
#endif  /* FLASH_CFG_CODE_FLASH_BGO */
    return ret;
}
/**********************************************************************************************************************
 End of function fwup_flash_set_callback
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: fwup_flash_get_bank_info
 *******************************************************************************************************************//**
 * @brief         Read current BANKSEL value.
 * @param[out]    bank_info             Current bank information.
 * @retval        FLASH_SUCCESS         Operation successful.
 *                                      (in non-blocking mode, this means the operations was started successfully)
 * @retval        FLASH_ERR_ADDRESS     Address is an invalid Code/Data Flash block start address.
 * @retval        FLASH_ERR_NULL_PTR    pcfg was NULL for a command that expects a configuration structure.
 * @retval        FLASH_ERR_BUSY        A different flash process is being executed or the module is not initialized.
 * @retval        FLASH_ERR_CMD_LOCKED  The flash control circuit was in a command locked state and was reset.
 * @retval        FLASH_ERR_ACCESSW     Access window error: Incorrect area specified.
 * @retval        FLASH_ERR_PARAM       Invalid command.
 * @details       Read current BANKSEL value.
 *                This is a function for abstracting the flash access.
 */
#if (FWUP_FLASH_BANK_MODE == 0)  /* Dual bank mode */
flash_err_t fwup_flash_get_bank_info(flash_bank_t *bank_info)
{
    flash_err_t ret = FLASH_SUCCESS;

#if (BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600)
    ret = R_FLASH_Control(FLASH_CMD_BANK_GET, bank_info);
#else
    /* Fix me for other flash control library */
#endif /* BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 */
    return ret;
}
/**********************************************************************************************************************
 End of function fwup_flash_get_bank_info
 *********************************************************************************************************************/
#endif /* FWUP_FLASH_BANK_MODE == 0 */

/***********************************************************************************************************************
 * Function Name: fwup_flash_set_bank_toggle
 *******************************************************************************************************************//**
 * @brief         Switch boot bank.
 * @details       Switch boot bank.
 *                This is a function for abstracting the flash access.
 */
void fwup_flash_set_bank_toggle(void)
{
#if (FWUP_FLASH_BANK_MODE == 0)  /* Dual bank mode */
    (void)R_FLASH_Control(FLASH_CMD_BANK_TOGGLE, NULL);
#else  /* Linear mode */
    R_BSP_NOP();
#endif /* FWUP_FLASH_BANK_MODE */
}
/**********************************************************************************************************************
 End of function fwup_flash_set_bank_toggle
 *********************************************************************************************************************/

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_BOOTLOADER)
#if (FWUP_CFG_BOOT_PROTECT_ENABLE == 1)
#if (FLASH_TYPE == FLASH_TYPE_4 || FLASH_TYPE == FLASH_TYPE_1)
/***********************************************************************************************************************
 * Function Name: fwup_flash_accesswindow_set
 *******************************************************************************************************************//**
 * @brief         Switch boot bank.
 * @details       Switch boot bank.
 *                This is a function for abstracting the flash access.
 */
static flash_err_t fwup_flash_accesswindow_set(flash_access_window_config_t * addr)
{
    flash_err_t ret;

    /* [Note] Set FSPR bit in R_FLASH_Control(FLASH_CMD_ACCESSWINDOW_SET) function. */
    R_BSP_InterruptsDisable();
    ret = R_FLASH_Control(FLASH_CMD_ACCESSWINDOW_SET, addr);
    R_BSP_InterruptsEnable();
    return ret;
}
#endif /* FLASH_TYPE */
#endif /* FWUP_CFG_BOOT_PROTECT_ENABLE */
#endif  /* FWUP_IMPLEMENTATION_BOOTLOADER */

/***********************************************************************************************************************
 * Function Name: fwup_flash_erase
 *******************************************************************************************************************//**
 * @brief     This function is used to erase the specified block in code flash or data flash.
 * @param[in] block_start_address Specifies the start address of block to erase.
 *                                The enum flash_block_address_t is defined in the corresponding MCU's
 *                                r_flash_rx\src\targets\mcu\r_flash_mcu.h file.
 *                                The blocks are labeled in the same fashion as they are
 *                                in the device's Hardware Manual.
 *                                For example, the block located at address 0xFFFFC000 is called Block 7
 *                                in the RX113 hardware manual, therefore "FLASH_CF_BLOCK_7" should be passed
 *                                for this parameter.
 *                                Similarly, to erase Data Flash Block 0 which is located at address 0x00100000,
 *                                this argument should be FLASH_DF_BLOCK_0.
 * @param[in] num_blocks          Specifies the number of blocks to be erased.
 *                                For type 1 parts, address + num_blocks cannot cross a 256K boundary.
 * @retval    FLASH_SUCCESS       Operation successful.
 *                                (if non-blocking mode is enabled, this means the operation was started successfully).
 * @retval    FLASH_ERR_BLOCKS    Invalid number of blocks specified.
 * @retval    FLASH_ERR_ADDRESS   Invalid address specified.
 * @retval    FLASH_ERR_BUSY      A different flash process is being executed, or the module is not initialized.
 * @retval    FLASH_ERR_FAILURE   Erasing failure. Sequencer has been reset.
 *                                Or callback function not registered (in non-blocking mode)
 * @details   Erases a contiguous number of code flash or data flash memory blocks.
 *            The block size varies depending on MCU types.
 *            The equates FLASH_CF_BLOCK_SIZE for code flash and FLASH_DF_BLOCK_SIZE
 *            for data flash are provided for these values.
 *            The enum flash_block_address_t is configured at compile time based on the memory configuration of
 *            the MCU device specified in the r_bsp module.
 *            This is a function for abstracting the flash access.
 */
flash_err_t fwup_flash_erase(flash_block_address_t block_start_address, uint32_t num_blocks)
{
    flash_err_t ret;

#if (FLASH_CFG_CODE_FLASH_BGO == 1)
    ret = R_FLASH_Erase(block_start_address, num_blocks);
#elif (FLASH_CFG_CODE_FLASH_BGO == 0)
#if ((defined(MCU_RX111) || defined(MCU_RX113) || defined(MCU_RX130)) && (MCU_ROM_SIZE_BYTES > 0x40000L)) /* for memory plain */
    /* The RX111, RX113, and RX130 code flash products with more than 256KB have two memory plains. */
    /* Code flashes that cross memory plain boundaries cannot be erased. */
    uint32_t i;

    if ((block_start_address <= FLASH_CF_BLOCK_END) &&
            (block_start_address >= FLASH_CF_LOWEST_VALID_BLOCK))    /* Check code flash address. */
    {
        for (i = 0; i < num_blocks; i++)
        {
            R_BSP_InterruptsDisable();
            ret = R_FLASH_Erase((flash_block_address_t)(block_start_address + (i * FLASH_CF_BLOCK_SIZE)), 1);
            R_BSP_InterruptsEnable();
            if (ret != FLASH_SUCCESS)
            {
                break;
            }
        }
    }
    else
    {
        R_BSP_InterruptsDisable();
        ret = R_FLASH_Erase(block_start_address, num_blocks);
        R_BSP_InterruptsEnable();
    }
#else  /* Case of without memory plains. */
    R_BSP_InterruptsDisable();
    ret = R_FLASH_Erase(block_start_address, num_blocks);
    R_BSP_InterruptsEnable();
#endif /* memory plain */
#else /* FLASH_CFG_CODE_FLASH_BGO == 0 */
    /* Fix me for other flash control library */
#endif /* FLASH_CFG_CODE_FLASH_BGO == 1 */
    return ret;
}
/**********************************************************************************************************************
 End of function fwup_flash_erase
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: fwup_flash_write
 *******************************************************************************************************************//**
 * @brief     This function is used to write data to code flash or data flash.
 * @param[in] src_address       This is the first address  of the buffer containing the data to write to Flash.
 * @param[in] dest_address      This is the first address  of the code flash or data flash area to rewrite data.
 *                              The address specified must be divisible by the minimum programming size.
 *                              See Description below for important restrictions regarding this parameter.
 * @param[in] num_bytes         The number of bytes contained in the buffer specified with src_address.
 *                              This number must be a multiple of the minimum programming size
 *                              for memory area you are writing to.
 * @retval    FLASH_SUCCESS     Operation successful.
 *                              (in non-blocking mode, this means the operation was started successfully)
 * @retval    FLASH_ERR_FAILURE Programming failed.
 *                              Possibly the destination address under access window or lockbit control;
 *                              or callback function not present(in non-blocking mode)
 * @retval    FLASH_ERR_BUSY    A different flash process is being executed or the module is not initialized.
 * @retval    FLASH_ERR_BYTES   Number of bytes provided was not a multiple of the minimum programming size
 *                              or exceed the maximum range.
 * @retval    FLASH_ERR_ADDRESS Invalid address was input or address not divisible by the minimum programming size.
 * @details   Writes data to flash memory. Before writing to any flash area, the area must already be erased.
 *            When performing a write the user must make sure to start the write on an address divisible
 *            by the minimum programming size and make the number of bytes to write be a multiple of the minimum
 *            programming size. The minimum programming size differs depending on what MCU package is being used and
 *            whether the code flash or data flash is being written to.
 *            This is a function for abstracting the flash access.
 */
flash_err_t fwup_flash_write(uint32_t src_address, uint32_t dest_address, uint32_t num_bytes)
{
    flash_err_t ret;

#if (FLASH_CFG_CODE_FLASH_BGO == 1 && FLASH_CFG_DATA_FLASH_BGO == 1)
    ret = R_FLASH_Write(src_address, dest_address, num_bytes);
#elif (FLASH_CFG_CODE_FLASH_BGO == 0 && FLASH_CFG_DATA_FLASH_BGO == 0)
#if (BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100)
    R_BSP_SoftwareDelay(FWUP_CFG_SCI_RECEIVE_WAIT, BSP_DELAY_MICROSECS);  /* SCI receive wait */
    R_BSP_InterruptsDisable();

    ret = R_FLASH_Write(src_address, dest_address, num_bytes);

    R_BSP_InterruptsEnable();
    FWUP_CFG_PORT_SYMBOL.PMR.BIT.FWUP_CFG_BIT_SYMBOL = 1;             /* Return port to UART flow control */
#endif /* BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100 */
#endif /* FLASH_CFG_CODE_FLASH_BGO */
    return ret;
}
/**********************************************************************************************************************
 End of function fwup_flash_write
 *********************************************************************************************************************/

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT != FWUP_IMPLEMENTATION_AFRTOS)
/***********************************************************************************************************************
 * Function Name: fwup_communication_open
 *******************************************************************************************************************//**
 * @brief   The function initializes the Communication module.
 *          This function must be called before calling any other API functions.
 * @retval  COMM_SUCCESS          Communication module initialized successfully.
 * @retval  COMM_ERROR            The communication module initialization has terminated Illegaly.
 * @details This function initializes the Communication module.
 *          This is a function for abstracting the Communication for firmware data download.
 *          Note that this function must be called before any other API function.
 */
e_comm_err_t fwup_communication_open(void)
{
    e_comm_err_t ret = COMM_SUCCESS;
#if (BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100)
#if (FWUP_CFG_COMMUNICATION_FUNCTION == FWUP_COMMUNICATION_SCI)  /* Case of SCI. */
    R_SCI_PinSet_serial_term();

    sci_cfg_t my_sci_config;
    sci_err_t my_sci_err;

    /* Set up the configuration data structure for asynchronous (UART) operation. */
    my_sci_config.async.baud_rate    = FWUP_CFG_SERIAL_TERM_SCI_BITRATE;
    my_sci_config.async.clk_src      = SCI_CLK_INT;
    my_sci_config.async.data_size    = SCI_DATA_8BIT;
    my_sci_config.async.parity_en    = SCI_PARITY_OFF;
    my_sci_config.async.parity_type  = SCI_EVEN_PARITY;
    my_sci_config.async.stop_bits    = SCI_STOPBITS_1;
    my_sci_config.async.int_priority = FWUP_CFG_SERIAL_TERM_SCI_INTERRUPT_PRIORITY;

    /* OPEN ASYNC CHANNEL
     *  Provide address of the configure structure,
     *  the callback function to be assigned,
     *  and the location for the handle to be stored.*/
#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_BOOTLOADER)
    my_sci_err = R_SCI_Open(SCI_CH_serial_term, SCI_MODE_ASYNC, &my_sci_config, my_sci_callback,
            &s_fwup_communication_handle);
#else
    my_sci_err = R_SCI_Open(SCI_CH_serial_term, SCI_MODE_ASYNC, &my_sci_config,
                            fwup_communication_callback, &s_fwup_communication_handle);
#endif /* FWUP_IMPLEMENTATION_BOOTLOADER */

    /* If there were an error this would demonstrate error detection of API calls. */
    if (SCI_SUCCESS != my_sci_err &&
        SCI_ERR_CH_NOT_CLOSED != my_sci_err)
    {
        R_BSP_NOP(); // Your error handling code would go here.
        ret = COMM_ERROR;
    }
#else /* FWUP_CFG_COMMUNICATION_FUNCTION == FWUP_COMMUNICATION_SCI */
    /* Fix me for other communication module */
#endif /* FWUP_CFG_COMMUNICATION_FUNCTION == FWUP_COMMUNICATION_SCI */
#else /* BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100 */
    /* Fix me for other MCU series */
#endif /* BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100 */
    return ret;
}
/**********************************************************************************************************************
 End of function fwup_communication_open
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: fwup_communication_close
 *******************************************************************************************************************//**
 * @brief   The function closes the Communication module.
 *          This function must be called before calling any other API functions.
 * @retval  COMM_SUCCESS          Communication module closed successfully.
 * @retval  COMM_ERROR            Closing the communication module has terminated Illegaly.
 * @details This function closes the Communication module.
 *          This is a function for abstracting the Communication for firmware data download.
 */
e_comm_err_t fwup_communication_close(void)
{
    e_comm_err_t ret = COMM_SUCCESS;

#if (BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100)
#if (FWUP_CFG_COMMUNICATION_FUNCTION == FWUP_COMMUNICATION_SCI)  /* Case of SCI. */
    sci_err_t my_sci_err;

    my_sci_err = R_SCI_Close(s_fwup_communication_handle);

    /* If there were an error this would demonstrate error detection of API calls. */
    if (SCI_SUCCESS != my_sci_err)
    {
        R_BSP_NOP(); // Your error handling code would go here.
        ret = COMM_ERROR;
    }
#else
    /* Fix me for other communication module */
#endif /* FWUP_CFG_COMMUNICATION_FUNCTION == FWUP_COMMUNICATION_SCI */
#else
    /* Fix me for other MCU series */
#endif /* BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100 */
return ret;
}
/**********************************************************************************************************************
 End of function fwup_communication_close
 *********************************************************************************************************************/

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS)

static void fwup_communication_callback(void *pArgs)
{
    sci_cb_args_t * p_args;

    p_args = (sci_cb_args_t *)pArgs;

#if (BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100)
#if (FWUP_CFG_COMMUNICATION_FUNCTION == FWUP_COMMUNICATION_SCI)  /* Case of SCI. */
    switch (p_args->event)
    {
    case SCI_EVT_RX_CHAR:

        /* From RXI interrupt; received character data is in p_args->byte */
        if ((s_sci_receive_control_block.p_sci_buffer_control->buffer_occupied_byte_size <
                sizeof(s_sci_receive_control_block.p_sci_buffer_control->buffer)) &&
            (FWUP_SCI_RECEIVE_BUFFER_EMPTY == s_sci_receive_control_block.p_sci_buffer_control->buffer_full_flag))
        {
            R_SCI_Receive(p_args->hdl, &s_sci_receive_control_block.p_sci_buffer_control->
                                        buffer[s_sci_receive_control_block.p_sci_buffer_control->
                                        buffer_occupied_byte_size++ ],
                                        1);
            if (s_sci_receive_control_block.p_sci_buffer_control->buffer_occupied_byte_size ==
                sizeof(s_sci_receive_control_block.p_sci_buffer_control->buffer))
            {
#if (FLASH_CFG_CODE_FLASH_BGO == 0)
                /* RTS HIGH */
                FWUP_CFG_PORT_SYMBOL.PODR.BIT.FWUP_CFG_BIT_SYMBOL = 1;  /* Set RTS to HIGH */
                FWUP_CFG_PORT_SYMBOL.PDR.BIT.FWUP_CFG_BIT_SYMBOL = 1;
                FWUP_CFG_PORT_SYMBOL.PMR.BIT.FWUP_CFG_BIT_SYMBOL = 0;   /* Change to general I/O port */
#endif /* FLASH_CFG_CODE_FLASH_BGO == 0 */
                s_sci_receive_control_block.total_byte_size +=
                    s_sci_receive_control_block.p_sci_buffer_control->buffer_occupied_byte_size;
                s_sci_receive_control_block.p_sci_buffer_control->buffer_occupied_byte_size = 0;
                s_sci_receive_control_block.p_sci_buffer_control->buffer_full_flag = FWUP_SCI_RECEIVE_BUFFER_FULL;
                if (FWUP_SCI_CONTROL_BLOCK_A == s_sci_receive_control_block.current_state)
                {
                    s_sci_receive_control_block.current_state        = FWUP_SCI_CONTROL_BLOCK_B;
                    s_sci_receive_control_block.p_sci_buffer_control = &s_sci_buffer_control[FWUP_SCI_CONTROL_BLOCK_B];
                }
                else
                {
                    s_sci_receive_control_block.current_state        = FWUP_SCI_CONTROL_BLOCK_A;
                    s_sci_receive_control_block.p_sci_buffer_control = &s_sci_buffer_control[FWUP_SCI_CONTROL_BLOCK_A];
                }
            }
        }
        break;
    case SCI_EVT_RXBUF_OVFL:

        /* From RXI interrupt; rx queue is full; 'lost' data is in p_args->byte
           You will need to increase buffer size or reduce baud rate */
        R_BSP_NOP();
        break;
    case SCI_EVT_OVFL_ERR:

        /* From receiver overflow error interrupt; error data is in p_args->byte
           Error condition is cleared in calling interrupt routine */
        R_BSP_NOP();
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
        R_BSP_NOP();  /* Do nothing */
        break;
    }
#else /* FWUP_CFG_COMMUNICATION_FUNCTION == FWUP_COMMUNICATION_SCI */
    /* Fix me for other communication module */
#endif /* FWUP_CFG_COMMUNICATION_FUNCTION == FWUP_COMMUNICATION_SCI */
#else /* BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100 */
    /* Fix me for other MCU series */
#endif /* BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100 */

} /* End of function fwup_communication_callback() */
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS */
#else /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT != FWUP_IMPLEMENTATION_AFRTOS */
    /* Fix me for other OS environment */
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT != FWUP_IMPLEMENTATION_AFRTOS */

/***********************************************************************************************************************
 * Function Name: fwup_update_status
 *******************************************************************************************************************//**
 * @brief   Update the Firmware update status.
 * @details Update the Firmware update status.
 *          Also update state transit flag.
 */
void fwup_update_status(e_fwup_state_t state)
{
    s_fwup_state                       = state;
    s_state_transit.state_transit_flag = true;
}
/**********************************************************************************************************************
 End of function fwup_update_status
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: fwup_get_status
 *******************************************************************************************************************//**
 * @brief   Return the current Firmware update status.
 * @brief   Return the current Firmware update status.
 */
e_fwup_state_t fwup_get_status(void)
{
    return s_fwup_state;
}
/**********************************************************************************************************************
 End of function fwup_get_status
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: fwup_state_monitoring_open
 *******************************************************************************************************************//**
 * @brief   Open the State transit monitoring module.
 * @retval  MONI_SUCCESS          State transit monitoring module initialized successfully.
 * @retval  MONI_ERROR            The State transit monitoring module initialization has terminated Illegaly.
 * @details Open the State transit monitoring module, and some initial settings.
 *          This is a function for abstracting the State transit monitoring module.
 */
e_state_monitoring_err_t fwup_state_monitoring_open(void)
{
    e_state_monitoring_err_t ret = MONI_SUCCESS;
#if (BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100)
    sys_time_err_t sys_time_api_error_code;

    sys_time_api_error_code = R_SYS_TIME_Open();
    if (SYS_TIME_SUCCESS != sys_time_api_error_code &&
        SYS_TIME_ERR_ALREADY_STARTED != sys_time_api_error_code)
    {
        ret = MONI_ERROR;
    }
#else
    /* Fix me for other State transit monitoring module */
#endif  /* BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100 */
    return ret;
}
/**********************************************************************************************************************
 End of function fwup_state_monitoring_open
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: fwup_state_monitoring_start
 *******************************************************************************************************************//**
 * @brief   Start status monitoring.
 * @retval  MONI_SUCCESS          State transit monitoring module started successfully.
 * @retval  MONI_ERROR            Start of the State transit monitoring module has terminated Illegaly.
 * @details Start the status monitoring by State transit monitoring module.
 *          This is a function for abstracting the State transit monitoring module.
 */
e_state_monitoring_err_t fwup_state_monitoring_start(void)
{
    e_state_monitoring_err_t ret = MONI_SUCCESS;
#if (BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100)
    sys_time_err_t sys_time_api_error_code;

    /* Set System-timer for check status */
    s_state_transit.check_status_counter     = 0;
    s_state_transit.state_transit_error_flag = STATE_MONITORING_IN_PROGRESS;
    s_state_transit.last_secure_boot_state   = s_fwup_state;
    s_state_transit.state_transit_flag       = true;

    sys_time_api_error_code = R_SYS_TIME_RegisterPeriodicCallback(fwup_state_monitoring_callback,
            MONITORING_STATUS_INTERVAL);
    if (SYS_TIME_SUCCESS != sys_time_api_error_code)
    {
        ret = MONI_ERROR;
    }
#else
    /* Fix me for other State transit monitoring module */
#endif  /* BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100 */
    return ret;
}
/**********************************************************************************************************************
 End of function fwup_state_monitoring_start
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: fwup_state_monitoring_stop
 *******************************************************************************************************************//**
 * @brief   Stop the callback process of State transit monitoring module.
 * @retval  MONI_SUCCESS          State transit monitoring module closed successfully.
 * @retval  MONI_ERROR            Closing the State transit monitoring module has terminated Illegally.
 * @details Stope the callback process of State transit monitoring module.
 *          This is a function for abstracting the System timer module.
 */
e_state_monitoring_err_t fwup_state_monitoring_stop(void)
{
    e_state_monitoring_err_t ret = MONI_SUCCESS;
#if (BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100)
    sys_time_err_t sys_time_api_error_code;

    if (true == R_SYS_TIME_IsPeriodicCallbackRegistered(fwup_state_monitoring_callback))
    {
        sys_time_api_error_code = R_SYS_TIME_UnregisterPeriodicCallback(fwup_state_monitoring_callback);
        if (SYS_TIME_SUCCESS != sys_time_api_error_code)
        {
            ret = MONI_ERROR;
        }
    }
#else
    /* Fix me for other State transit monitoring module */
#endif  /* BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100 */
    return ret;
}
/**********************************************************************************************************************
 End of function fwup_state_monitoring_stop
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: fwup_state_monitoring_close
 *******************************************************************************************************************//**
 * @brief   Close the all callback process of State transit monitoring module.
 * @retval  MONI_SUCCESS          State transit monitoring module closed successfully.
 * @retval  MONI_ERROR            Closing the State transit monitoring module has terminated Illegally.
 * @details Close the all callback process of State transit monitoring module.
 *          This is a function for abstracting the System timer module.
 */
e_state_monitoring_err_t fwup_state_monitoring_close(void)
{
    e_state_monitoring_err_t ret = MONI_SUCCESS;
#if (BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100)
    sys_time_err_t sys_time_api_error_code;

    sys_time_api_error_code = R_SYS_TIME_Close();
    if (SYS_TIME_SUCCESS != sys_time_api_error_code &&
        SYS_TIME_ERR_NOT_STARTED != sys_time_api_error_code)
    {
        ret = MONI_ERROR;
    }
#else
    /* Fix me for other State transit monitoring module */
#endif  /* BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100 */
    return ret;
}
/**********************************************************************************************************************
 End of function fwup_state_monitoring_close
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: fwup_state_monitoring_is_error
 *******************************************************************************************************************//**
 * @brief   Start status monitoring.
 * @retval  MONI_SUCCESS          State transit monitoring module started successfully.
 * @retval  MONI_ERROR            Start of the State transit monitoring module has terminated Illegaly.
 * @details Start the status monitoring by State transit monitoring module.
 *          This is a function for abstracting the State transit monitoring module.
 */
e_state_monitoring_flag_t fwup_state_monitoring_is_error(void)
{
    e_state_monitoring_flag_t ret = STATE_MONITORING_IN_PROGRESS;

#if (BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100)
    if (true == R_SYS_TIME_IsPeriodicCallbackRegistered(fwup_state_monitoring_callback))
    {
        ret = s_state_transit.state_transit_error_flag;
    }
#else
    /* Fix me for other State transit monitoring module */
#endif  /* BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100 */
    return ret;
}
/**********************************************************************************************************************
 End of function fwup_state_monitoring_is_error
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: fwup_state_monitoring_callback
 *******************************************************************************************************************//**
 * @brief   Callback function of State transit monitoring module.
 * @details Callback function of State transit monitoring module..
 */
static void fwup_state_monitoring_callback(void)
{
#if (BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100)
    if ((s_fwup_state      == s_state_transit.last_secure_boot_state) &&
        (false             == s_state_transit.state_transit_flag) &&
        (fwup_get_status() != FWUP_STATE_FINALIZE))
    {

        /* Status NOT changed */
        s_state_transit.check_status_counter++;
        if (s_state_transit.check_status_counter >= MONITORING_STATUS_COUNT)
        {
            s_state_transit.state_transit_error_flag = STATE_MONITORING_ERROR;
        }
    }
    else
    {

        /* Status changed, or Status is same but changed */
        s_state_transit.last_secure_boot_state = fwup_get_status();
        s_state_transit.check_status_counter   = 0;
        s_state_transit.state_transit_flag     = false;
    }
#else
    /* Fix me for other State transit monitoring module */
#endif  /* BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100 */
}
/**********************************************************************************************************************
 End of function fwup_state_monitoring_callback
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: fwup_interrupts_disable
 *******************************************************************************************************************//**
 * @brief   Set interrupt disable.
 * @details Set interrupt disable.
 */
void fwup_interrupts_disable(void)
{
#if (BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100)
    R_BSP_InterruptsDisable();
#else
    /* Fix me for other BSP module */
#endif  /* BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100 */
}
/**********************************************************************************************************************
 End of function fwup_interrupts_disable
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: fwup_register_protect_disable
 *******************************************************************************************************************//**
 * @brief   Callback function of State transit monitoring module.
 * @details Callback function of State transit monitoring module..
 */
void fwup_register_protect_disable(void)
{
#if (BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100)
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
#else
    /* Fix me for other BSP module */
#endif  /* BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100 */
}
/**********************************************************************************************************************
 End of function fwup_register_protect_disable
 *********************************************************************************************************************/

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS)
/***********************************************************************************************************************
 * Function Name: fwup_software_delay_ms
 *******************************************************************************************************************//**
 * @brief   Software delay function.
 * @details Software delay function.
 */
static void fwup_software_delay_ms(uint32_t delay)
{
#if (BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100)
    (void)R_BSP_SoftwareDelay(delay, BSP_DELAY_MILLISECS);
#else
    /* Fix me for other BSP module */
#endif  /* BSP_MCU_SERIES_RX700 || BSP_MCU_SERIES_RX600 || BSP_MCU_SERIES_RX200 || BSP_MCU_SERIES_RX100 */
}
#else
/* Fix me for other OS environment */
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS */

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_BOOTLOADER)
#if (FWUP_CFG_BOOT_PROTECT_ENABLE == 1)
/***********************************************************************************************************************
 * Function Name: fwup_get_boot_protect
 *******************************************************************************************************************//**
 * @brief   Get setting of current Boot Protect.
 * @retval  true    Protection enabled.
 * @retval  false    Protection disabled.
 * @details Get setting of current Boot Protect.
 */
bool fwup_get_boot_protect(void)
{
    bool ret;
#if (FLASH_TYPE == FLASH_TYPE_4) /* FLASH_TYPE_4 */
    fawreg_t faw;

    faw.LONG = FLASH.FAWMON.LONG;
    if (faw.BIT.FSPR == 1)
    {
        ret = false;
    }
    else
    {
        ret = true;
    }
#elif (FLASH_TYPE == FLASH_TYPE_3) /* FLASH_TYPE_3 */
    flash_err_t err;
    flash_lockbit_config_t info;

    info.block_start_address = FLASH_CF_BLOCK_0;
    info.num_blocks = BOOT_LOADER_MIRROR_BLOCK_NUMBER;

    R_BSP_InterruptsDisable();
    err = R_FLASH_Control(FLASH_CMD_LOCKBIT_READ, &info);
    R_BSP_InterruptsEnable();
    if (info.result == FLASH_RES_LOCKBIT_STATE_NON_PROTECTED)
    {
        ret = false;
    }
    else
    {
        ret = true;
    }
    if (err != FLASH_SUCCESS)
    {
        R_BSP_NOP(); // Your error handling code would go here.
    }
#elif (FLASH_TYPE == FLASH_TYPE_1) /* FLASH_TYPE_1 */
    flash_access_window_config_t access_window;

    R_BSP_InterruptsDisable();
    ret = R_FLASH_Control(FLASH_CMD_ACCESSWINDOW_GET, &access_window);
    R_BSP_InterruptsEnable();

    if (access_window.start_addr == (uint32_t)BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS &&
        access_window.end_addr == (uint32_t)BOOT_LOADER_LOW_ADDRESS)
    {
        ret = true;
    }
    else
    {
        ret = false;
    }
#else
    /* Fix me for other flash type */
#endif  /* (FLASH_TYPE == FLASH_TYPE_4) */
    return ret;
}

flash_err_t fwup_set_boot_protect(void)
{
    flash_err_t ret = FLASH_SUCCESS;
#if (FLASH_TYPE == FLASH_TYPE_4 || FLASH_TYPE == FLASH_TYPE_1) /* FLASH_TYPE_4, 1 */
    flash_access_window_config_t access_window;

    /* Set access window */
#if (FWUP_FLASH_BANK_MODE == 0) /* Dual mode */
    access_window.start_addr = (uint32_t)BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS;
    access_window.end_addr = (uint32_t)BOOT_LOADER_MIRROR_LOW_ADDRESS;
#else    // Linear mode
    access_window.start_addr = (uint32_t)BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS;
    access_window.end_addr = (uint32_t)BOOT_LOADER_LOW_ADDRESS;
#endif  /* FWUP_FLASH_BANK_MODE */

    /* Set access window, Set FSPR bit for boot protection also */
    ret = fwup_flash_accesswindow_set(&access_window);
#elif (FLASH_TYPE == FLASH_TYPE_3) /* FLASH_TYPE_3 */
    flash_lockbit_config_t info;

    info.block_start_address = FLASH_CF_BLOCK_0;
    info.num_blocks = BOOT_LOADER_MIRROR_BLOCK_NUMBER;

    R_BSP_InterruptsDisable();
    ret = R_FLASH_Control(FLASH_CMD_LOCKBIT_WRITE, &info);
    R_BSP_InterruptsEnable();
    if (ret != FLASH_SUCCESS)
    {
        return ret;
    }
    R_BSP_InterruptsDisable();
    ret = R_FLASH_Control(FLASH_CMD_LOCKBIT_ENABLE, NULL);
    R_BSP_InterruptsEnable();
#else
    /* Fix me for other protection scheme */
#endif  /* (FLASH_TYPE == FLASH_TYPE_4 || FLASH_TYPE == FLASH_TYPE_1) */
    return ret;
}
#endif  /* FWUP_CFG_BOOT_PROTECT_ENABLE == 1 */
#else /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_BOOTLOADER */
/* Fix me for other OS environment */
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_BOOTLOADER */

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT != FWUP_IMPLEMENTATION_AFRTOS)
/***********************************************************************************************************************
 * Function Name: fwup_verification_sha256_ecdsa
 * Description  :
 * Arguments    :
 * Return Value : none
 **********************************************************************************************************************/
int32_t fwup_verification_sha256_ecdsa(const uint8_t *pucData, uint32_t ulSize, const uint8_t *pucSignature,
        uint32_t ulSignatureSize)
{
    int32_t   x_result = -1;
    uint8_t   puc_hash[TC_SHA256_DIGEST_SIZE];
    uint8_t   data_length;
    uint8_t   public_key[64];
    uint8_t   binary[256];
    uint8_t * p_head_pointer;
    uint8_t * p_current_pointer;
    uint8_t * p_tail_pointer;

    /* Hash message */
    struct tc_sha256_state_struct x_ctx;
    tc_sha256_init( &x_ctx);
    tc_sha256_update( &x_ctx, pucData, ulSize);
    tc_sha256_final(puc_hash, &x_ctx);

    /* extract public key from g_code_signer_public_key (pem format) */
    p_head_pointer = (uint8_t*)strstr((char*)g_code_signer_public_key, "-----BEGIN PUBLIC KEY-----");
    if (p_head_pointer)
    {
        p_head_pointer += strlen("-----BEGIN PUBLIC KEY-----");
        p_tail_pointer =  (uint8_t*)strstr((char*)g_code_signer_public_key, "-----END PUBLIC KEY-----");
        base64_decode(p_head_pointer, binary, p_tail_pointer - p_head_pointer);
        p_current_pointer = binary;
        data_length       = *(p_current_pointer + 1);
        while (1)
        {
            switch (*p_current_pointer)
            {
                case 0x30: /* found "SEQUENCE" */
                    p_current_pointer += 2;
                    break;
                case 0x03: /* found BIT STRING (maybe public key) */
                    if (0x42 == (*(p_current_pointer + 1)))
                    {
                        memcpy(public_key, p_current_pointer + 4, 64);

                        /* Verify signature */
                        if (uECC_verify(public_key, puc_hash, TC_SHA256_DIGEST_SIZE, pucSignature, uECC_secp256r1()))
                        {
                            x_result = 0;
                        }
                    }
                    p_current_pointer += ((*(p_current_pointer + 1)) + 2);
                    break;
                default:
                    p_current_pointer += ((*(p_current_pointer + 1)) + 2);
                    break;
            }
            if ((p_current_pointer - binary) > data_length)
            {

                /* parsing error */
                break;
            }
        }
    }
    return x_result;
}
/**********************************************************************************************************************
 End of function fwup_verification_sha256_ecdsa
 *********************************************************************************************************************/
#endif  /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT != FWUP_IMPLEMENTATION_AFRTOS */

#if ( BSP_CFG_USER_CHARPUT_ENABLED == 1 )
/***********************************************************************************************************************
 * Function Name: my_sw_charput_function
 * Description  : char data output API
 * Arguments    : data -
 *                    Send data with SCI
 * Return Value : none
 **********************************************************************************************************************/
void my_sw_charput_function(uint8_t data)
{
    uint32_t arg = 0;

    /* do not call printf()->charput in interrupt context */
    do
    {

        /* Casting void pointer is used for address. */
        R_SCI_Control(s_fwup_communication_handle, SCI_CMD_TX_Q_BYTES_FREE, (void*) &arg);
    } while (SCI_CFG_CH8_TX_BUFSIZ != arg);

    /* Casting uint8_t pointer is used for address. */
    R_SCI_Send(s_fwup_communication_handle, (uint8_t*) &data, 1);

    return;
}
/**********************************************************************************************************************
 End of function my_sw_charput_function
 *********************************************************************************************************************/
#endif /* BSP_CFG_USER_CHARPUT_ENABLED */

#if ( BSP_CFG_USER_CHARGET_ENABLED == 1 )
/***********************************************************************************************************************
 * Function Name: my_sw_charget_function
 * Description  :
 * Arguments    :
 * Return Value : none
 **********************************************************************************************************************/
void my_sw_charget_function (void)
{
    R_BSP_NOP();
}
/**********************************************************************************************************************
 End of function my_sw_charget_function
 *********************************************************************************************************************/
#endif /* BSP_CFG_USER_CHARGET_ENABLED */

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS)

/*-----------------------------------------------------------*/

static int s_vsnprintf_safe(int8_t * s,
                            size_t n,
                            const int8_t * format,
                            va_list arg)
{
    int ret;

    ret = vsnprintf( (char *)s, n, (const char *)format, arg );

    /* Check if the string was truncated and if so, update the return value
     * to reflect the number of characters actually written. */
    if( ret >= n )
    {
        /* Do not include the terminating NULL character to keep the behaviour
         * same as the standard. */
        ret = n - 1;
    }
    else if( ret < 0 )
    {
        /* Encoding error - Return 0 to indicate that nothing was written to the
         * buffer. */
        ret = 0;
    }
    else
    {
        /* Complete string was written to the buffer. */
    }

    return ret;
}

/*-----------------------------------------------------------*/

static int s_snprintf_safe( int8_t * s,
                          size_t n,
                          const int8_t * format,
                          ... )
{
    int ret;
    va_list args;

    va_start( args, format );
    ret = s_vsnprintf_safe( s, n, format, args );
    va_end( args );

    return ret;
}

/*-----------------------------------------------------------*/

static void s_log_printf_common(uint8_t usLoggingLevel,
                                const int8_t * pcFormat,
                                va_list args)
{
    size_t xLength = 0;
    int8_t pcPrintString[configLOGGING_MAX_MESSAGE_LENGTH];

    if (NULL != pcPrintString)
    {
        const int8_t * pcLevelString = NULL;
        size_t ulFormatLen = 0UL;

        /* Choose the string for the log level metadata for the log message. */
        switch (usLoggingLevel)
        {
            case LOG_ERROR:
                pcLevelString = "ERROR";
                break;

            case LOG_WARN:
                pcLevelString = "WARN";
                break;

            case LOG_INFO:
            case LOG_INFO_NOLF:
                pcLevelString = "INFO";
                break;

            case LOG_DEBUG:
                pcLevelString = "DEBUG";
                break;

            case LOG_INFO_CONTINUE:
            default:
                pcLevelString = NULL;
                break;
        }

        /* Add the chosen log level information as prefix for the message. */
        if( ( pcLevelString != NULL ) && ( xLength < configLOGGING_MAX_MESSAGE_LENGTH ) )
        {
            xLength += s_snprintf_safe( pcPrintString + xLength, configLOGGING_MAX_MESSAGE_LENGTH - xLength, "[%s] ", pcLevelString );
        }

        if( xLength < configLOGGING_MAX_MESSAGE_LENGTH )
        {
            xLength += s_vsnprintf_safe( pcPrintString + xLength, configLOGGING_MAX_MESSAGE_LENGTH - xLength, pcFormat, args );
        }

        /* Add newline characters if the message does not end with them.*/
        ulFormatLen = strlen( (char *)pcFormat );

        if( ( ulFormatLen >= 2 ) &&
            ( strncmp( (char *)(pcFormat + ulFormatLen), "\r\n", 2 ) != 0 ) &&
            ( xLength < configLOGGING_MAX_MESSAGE_LENGTH ) &&
			( LOG_INFO_NOLF != usLoggingLevel) )
        {
            xLength += s_snprintf_safe( (int8_t *)(pcPrintString + xLength), configLOGGING_MAX_MESSAGE_LENGTH - xLength, "%s", "\r\n" );
        }

        pcPrintString[xLength] = '\0';

        /* Only send the buffer to the logging task if it is
         * not empty. */
        if (xLength > 0)
        {
            printf ("%s", pcPrintString);
        }
    }
}

/*-----------------------------------------------------------*/

static void s_log_printf_error(const int8_t * pcFormat,
                          ...)
{
    va_list args;

    va_start (args, pcFormat);
    s_log_printf_common (LOG_ERROR, pcFormat, args);

    va_end (args);
}

/*-----------------------------------------------------------*/

static void s_log_printf_warn(const int8_t * pcFormat,
                         ...)
{
    va_list args;

    va_start (args, pcFormat);
    s_log_printf_common (LOG_WARN, pcFormat, args);

    va_end (args);
}

/*-----------------------------------------------------------*/

static void s_log_printf_info(const int8_t * pcFormat,
                         ...)
{
    va_list args;

    va_start (args, pcFormat);
    s_log_printf_common (LOG_INFO, pcFormat, args);

    va_end (args);
}

/*-----------------------------------------------------------*/

static void s_log_printf_info_nolf(const int8_t * pcFormat,
                         ...)
{
    va_list args;

    va_start (args, pcFormat);
    s_log_printf_common (LOG_INFO_NOLF, pcFormat, args);

    va_end (args);
}

/*-----------------------------------------------------------*/

static void s_log_printf_info_continue(const int8_t * pcFormat,
                         ...)
{
    va_list args;

    va_start (args, pcFormat);
    s_log_printf_common (LOG_INFO_CONTINUE, pcFormat, args);

    va_end (args);
}

/*-----------------------------------------------------------*/

static void s_log_printf_debug(const int8_t * pcFormat,
                          ...)
{
    va_list args;

    va_start (args, pcFormat);
    s_log_printf_common (LOG_DEBUG, pcFormat, args);

    va_end (args);
}

/*-----------------------------------------------------------*/

#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS */

#if (FWUP_UNIT_TEST == 1)
void g_setting_unit_test_s_fwup_state (e_fwup_state_t stat)
{
    s_fwup_state = stat;
}
#endif /* FWUP_UNIT_TEST */
