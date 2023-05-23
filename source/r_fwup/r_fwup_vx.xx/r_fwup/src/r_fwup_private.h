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
 * Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_fwup_private.h
 * Description  : Functions for using Firmware update module.
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
 *           31.05.2022 1.05    Added support for RX660
 *           05.12.2022 1.06    Added support for Azure ADU
 *                              Added support for excluding communication drivers
 *                              Added support for unbuffered FW updates
 *********************************************************************************************************************/


/*****************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "platform.h"

#ifndef FWUP_PRIVATE_H
#define FWUP_PRIVATE_H

/******************************************************************************
 Macro definitions
 ******************************************************************************/
/* Set the implementation environment. */
#define FWUP_IMPLEMENTATION_BOOTLOADER  (0)   /* Bootloader. (default) */
#define FWUP_IMPLEMENTATION_NONEOS      (1)   /* Firmware update w/o OS. */
#define FWUP_IMPLEMENTATION_NONEOSDRV   (2)   /* Firmware update w/o OS, communication driver-less. */
#define FWUP_IMPLEMENTATION_AFRTOS      (3)   /* Firmware update with Amazon FreeRTOS(OTA). */
#define FWUP_IMPLEMENTATION_AZURE       (4)   /* Firmware update with Azure ADU. */

/* Set the communication function. */
#define FWUP_COMMUNICATION_SCI          (0)   /* SCI connection. (default) */
#define FWUP_COMMUNICATION_ETHERNET     (1)   /* Ethernet connection. */
#define FWUP_COMMUNICATION_USB          (2)   /* USB connection. */
#define FWUP_COMMUNICATION_SDHI         (3)   /* SDHI connection. */
#define FWUP_COMMUNICATION_QSPI         (4)   /* QSPI connection. */

/* Set the combination of "implementation environment" and "communication function". */
#define FWUP_COMM_SCI_BOOTLOADER        (0)   /* Used for Bootloader with SCI connection from COM port.*/
#define FWUP_COMM_SCI_PRIMITIVE         (1)   /* SCI connection from COM port using primitive R/W.*/
#define FWUP_COMM_USB_PRIMITIVE         (2)   /* USB connection from COM port using primitive R/W.*/
#define FWUP_COMM_QSPI_PRIMITIVE        (3)   /* Connect external storage (SD card) to QSPI using primitive R/W.*/
#define FWUP_COMM_SCI_AFRTOS            (4)   /* Connect wireless module to SCI with Amazon FreeRTOS.*/
#define FWUP_COMM_ETHER_AFRTOS          (5)   /* Connect Eathernet with Amazon FreeRTOS.*/
#define FWUP_COMM_USB_AFRTOS            (6)   /* Connect LTE modem to USB with Amazon FreeRTOS.*/
#define FWUP_COMM_SDHI_AFRTOS           (7)   /* Connect wireless module to SDHI with Amazon FreeRTOS.*/
#define FWUP_COMM_ETHER_AZURE           (8)   /* Connect Eathernet (include Wi-Fi) with Azure ADU.*/

/* Set the flash bank mode. */
#ifdef BSP_CFG_CODE_FLASH_BANK_MODE
#if (BSP_CFG_CODE_FLASH_BANK_MODE == 0)
#define FWUP_FLASH_BANK_MODE            (0)   /* Dual bank mode */
#elif(BSP_CFG_CODE_FLASH_BANK_MODE == 1)
#define FWUP_FLASH_BANK_MODE            (1)   /* Linear mode */
#endif
#else /* if not defined BSP_CFG_CODE_FLASH_BANK_MODE */
#define FWUP_FLASH_BANK_MODE            (1)   /* Linear mode */
#endif /* BSP_CFG_CODE_FLASH_BANK_MODE */

/* Check the combination of "implementation environment" and "communication function"
 and set value to FWUP_ENV_COMMUNICATION_FUNCTION.
 0 = Used for Bootloader with SCI connection from COM port.
 1 = SCI connection from COM port using primitive R/W.
 2 = USB connection from COM port using primitive R/W.
 3 = Connect external storage (SD card) to QSPI using primitive R/W.
 4 = Connect wireless module to SCI with Amazon FreeRTOS.
 5 = Connect Eathernet with Amazon FreeRTOS.
 6 = Connect LTE modem to USB with Amazon FreeRTOS.
 7 = Connect wireless module to SDHI with Amazon FreeRTOS.
 8 = External storage (SD card + file system) connected to SCI.
 9 = External storage (USB memory + file system) connected to USB.
 10 = External storage (SD card + file system) connected to SDHI.
 11 = External storage (Serial flash + file system) connected to QSPI.
 */

/* Case of Bootloader. */
#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_BOOTLOADER)
#if (FWUP_CFG_COMMUNICATION_FUNCTION == FWUP_COMMUNICATION_SCI)  /* Case of SCI. */
#define FWUP_ENV_COMMUNICATION_FUNCTION     (FWUP_COMM_SCI_BOOTLOADER)
#else
#error "Illegal combination. Check the CFG value of r_fwup_config.h"
#endif /* FWUP_CFG_COMMUNICATION_FUNCTION */

/* Case of Firmware update w/o OS. */
#elif ((FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS) || (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOSDRV))
#if (FWUP_CFG_COMMUNICATION_FUNCTION == FWUP_COMMUNICATION_SCI)  /* Case of SCI */
#define FWUP_ENV_COMMUNICATION_FUNCTION     (FWUP_COMM_SCI_PRIMITIVE)
#elif (FWUP_CFG_COMMUNICATION_FUNCTION == FWUP_COMMUNICATION_USB)  /* Case of USB. */
#define FWUP_ENV_COMMUNICATION_FUNCTION     (FWUP_COMM_USB_PRIMITIVE)
#elif (FWUP_CFG_COMMUNICATION_FUNCTION == FWUP_COMMUNICATION_QSPI)  /* Case of QSPI. */
#define FWUP_ENV_COMMUNICATION_FUNCTION     (FWUP_COMM_QSPI_PRIMITIVE)
#else
#error "Illegal combination. Check the CFG value of r_fwup_config.h"
#endif /* FWUP_CFG_COMMUNICATION_FUNCTION */

/* Case of Firmware update with Amazon FreeRTOS(OTA). */
#elif (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS)
#if (FWUP_CFG_COMMUNICATION_FUNCTION == FWUP_COMMUNICATION_SCI)  /* Case of SCI */
#define FWUP_ENV_COMMUNICATION_FUNCTION     (FWUP_COMM_SCI_AFRTOS)
#elif (FWUP_CFG_COMMUNICATION_FUNCTION == FWUP_COMMUNICATION_ETHERNET)  /* Case of Ethernet. */
#define FWUP_ENV_COMMUNICATION_FUNCTION     (FWUP_COMM_ETHER_AFRTOS)
#elif (FWUP_CFG_COMMUNICATION_FUNCTION == FWUP_COMMUNICATION_USB)  /* Case of USB. */
#define FWUP_ENV_COMMUNICATION_FUNCTION     (FWUP_COMM_USB_AFRTOS)
#elif (FWUP_CFG_COMMUNICATION_FUNCTION == FWUP_COMMUNICATION_SDHI)  /* Case of SDHI. */
#define FWUP_ENV_COMMUNICATION_FUNCTION     (FWUP_COMM_SDHI_AFRTOS)
#else
#error "Illegal combination. Check the CFG value of r_fwup_config.h"
#endif /* FWUP_CFG_COMMUNICATION_FUNCTION */

/* Case of Firmware update with Azure ADU. */
#elif (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AZURE)
#if (FWUP_CFG_COMMUNICATION_FUNCTION == FWUP_COMMUNICATION_ETHERNET)  /* Case of Ethernet. */
#define FWUP_ENV_COMMUNICATION_FUNCTION     (FWUP_COMM_ETHER_AZURE)
#else
#error "Illegal combination. Check the CFG value of r_fwup_config.h"
#endif /* FWUP_CFG_COMMUNICATION_FUNCTION */
#endif  /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT */

/*------------------------------------------ firmware update configuration (start) -----------------------------------*/
/* Common definition ------------------------------------------------------------------------------------------------ */
#define BOOT_LOADER_PGM_SIZE                        (65536)
#define BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH     (0x200)
#define BOOT_LOADER_USER_FIRMWARE_DESCRIPTOR_LENGTH (0x100)
#define INITIAL_FIRMWARE_FILE_NAME                  "userprog.rsu"

#define FLASH_INTERRUPT_PRIORITY (14)    /* 0(low) - 15(high) */

#define BOOT_LOADER_LOW_ADDRESS   (FLASH_CF_BLOCK_END - BOOT_LOADER_PGM_SIZE + 1)
#define USER_RESET_VECTOR_ADDRESS (BOOT_LOADER_LOW_ADDRESS - 4)
#if (FWUP_CFG_OTA_DATA_STORAGE == 0)
#define BOOT_LOADER_USER_CONST_DATA_LOW_ADDRESS           (FLASH_DF_BLOCK_0)
#define BOOT_LOADER_UPDATE_CONST_DATA_TARGET_BLOCK_NUMBER (FLASH_NUM_BLOCKS_DF)
#else /* FWUP_CFG_OTA_DATA_STORAGE = 1 */
#define BOOT_LOADER_USER_CONST_DATA_LOW_ADDRESS           (FLASH_CF_LO_BANK_LO_ADDR)
#define BOOT_LOADER_UPDATE_CONST_DATA_TARGET_BLOCK_NUMBER (1)
#endif /* FWUP_CFG_OTA_DATA_STORAGE */
#define BOOT_LOADER_CF_DF_BLOCK_SIZE            (FLASH_CF_MEDIUM_BLOCK_SIZE)

/* Definition for each MCU ------------------------------------------------------------------------------------------ */
#if ((FLASH_TYPE == FLASH_TYPE_4) || (FLASH_TYPE == FLASH_TYPE_3))
#define BOOT_LOADER_FLASH_CF_BLOCK_SIZE                 (FLASH_CF_MEDIUM_BLOCK_SIZE)

#if (FWUP_FLASH_BANK_MODE == 0) /* Dual mode */
#if (FWUP_CFG_OTA_DATA_STORAGE == 0) /* (Use Dataflash) */
#define BOOT_LOADER_TOTAL_UPDATE_SIZE                   ((BSP_ROM_SIZE_BYTES - (BOOT_LOADER_PGM_SIZE * 2)) >> 1) /* Size of User program */
#define BOOT_LOADER_UPDATE_EXECUTE_AREA_HIGH_ADDRESS    (BOOT_LOADER_LOW_ADDRESS - BOOT_LOADER_FLASH_CF_BLOCK_SIZE)
#define BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS     (BOOT_LOADER_LOW_ADDRESS - BOOT_LOADER_TOTAL_UPDATE_SIZE)
#define BOOT_LOADER_MIRROR_HIGH_ADDRESS                 (BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS - FLASH_CF_SMALL_BLOCK_SIZE)
#define BOOT_LOADER_MIRROR_LOW_ADDRESS                  (BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS - BOOT_LOADER_PGM_SIZE)
#define BOOT_LOADER_UPDATE_TEMPORARY_AREA_HIGH_ADDRESS  (BOOT_LOADER_MIRROR_LOW_ADDRESS - BOOT_LOADER_FLASH_CF_BLOCK_SIZE)
#define BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS   (BOOT_LOADER_MIRROR_LOW_ADDRESS - BOOT_LOADER_TOTAL_UPDATE_SIZE)
#define BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER          ((BOOT_LOADER_TOTAL_UPDATE_SIZE) / (FLASH_CF_MEDIUM_BLOCK_SIZE))
#define BOOT_LOADER_MIRROR_BLOCK_NUMBER                 ((BOOT_LOADER_PGM_SIZE) / (FLASH_CF_SMALL_BLOCK_SIZE))

#else /* FWUP_CFG_OTA_DATA_STORAGE == 1 (Use Codeflash) */
#define BOOT_LOADER_TOTAL_UPDATE_SIZE                   ((BSP_ROM_SIZE_BYTES - ((BOOT_LOADER_PGM_SIZE + BOOT_LOADER_CF_DF_BLOCK_SIZE) * 2)) >> 1) /* Size of User program */
#define BOOT_LOADER_UPDATE_EXECUTE_AREA_HIGH_ADDRESS    (BOOT_LOADER_LOW_ADDRESS - BOOT_LOADER_FLASH_CF_BLOCK_SIZE)
#define BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS     (BOOT_LOADER_LOW_ADDRESS - BOOT_LOADER_TOTAL_UPDATE_SIZE)
#define BOOT_LOADER_MIRROR_HIGH_ADDRESS                 (BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS - BOOT_LOADER_CF_DF_BLOCK_SIZE - FLASH_CF_SMALL_BLOCK_SIZE)
#define BOOT_LOADER_MIRROR_LOW_ADDRESS                  (BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS - BOOT_LOADER_CF_DF_BLOCK_SIZE - BOOT_LOADER_PGM_SIZE)
#define BOOT_LOADER_UPDATE_TEMPORARY_AREA_HIGH_ADDRESS  (BOOT_LOADER_MIRROR_LOW_ADDRESS - BOOT_LOADER_FLASH_CF_BLOCK_SIZE)
#define BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS   (BOOT_LOADER_MIRROR_LOW_ADDRESS - BOOT_LOADER_TOTAL_UPDATE_SIZE)
#define BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER          ((BOOT_LOADER_TOTAL_UPDATE_SIZE) / (FLASH_CF_MEDIUM_BLOCK_SIZE))
#define BOOT_LOADER_MIRROR_BLOCK_NUMBER                 ((BOOT_LOADER_PGM_SIZE) / (FLASH_CF_SMALL_BLOCK_SIZE))
#define BOOT_LOADER_UPDATE_EXECUTE_DATA_LOW_ADDRESS     (BOOT_LOADER_LOW_ADDRESS - BOOT_LOADER_TOTAL_UPDATE_SIZE - BOOT_LOADER_CF_DF_BLOCK_SIZE)
#define BOOT_LOADER_UPDATE_TEMPORARY_DATA_LOW_ADDRESS   (FLASH_CF_LO_BANK_LO_ADDR)
#endif /* FWUP_CFG_OTA_DATA_STORAGE */

#if (FLASH_TYPE == FLASH_TYPE_3) /* Flash Type 3 w/ Dual mode */
#error "Illegal definition. Check the CFG value of r_bsp_config.h, r_flash_config.h"
#endif /* (FLASH_TYPE == FLASH_TYPE_3) */
#if( FWUP_CFG_NO_USE_BUFFER == 1 )
#error "Illegal definition. Check the CFG value of r_bsp_config.h, r_flash_config.h"
#endif /* ( FWUP_CFG_NO_USE_BUFFER == 1 ) */

#else /* Linear mode */
#if( FWUP_CFG_NO_USE_BUFFER == 1 )
#define BOOT_LOADER_TOTAL_UPDATE_SIZE                   (BSP_ROM_SIZE_BYTES - BOOT_LOADER_PGM_SIZE) /* Size of User program */
#define BOOT_LOADER_UPDATE_EXECUTE_AREA_HIGH_ADDRESS    (BOOT_LOADER_LOW_ADDRESS - BOOT_LOADER_FLASH_CF_BLOCK_SIZE)
#define BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS     (BOOT_LOADER_LOW_ADDRESS - BOOT_LOADER_TOTAL_UPDATE_SIZE)
#define BOOT_LOADER_UPDATE_TEMPORARY_AREA_HIGH_ADDRESS  BOOT_LOADER_UPDATE_EXECUTE_AREA_HIGH_ADDRESS
#define BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS   BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS
#else
#define BOOT_LOADER_TOTAL_UPDATE_SIZE                   (BSP_ROM_SIZE_BYTES - BOOT_LOADER_PGM_SIZE >> 1) /* Size of User program */
#define BOOT_LOADER_UPDATE_EXECUTE_AREA_HIGH_ADDRESS    (BOOT_LOADER_LOW_ADDRESS - BOOT_LOADER_FLASH_CF_BLOCK_SIZE)
#define BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS     (BOOT_LOADER_LOW_ADDRESS - BOOT_LOADER_TOTAL_UPDATE_SIZE)
#define BOOT_LOADER_UPDATE_TEMPORARY_AREA_HIGH_ADDRESS  (BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS - BOOT_LOADER_FLASH_CF_BLOCK_SIZE)
#define BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS   (BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS - BOOT_LOADER_TOTAL_UPDATE_SIZE)
#endif /* ( FWUP_CFG_NO_USE_BUFFER == 1 ) */
#define BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER          ((BOOT_LOADER_TOTAL_UPDATE_SIZE) / (FLASH_CF_MEDIUM_BLOCK_SIZE))
#define BOOT_LOADER_MIRROR_BLOCK_NUMBER                 ((BOOT_LOADER_PGM_SIZE) / (FLASH_CF_SMALL_BLOCK_SIZE))
#endif /* (FWUP_FLASH_BANK_MODE) */

#elif (FLASH_TYPE == FLASH_TYPE_1)
#define BOOT_LOADER_FLASH_CF_BLOCK_SIZE                 (FLASH_CF_BLOCK_SIZE)
#define BOOT_LOADER_TOTAL_UPDATE_SIZE                   (BSP_ROM_SIZE_BYTES - BOOT_LOADER_PGM_SIZE >> 1) /* Size of User program */
#define BOOT_LOADER_UPDATE_EXECUTE_AREA_HIGH_ADDRESS    (BOOT_LOADER_LOW_ADDRESS - BOOT_LOADER_FLASH_CF_BLOCK_SIZE)
#define BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS     (BOOT_LOADER_LOW_ADDRESS - BOOT_LOADER_TOTAL_UPDATE_SIZE)
#define BOOT_LOADER_UPDATE_TEMPORARY_AREA_HIGH_ADDRESS  (BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS - BOOT_LOADER_FLASH_CF_BLOCK_SIZE)
#define BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS   (BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS - BOOT_LOADER_TOTAL_UPDATE_SIZE)
#define BOOT_LOADER_UPDATE_TARGET_BLOCK_NUMBER          ((BOOT_LOADER_TOTAL_UPDATE_SIZE) / (FLASH_CF_BLOCK_SIZE))
#define BOOT_LOADER_MIRROR_BLOCK_NUMBER                 ((BOOT_LOADER_PGM_SIZE) / (FLASH_CF_BLOCK_SIZE))

#if (FWUP_FLASH_BANK_MODE == 0) /* Flash type 1 w/ Dual mode */
#error "Illegal definition. Check the CFG value of r_bsp_config.h, r_flash_config.h"
#endif /* (FWUP_FLASH_BANK_MODE) */
#if( FWUP_CFG_NO_USE_BUFFER == 1 )
#error "Illegal definition. Check the CFG value of r_bsp_config.h, r_flash_config.h"
#endif /* ( FWUP_CFG_NO_USE_BUFFER == 1 ) */

#endif /* (FLASH_TYPE) */

/* Start address for FLASH erase */
#if (FLASH_TYPE == FLASH_TYPE_4 || FLASH_TYPE == FLASH_TYPE_3)
#define BOOT_LOADER_UPDATE_TEMPORARY_AREA_ERASE_ADDRESS    (BOOT_LOADER_UPDATE_TEMPORARY_AREA_HIGH_ADDRESS)
#define BOOT_LOADER_UPDATE_EXECUTE_AREA_ERASE_ADDRESS      (BOOT_LOADER_UPDATE_EXECUTE_AREA_HIGH_ADDRESS)
#define BOOT_LOADER_MIRROR_ERASE_ADDRESS                   (BOOT_LOADER_MIRROR_HIGH_ADDRESS)
#elif(FLASH_TYPE == FLASH_TYPE_1)
#define BOOT_LOADER_UPDATE_TEMPORARY_AREA_ERASE_ADDRESS    (BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS)
#define BOOT_LOADER_UPDATE_EXECUTE_AREA_ERASE_ADDRESS      (BOOT_LOADER_UPDATE_EXECUTE_AREA_LOW_ADDRESS)
#define BOOT_LOADER_MIRROR_ERASE_ADDRESS                   (BOOT_LOADER_MIRROR_LOW_ADDRESS)
#endif
/*------------------------------------------ firmware update configuration (end) -------------------------------------*/

#define LIFECYCLE_STATE_BLANK       (0xff)
#define LIFECYCLE_STATE_TESTING     (0xfe)
#define LIFECYCLE_STATE_INSTALLING  (0xfc)
#define LIFECYCLE_STATE_VALID       (0xf8)
#define LIFECYCLE_STATE_INVALID     (0xf0)
#define LIFECYCLE_STATE_EOL         (0xe0)

#define DIRECT_UPDATE_ON            (0x01)

#define R_OTA_ERR_NONE              (0)
#define R_OTA_ERR_INVALID_CONTEXT   (-1)
#define R_OTA_ERR_QUEUE_SEND_FAIL   (-2)

#define OTA_FLASHING_IN_PROGRESS    (0)
#define OTA_FLASHING_COMPLETE       (1)

#define OTA_SIGUNATURE_SEQUENCE_TOP_VALUE                   (0x30)
#define OTA_SIGUNATURE_INTEGER_VALUE                        (0x02)
#define OTA_SIGUNATURE_NOT_INCLUDE_NEGATIVE_NUMBER_VALUE    (0x20)
#define OTA_SIGUNATURE_INCLUDE_NEGATIVE_NUMBER_VALUE        (0x21)
#define OTA_SIGUNATURE_DATA_HALF_LENGTH                     (32)
#define OTA_SIGUNATURE_SEQUENCE_INFO_LENGTH                 (2)
#define OTA_SIGUNATURE_SKIP                                 (2)

#define OTA_FLASH_MIN_PGM_SIZE_MASK (0xFFFFFFFF - FLASH_CF_MIN_PGM_SIZE + 1)

#define FWUP_SCI_CONTROL_BLOCK_A         (0)
#define FWUP_SCI_CONTROL_BLOCK_B         (1)
#define FWUP_SCI_CONTROL_BLOCK_TOTAL_NUM (2)

#define FWUP_SCI_RECEIVE_BUFFER_EMPTY (0)
#define FWUP_SCI_RECEIVE_BUFFER_FULL  (1)

#define MONITORING_STATUS_INTERVAL    (1)
#define MONITORING_STATUS_COUNT       (6000)

#define FRAGMENTED_PACKET_TYPE_HEAD (0)
#define FRAGMENTED_PACKET_TYPE_TAIL (1)

#define FRAGMENTED_FLASH_BLOCK_TYPE_HEAD (0)
#define FRAGMENTED_FLASH_BLOCK_TYPE_TAIL (1)

#define FWUP_BYTE_POOL_SIZE     (0x2000)
#define FWUP_TASK_STACK_SIZE    (2048)
#define FWUP_TASK_PRI           (10)
#define FWUP_QUEUE_SIZE         (1)

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS)
#define FWUP_WRITE_BASE_ADDR (BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS + BOOT_LOADER_USER_FIRMWARE_HEADER_LENGTH)
#elif (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AZURE)
#define FWUP_WRITE_BASE_ADDR (BOOT_LOADER_UPDATE_TEMPORARY_AREA_LOW_ADDRESS)
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT */

#if (FWUP_CFG_OTA_DATA_STORAGE == 1)
#define FWUP_COPY_FLAG_ON          (0x00)
#define FWUP_COPY_FLAG_OFF         (0xff)
#endif /* FWUP_CFG_OTA_DATA_STORAGE == 1 */

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_BOOTLOADER)
#define FWUP_WRITE_BLOCK_SIZE       (1024)
/* For reduce the ROM size by control printf */
#if (FWUP_CFG_BOOTLOADER_LOG_DISABLE == 1)
/* Log output w/o variable output */
# define DEBUG_LOG(str)
/* Log output w/ variable output */
# define DEBUG_LOG2(fmt, ...)
#else
/* Log output w/o variable output */
#define DEBUG_LOG(fmt)           printf(fmt)
/* Log output w/ variable output */
#define DEBUG_LOG2(fmt, ...)     printf(fmt, __VA_ARGS__)
#endif /* FWUP_CFG_BOOTLOADER_LOG_DISABLE */
#else
#define FWUP_WRITE_BLOCK_SIZE       (1024)
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT */

#define FLASH_DF_TOTAL_BLOCK_SIZE (FLASH_DF_BLOCK_INVALID - FLASH_DF_BLOCK_0)

#define INTEGRITY_CHECK_SCHEME_HASH_SHA256_STANDALONE      "hash-sha256"
#define INTEGRITY_CHECK_SCHEME_SIG_SHA256_ECDSA_STANDALONE "sig-sha256-ecdsa"

#if (FWUP_CFG_COMMUNICATION_FUNCTION == FWUP_COMMUNICATION_SCI)  /* Case of SCI */
#if !defined(FWUP_CFG_SERIAL_TERM_SCI)
#error "Error! Need to define FWUP_CFG_SERIAL_TERM_SCI in r_fwup_config.h"
#elif FWUP_CFG_SERIAL_TERM_SCI == (0)
#define R_SCI_PinSet_serial_term()   (R_SCI_PinSet_SCI0())
#define SCI_CH_serial_term           (SCI_CH0)
#define SCI_CH_TX_BUFSIZ_serial_term (SCI_CFG_CH0_TX_BUFSIZ)
#elif FWUP_CFG_SERIAL_TERM_SCI == (1)
#define R_SCI_PinSet_serial_term()   (R_SCI_PinSet_SCI1())
#define SCI_CH_serial_term           (SCI_CH1)
#define SCI_CH_TX_BUFSIZ_serial_term (SCI_CFG_CH1_TX_BUFSIZ)
#elif FWUP_CFG_SERIAL_TERM_SCI == (2)
#define R_SCI_PinSet_serial_term()   (R_SCI_PinSet_SCI2())
#define SCI_CH_serial_term           (SCI_CH2)
#define SCI_CH_TX_BUFSIZ_serial_term (SCI_CFG_CH2_TX_BUFSIZ)
#elif FWUP_CFG_SERIAL_TERM_SCI == (3)
#define R_SCI_PinSet_serial_term()   (R_SCI_PinSet_SCI3())
#define SCI_CH_serial_term           (SCI_CH3)
#define SCI_CH_TX_BUFSIZ_serial_term (SCI_CFG_CH3_TX_BUFSIZ)
#elif FWUP_CFG_SERIAL_TERM_SCI == (4)
#define R_SCI_PinSet_serial_term()   (R_SCI_PinSet_SCI4())
#define SCI_CH_serial_term           (SCI_CH4)
#define SCI_CH_TX_BUFSIZ_serial_term (SCI_CFG_CH4_TX_BUFSIZ)
#elif FWUP_CFG_SERIAL_TERM_SCI == (5)
#define R_SCI_PinSet_serial_term()   (R_SCI_PinSet_SCI5())
#define SCI_CH_serial_term           (SCI_CH5)
#define SCI_CH_TX_BUFSIZ_serial_term (SCI_CFG_CH5_TX_BUFSIZ)
#elif FWUP_CFG_SERIAL_TERM_SCI == (6)
#define R_SCI_PinSet_serial_term()   (R_SCI_PinSet_SCI6())
#define SCI_CH_serial_term           (SCI_CH6)
#define SCI_CH_TX_BUFSIZ_serial_term (SCI_CFG_CH6_TX_BUFSIZ)
#elif FWUP_CFG_SERIAL_TERM_SCI == (7)
#define R_SCI_PinSet_serial_term()   (R_SCI_PinSet_SCI7())
#define SCI_CH_serial_term           (SCI_CH7)
#define SCI_CH_TX_BUFSIZ_serial_term (SCI_CFG_CH7_TX_BUFSIZ)
#elif FWUP_CFG_SERIAL_TERM_SCI == (8)
#define R_SCI_PinSet_serial_term()   (R_SCI_PinSet_SCI8())
#define SCI_CH_serial_term           (SCI_CH8)
#define SCI_CH_TX_BUFSIZ_serial_term (SCI_CFG_CH8_TX_BUFSIZ)
#elif FWUP_CFG_SERIAL_TERM_SCI == (9)
#define R_SCI_PinSet_serial_term()   (R_SCI_PinSet_SCI9())
#define SCI_CH_serial_term           (SCI_CH9)
#define SCI_CH_TX_BUFSIZ_serial_term (SCI_CFG_CH9_TX_BUFSIZ)
#elif FWUP_CFG_SERIAL_TERM_SCI == (10)
#define R_SCI_PinSet_serial_term()   (R_SCI_PinSet_SCI10())
#define SCI_CH_serial_term           (SCI_CH10)
#define SCI_CH_TX_BUFSIZ_serial_term (SCI_CFG_CH10_TX_BUFSIZ)
#elif FWUP_CFG_SERIAL_TERM_SCI == (11)
#define R_SCI_PinSet_serial_term()   (R_SCI_PinSet_SCI11())
#define SCI_CH_serial_term           (SCI_CH11)
#define SCI_CH_TX_BUFSIZ_serial_term (SCI_CFG_CH11_TX_BUFSIZ)
#elif FWUP_CFG_SERIAL_TERM_SCI == (12)
#define R_SCI_PinSet_serial_term()   (R_SCI_PinSet_SCI12())
#define SCI_CH_serial_term           (SCI_CH12)
#define SCI_CH_TX_BUFSIZ_serial_term (SCI_CFG_CH12_TX_BUFSIZ)
#else /* FWUP_CFG_SERIAL_TERM_SCI */
#error "Error! Invalid setting for FWUP_CFG_SERIAL_TERM_SCI in r_fwup_config.h"
#endif /*  containing !definedFWUP_CFG_SERIAL_TERM_SCI */
#endif  /* (FWUP_CFG_COMMUNICATION_FUNCTION == FWUP_COMMUNICATION_SCI) */

#define BOOT_LOADER_MAGIC_CODE        "Renesas"
#define BOOT_LOADER_MAGIC_CODE_LENGTH (7)

/* The OTA signature algorithm string is specified by the PAL. */
#define OTA_FILE_SIG_KEY_STR_MAX_LENGTH    (32)

#if ((FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS) || (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOSDRV) || (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AZURE))
/**
 * @brief No log messages.
 *
 * When @ref FWUP_CFG_LOG_LEVEL is #LOG_NONE, logging is disabled and no
 * logging messages are printed.
 */
#define LOG_NONE            (0)
#define LOG_ERROR           (1)
#define LOG_WARN            (2)
#define LOG_INFO            (3)
#define LOG_DEBUG           (4)
#define LOG_INFO_NOLF       (5)
#define LOG_INFO_CONTINUE   (6)

#if FWUP_CFG_LOG_LEVEL > LOG_DEBUG
#error "Illegal setting of FWUP_CFG_LOG_LEVEL. Check the CFG value of r_fwup_config.h"
#endif /* FWUP_CFG_LOG_LEVEL > LOG_DEBUG */

#if FWUP_CFG_LOG_LEVEL == LOG_DEBUG
/* All log level messages will logged. */
#define LogError( message )          s_log_printf_error message
#define LogWarn( message )
#define LogInfo( message )           s_log_printf_info message
#define LogInfoNoLF( message )       s_log_printf_info_nolf message
#define LogInfoContinue( message )   s_log_printf_info_continue message
#define LogDebug( message )          s_log_printf_debug message

#elif FWUP_CFG_LOG_LEVEL == LOG_INFO
/* Only INFO, WARNING and ERROR messages will be logged. */
#define LogError( message )          s_log_printf_error message
#define LogWarn( message )
#define LogInfo( message )           s_log_printf_info message
#define LogInfoNoLF( message )       s_log_printf_info_nolf message
#define LogInfoContinue( message )   s_log_printf_info_continue message
#define LogDebug( message )

#elif FWUP_CFG_LOG_LEVEL == LOG_WARN
/* Only WARNING and ERROR messages will be logged.*/
#define LogError( message )          s_log_printf_error message
#define LogWarn( message )
#define LogInfo( message )
#define LogInfoNoLF( message )
#define LogInfoContinue( message )
#define LogDebug( message )

#elif FWUP_CFG_LOG_LEVEL == LOG_ERROR
/* Only ERROR messages will be logged. */
#define LogError( message )          s_log_printf_error message
#define LogWarn( message )
#define LogInfo( message )
#define LogInfoNoLF( message )
#define LogInfoContinue( message )
#define LogDebug( message )

#else
#define LogError( message )
#define LogWarn( message )
#define LogInfo( message )
#define LogInfoNoLF( message )
#define LogInfoContinue( message )
#define LogDebug( message )
#endif /* FWUP_CFG_LOG_LEVEL == LOG_DEBUG */

#define configLOGGING_MAX_MESSAGE_LENGTH (200)

/**
 * @constantspage{ota,OTA library}
 *
 * @section ota_constants_err_codes OTA Error Codes
 * @brief OTA Agent error codes returned by OTA agent API.
 *
 * @snippet this define_ota_err_codes
 *
 * OTA agent error codes are in the upper 8 bits of the 32 bit OTA error word, OTA_Err_t.
 *
 * @section ota_constants_err_code_helpers OTA Error Code Helper constants
 * @brief OTA Error code helper constant for extracting the error code from the OTA error returned.
 *
 * @snippet this define_ota_err_code_helpers
 *
 * OTA error codes consist of an agent code in the upper 8 bits of a 32 bit word and sometimes
 * merged with a platform specific code in the lower 24 bits. You must refer to the platform PAL
 * layer in use to determine the meaning of the lower 24 bits.
 */

/* @[define_ota_err_codes] */
#define kOTA_Err_Panic                   (0xfe000000UL)     /*!< Unrecoverable FW error. Probably should log error and reboot. */
#define kOTA_Err_Uninitialized           (0xff000000UL)     /*!< The error code has not yet been set by a logic path. */
#define kOTA_Err_None                    (0x00000000UL)
#define kOTA_Err_SignatureCheckFailed    (0x01000000UL)     /*!< The signature check failed for the specified file. */
#define kOTA_Err_BadSignerCert           (0x02000000UL)     /*!< The signer certificate was not readable or zero length. */
#define kOTA_Err_OutOfMemory             (0x03000000UL)     /*!< General out of memory error. */
#define kOTA_Err_ActivateFailed          (0x04000000UL)     /*!< The activation of the new OTA image failed. */
#define kOTA_Err_CommitFailed            (0x05000000UL)     /*!< The acceptance commit of the new OTA image failed. */
#define kOTA_Err_RejectFailed            (0x06000000UL)     /*!< Error trying to reject the OTA image. */
#define kOTA_Err_AbortFailed             (0x07000000UL)     /*!< Error trying to abort the OTA. */
#define kOTA_Err_PublishFailed           (0x08000000UL)     /*!< Attempt to publish a MQTT message failed. */
#define kOTA_Err_BadImageState           (0x09000000UL)     /*!< The specified OTA image state was out of range. */
#define kOTA_Err_NoActiveJob             (0x0a000000UL)     /*!< Attempt to set final image state without an active job. */
#define kOTA_Err_NoFreeContext           (0x0b000000UL)     /*!< There wasn't an OTA file context available for processing. */
#define kOTA_Err_HTTPInitFailed          (0x0c000000UL)     /*!< Error initializing the HTTP connection. */
#define kOTA_Err_HTTPRequestFailed       (0x0d000000UL)     /*!< Error sending the HTTP request. */
#define kOTA_Err_FileAbort               (0x10000000UL)     /*!< Error in low level file abort. */
#define kOTA_Err_FileClose               (0x11000000UL)     /*!< Error in low level file close. */
#define kOTA_Err_RxFileCreateFailed      (0x12000000UL)     /*!< The PAL failed to create the OTA receive file. */
#define kOTA_Err_BootInfoCreateFailed    (0x13000000UL)     /*!< The PAL failed to create the OTA boot info file. */
#define kOTA_Err_RxFileTooLarge          (0x14000000UL)     /*!< The OTA receive file is too big for the platform to support. */
#define kOTA_Err_NullFilePtr             (0x20000000UL)     /*!< Attempt to use a null file pointer. */
#define kOTA_Err_MomentumAbort           (0x21000000UL)     /*!< Too many OTA stream requests without any response. */
#define kOTA_Err_DowngradeNotAllowed     (0x22000000UL)     /*!< Firmware version is older than the previous version. */
#define kOTA_Err_SameFirmwareVersion     (0x23000000UL)     /*!< Firmware version is the same as previous. New firmware could have failed to commit. */
#define kOTA_Err_JobParserError          (0x24000000UL)     /*!< An error occurred during job document parsing. See reason sub-code. */
#define kOTA_Err_FailedToEncodeCBOR      (0x25000000UL)     /*!< Failed to encode CBOR object. */
#define kOTA_Err_ImageStateMismatch      (0x26000000UL)     /*!< The OTA job was in Self Test but the platform image state was not. Possible tampering. */
#define kOTA_Err_GenericIngestError      (0x27000000UL)     /*!< A failure in block ingestion not caused by the PAL. See the error sub code. */
#define kOTA_Err_UserAbort               (0x28000000UL)     /*!< User aborted the active OTA. */
#define kOTA_Err_ResetNotSupported       (0x29000000UL)     /*!< We tried to reset the device but the device doesn't support it. */
#define kOTA_Err_TopicTooLarge           (0x2a000000UL)     /*!< Attempt to build a topic string larger than the supplied buffer. */
#define kOTA_Err_SelfTestTimerFailed     (0x2b000000UL)     /*!< Attempt to start self-test timer faield. */
#define kOTA_Err_EventQueueSendFailed    (0x2c000000UL)     /*!< Posting event message to the event queue failed. */
#define kOTA_Err_InvalidDataProtocol     (0x2d000000UL)     /*!< Job does not have a valid protocol for data transfer. */
/* @[define_ota_err_codes] */

/* @[define_ota_err_code_helpers] */
#define OTA_PAL_ERR_MASK                    (0xffffffUL)                                                                                      /*!< The PAL layer uses the signed low 24 bits of the OTA error code. */
#define OTA_PAL_SUB_BITS                    (24U)                                                                                             /*!< The OTA Agent error code is the highest 8 bits of the word. */
#define OTA_PAL_MAIN_ERR( err )             ( ( OtaPalMainStatus_t ) ( uint32_t ) ( ( uint32_t ) ( err ) >> ( uint32_t ) OTA_PAL_SUB_BITS ) ) /*!< Helper to get the OTA PAL main error code. */
#define OTA_PAL_SUB_ERR( err )              ( ( uint32_t ) ( err ) & ( uint32_t ) OTA_PAL_ERR_MASK )                                          /*!< Helper to get the OTA PAL sub error code. */
#define OTA_PAL_COMBINE_ERR( main, sub )    ( ( ( uint32_t ) ( main ) << ( uint32_t ) OTA_PAL_SUB_BITS ) | ( uint32_t ) ( sub ) )             /*!< Helper to combine the OTA PAL main and sub error code. */
/* @[define_ota_err_code_helpers] */

/**
 * @brief Log base 2 of the size of the file data block message (excluding the header).
 *
 * 10 bits yields a data block size of 1KB.
 */
#define OTACONFIG_LOG2_FILE_BLOCK_SIZE           (10UL)

#define bool_t    uint8_t

#elif (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AFRTOS)
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT */

/*****************************************************************************
 Typedef definitions
 ******************************************************************************/
#if ((FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOS) || (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_NONEOSDRV) || (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_AZURE))
/**
 * @ingroup ota_enum_types
 * @brief OTA Image states.
 *
 * After an OTA update image is received and authenticated, it is logically moved to
 * the Self Test state by the OTA agent pending final acceptance. After the image is
 * activated and tested by your user code, you should put it into either the Accepted
 * or Rejected state by calling @ref OTA_SetImageState ( OtaImageStateAccepted ) or
 * @ref OTA_SetImageState ( OtaImageStateRejected ). If the image is accepted, it becomes
 * the main firmware image to be booted from then on. If it is rejected, the image is
 * no longer valid and shall not be used, reverting to the last known good image.
 *
 * If you want to abort an active OTA transfer, you may do so by calling the API
 * @ref OTA_SetImageState ( OtaImageStateAborted ).
 */
typedef enum OtaImageState
{
    OtaImageStateUnknown = 0,  /*!< @brief The initial state of the OTA MCU Image. */
    OtaImageStateTesting = 1,  /*!< @brief The state of the OTA MCU Image post successful download and reboot. */
    OtaImageStateAccepted = 2, /*!< @brief The state of the OTA MCU Image post successful download and successful self_test. */
    OtaImageStateRejected = 3, /*!< @brief The state of the OTA MCU Image when the job has been rejected. */
    OtaImageStateAborted = 4,  /*!< @brief The state of the OTA MCU Image after a timeout publish to the stream request fails.
                                *   Also if the OTA MCU image is aborted in the middle of a stream. */
    OtaImageState_EOL = 5,   /*!< @brief The state of the OTA MCU Image when the EOL process. */
    OtaLastImageState = OtaImageState_EOL
} OtaImageState_t;

/**
 * @ingroup ota_enum_types
 * @brief OTA Platform Image State.
 *
 * The image state set by platform implementation.
 */
typedef enum OtaPalImageState
{
    OtaPalImageStateUnknown = 0,   /*!< @brief The initial state of the OTA PAL Image. */
    OtaPalImageStatePendingCommit, /*!< @brief OTA PAL Image awaiting update. */
    OtaPalImageStateValid,         /*!< @brief OTA PAL Image is valid. */
    OtaPalImageStateInvalid        /*!< @brief OTA PAL Image is invalid. */
} OtaPalImageState_t;

/**
 * @brief The OTA platform interface return status. Composed of main and sub status.
 */
typedef uint32_t   OtaPalStatus_t;

/**
 * @brief The OTA platform interface sub status.
 */
typedef uint32_t   OtaPalSubStatus_t;

/**
 * @ingroup ota_enum_types
 * @brief The OTA platform interface main status.
 */
typedef enum OtaPalMainStatus
{
    OtaPalSuccess = 0,          /*!< @brief OTA platform interface success. */
    OtaPalUninitialized = 0xe0, /*!< @brief Result is not yet initialized from PAL. */
    OtaPalOutOfMemory,          /*!< @brief Out of memory. */
    OtaPalNullFileContext,      /*!< @brief The PAL is called with a NULL file context. */
    OtaPalSignatureCheckFailed, /*!< @brief The signature check failed for the specified file. */
    OtaPalRxFileCreateFailed,   /*!< @brief The PAL failed to create the OTA receive file. */
    OtaPalRxFileTooLarge,       /*!< @brief The OTA receive file is too big for the platform to support. */
    OtaPalBootInfoCreateFailed, /*!< @brief The PAL failed to create the OTA boot info file. */
    OtaPalBadSignerCert,        /*!< @brief The signer certificate was not readable or zero length. */
    OtaPalBadImageState,        /*!< @brief The specified OTA image state was out of range. */
    OtaPalAbortFailed,          /*!< @brief Error trying to abort the OTA. */
    OtaPalRejectFailed,         /*!< @brief Error trying to reject the OTA image. */
    OtaPalCommitFailed,         /*!< @brief The acceptance commit of the new OTA image failed. */
    OtaPalActivateFailed,       /*!< @brief The activation of the new OTA image failed. */
    OtaPalFileAbort,            /*!< @brief Error in low level file abort. */
    OtaPalFileClose             /*!< @brief Error in low level file close. */
} OtaPalMainStatus_t;

/**
 * @structs{ota,OTA library}
 */

/* A composite cryptographic signature structure able to hold our largest supported signature. */

/* Max bytes supported for a file signature (2048 bit RSA is 256 bytes). */
#define kOTA_MaxSignatureSize    (256)

/**
 * @ingroup ota_struct_types
 * @brief OTA File Signature info.
 *
 * File key signature information to verify the authenticity of the incoming file
 */
typedef struct
{
    uint16_t size;                         /*!< @brief Size, in bytes, of the signature. */
    uint8_t data[ kOTA_MaxSignatureSize ]; /*!< @brief The binary signature data. */
} Sig256_t;

/**
 * @ingroup ota_struct_types
 * @brief OTA File Context Information.
 *
 * Information about an OTA Update file that is to be streamed. This structure is filled in from a
 * job notification MQTT message. Currently only one file context can be streamed at time.
 */
typedef struct OtaFileContext
{
    uint8_t * pFilePath;          /*!< @brief Update file pathname. */
    uint16_t filePathMaxSize;     /*!< @brief Maximum size of the update file path */
    #if defined( WIN32 ) || defined( __linux__ )
        FILE * pFile;             /*!< @brief File type is stdio FILE structure after file is open for write. */
    #else
        uint8_t * pFile;          /*!< @brief File type is RAM/Flash image pointer after file is open for write. */
    #endif
    uint32_t fileSize;            /*!< @brief The size of the file in bytes. */
    uint32_t blocksRemaining;     /*!< @brief How many blocks remain to be received (a code optimization). */
    uint32_t fileAttributes;      /*!< @brief Flags specific to the file being received (e.g. secure, bundle, archive). */
    uint32_t serverFileID;        /*!< @brief The file is referenced by this numeric ID in the OTA job. */
    uint8_t * pJobName;           /*!< @brief The job name associated with this file from the job service. */
    uint16_t jobNameMaxSize;      /*!< @brief Maximum size of the job name. */
    uint8_t * pStreamName;        /*!< @brief The stream associated with this file from the OTA service. */
    uint16_t streamNameMaxSize;   /*!< @brief Maximum size of the stream name. */
    uint8_t * pRxBlockBitmap;     /*!< @brief Bitmap of blocks received (for deduplicating and missing block request). */
    uint16_t blockBitmapMaxSize;  /*!< @brief Maximum size of the block bitmap. */
    uint8_t * pCertFilepath;      /*!< @brief Pathname of the certificate file used to validate the receive file. */
    uint16_t certFilePathMaxSize; /*!< @brief Maximum certificate path size. */
    uint8_t * pUpdateUrlPath;     /*!< @brief Url for the file. */
    uint16_t updateUrlMaxSize;    /*!< @brief Maximum size of the url. */
    uint8_t * pAuthScheme;        /*!< @brief Authorization scheme. */
    uint16_t authSchemeMaxSize;   /*!< @brief Maximum size of the auth scheme. */
    uint32_t updaterVersion;      /*!< @brief Used by OTA self-test detection, the version of Firmware that did the update. */
    bool isInSelfTest;            /*!< @brief True if the job is in self test mode. */
    uint8_t * pProtocols;         /*!< @brief Authorization scheme. */
    uint16_t protocolMaxSize;     /*!< @brief Maximum size of the  supported protocols string. */
    uint8_t * pDecodeMem;         /*!< @brief Decode memory. */
    uint32_t decodeMemMaxSize;    /*!< @brief Maximum size of the decode memory. */
    uint32_t fileType;            /*!< @brief The file type id set when creating the OTA job. */
    Sig256_t * pSignature;        /*!< @brief Pointer to the file's signature structure. */
} OtaFileContext_t;

/**
 * @functionpointers{ota,OTA library}
 */

/**
 * @brief OTA Error type.
 */
typedef uint32_t                 OTA_Err_t;

#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT */

typedef enum e_fwup_state
{
    FWUP_STATE_INITIALIZING = 1,
    FWUP_STATE_DATA_RECEIVE_START,
    FWUP_STATE_DATA_RECEIVE,
    FWUP_STATE_FLASH_WRITE_WAIT,
    FWUP_STATE_FLASH_WRITE_COMPLETE,
    FWUP_STATE_CHECK_SIGNATURE,
    FWUP_STATE_BANK1_CHECK,
    FWUP_STATE_BANK1_UPDATE_LIFECYCLE_ERASE_WAIT,
    FWUP_STATE_BANK1_UPDATE_LIFECYCLE_ERASE_COMPLETE,
    FWUP_STATE_BANK1_UPDATE_LIFECYCLE_WRITE_WAIT,
    FWUP_STATE_BANK1_UPDATE_LIFECYCLE_WRITE_COMPLETE,
    FWUP_STATE_BANK0_CHECK,
    FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_ERASE_WAIT,
    FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_ERASE_COMPLETE,
    FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_WRITE_WAIT,
    FWUP_STATE_BANK0_INSTALL_SECURE_BOOT_WRITE_COMPLETE,
    FWUP_STATE_BANK0_INSTALL_SET_BOOT_PROTECT,
    FWUP_STATE_INSTALL_DATA_FLASH_ERASE_WAIT,
    FWUP_STATE_INSTALL_DATA_FLASH_ERASE_COMPLETE,
    FWUP_STATE_BANK0_INSTALL_CODE_FLASH_ERASE_WAIT,
    FWUP_STATE_BANK0_INSTALL_CODE_FLASH_ERASE_COMPLETE,
    FWUP_STATE_BANK0_INSTALL_CODE_FLASH_READ_WAIT,
    FWUP_STATE_BANK0_INSTALL_CODE_FLASH_READ_COMPLETE,
    FWUP_STATE_BANK0_INSTALL_CODE_FLASH_WRITE_WAIT,
    FWUP_STATE_BANK0_INSTALL_CODE_FLASH_WRITE_COMPLETE,
    FWUP_STATE_INSTALL_DATA_FLASH_READ_WAIT,
    FWUP_STATE_INSTALL_DATA_FLASH_READ_COMPLETE,
    FWUP_STATE_INSTALL_DATA_FLASH_WRITE_WAIT,
    FWUP_STATE_INSTALL_DATA_FLASH_WRITE_COMPLETE,
    FWUP_STATE_BANK0_UPDATE_CHECK,
    FWUP_STATE_BANK1_UPDATE_CODE_FLASH_ERASE_WAIT,
    FWUP_STATE_BANK1_UPDATE_CODE_FLASH_ERASE_COMPLETE,
    FWUP_STATE_BANK1_UPDATE_DATA_AREA_ERASE,
    FWUP_STATE_BANK1_UPDATE_DATA_AREA_ERASE_WAIT,
    FWUP_STATE_BANK1_UPDATE_DATA_AREA_ERASE_COMPLETE,
    FWUP_STATE_BANK1_UPDATE_DATA_AREA_COPY_WAIT,
    FWUP_STATE_BANK1_UPDATE_DATA_AREA_COPY_COMPLETE,
    FWUP_STATE_BANK1_UPDATE_COPY_FLAG_ERASE,
    FWUP_STATE_BANK1_UPDATE_COPY_FLAG_ERASE_WAIT,
    FWUP_STATE_BANK1_UPDATE_COPY_FLAG_ERASE_COMPLETE,
    FWUP_STATE_BANK1_UPDATE_COPY_FLAG_WRITE_WAIT,
    FWUP_STATE_BANK1_UPDATE_COPY_FLAG_WRITE_COMPLETE,
    FWUP_STATE_EOL_BANK1_ERASE_WAIT,
    FWUP_STATE_EOL_BANK1_ERASE_COMPLETE,
    FWUP_STATE_EOL_BANK1_LIFECYCLE_WRITE_WAIT,
    FWUP_STATE_EOL_BANK1_LIFECYCLE_WRITE_COMPLETE,
    FWUP_STATE_EOL_DATA_FLASH_ERASE_WAIT,
    FWUP_STATE_EOL_DATA_FLASH_ERASE_COMPLETE,
    FWUP_STATE_FINALIZE,
    FWUP_STATE_FATAL_ERROR = 200
} e_fwup_state_t;

typedef enum e_comm_err
{
    COMM_SUCCESS = 0,           // Normally terminated.
    COMM_ERROR,                 // Illegal terminated.
} e_comm_err_t;

typedef enum e_state_monitoring_err
{
    MONI_SUCCESS = 0,           // Normally terminated.
    MONI_ERROR,                 // Illegal terminated.
} e_state_monitoring_err_t;

typedef enum e_state_monitoring_flag
{
    STATE_MONITORING_IN_PROGRESS = 0, STATE_MONITORING_ERROR = -1
} e_state_monitoring_flag_t;

typedef struct st_state_monitoring
{
    uint32_t                  check_status_counter;       // Counter for monitoring state.
    e_state_monitoring_flag_t state_transit_error_flag;   // State transition error flag.
    e_fwup_state_t            last_secure_boot_state;     // Keep last state.
    bool                      state_transit_flag;         // State transition execution flag.
} st_state_monitoring_t;

typedef struct st_sci_buffer_control
{
    uint8_t  buffer[FWUP_WRITE_BLOCK_SIZE];
    uint32_t buffer_occupied_byte_size;
    uint32_t buffer_full_flag;
} st_sci_buffer_control_t;

typedef struct st_sci_receive_control_block
{
    st_sci_buffer_control_t * p_sci_buffer_control;
    uint32_t                total_byte_size;
    uint32_t                current_state;
} st_sci_receive_control_block_t;

typedef struct st_firmware_update_control_block
{
    uint8_t  magic_code[7];
    uint8_t  image_flag;
    uint8_t  signature_type[32];
    uint32_t signature_size;
    uint8_t  signature[256];
    uint32_t dataflash_flag;
    uint32_t dataflash_start_address;
    uint32_t dataflash_end_address;
#if (FWUP_CFG_OTA_DATA_STORAGE == 0)
    uint8_t  reserved1[200];
#else /* FWUP_CFG_OTA_DATA_STORAGE = 1 */
    uint8_t  data_copy_flag;
    uint8_t  reserved1[199];
#endif /* FWUP_CFG_OTA_DATA_STORAGE */
    uint32_t sequence_number;
    uint32_t start_address;
    uint32_t end_address;
    uint32_t execution_address;
    uint32_t hardware_id;
    uint8_t  direct_update_flag;
    uint8_t  reserved2[235];
} st_fwup_control_block_t;

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_BOOTLOADER)
typedef struct st_load_fw_control_block
{
    uint32_t flash_buffer[FWUP_WRITE_BLOCK_SIZE / 4];
    uint32_t offset;
    uint32_t progress;
} st_load_fw_control_block_t;
#else
typedef struct st_load_fw_control_block {
    uint32_t         status;
    uint32_t         processed_byte;
    uint32_t         total_image_length;
    OtaImageState_t  eSavedAgentState;
    OtaFileContext_t * OtaFileContext;
} st_load_fw_control_block_t;
#endif  /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT */

typedef struct st_fragmented_packet
{
    uint32_t offset;
    uint8_t  type;  /* 0: head, 1: tail */
    uint8_t  * p_binary;
    uint32_t length;
} st_fragmented_packet_t;

typedef struct st_fragmented_packet_list
{
    st_fragmented_packet_t           content;
    struct st_fragmented_packet_list * prev;
    struct st_fragmented_packet_list * next;
} st_fragmented_packet_list_t;

typedef struct st_fragmented_search_list
{
    struct st_fragmented_packet_list * list;
    uint32_t                         count;
} st_fragmented_search_list_t;

typedef struct st_flash_block
{
    uint32_t offset;
    uint8_t  * p_binary;
    uint32_t length;
} st_flash_block_t;

typedef struct st_fragmented_block_list
{
    st_flash_block_t                content;
    struct st_fragmented_block_list * next;
} st_fragmented_block_list_t;

typedef struct st_packet_block_for_queue
{
    uint32_t ulOffset;
    uint32_t length;
    uint8_t  p_packet[FLASH_CF_MIN_PGM_SIZE];
} st_packet_block_for_queue_t;

/* extern const char cOTA_JSON_FileSignatureKey[OTA_FILE_SIG_KEY_STR_MAX_LENGTH]; */   /* FIX ME. */
extern bool g_is_opened;

/* Function Name: fwup_flash_open */
flash_err_t fwup_flash_open (void);

/* Function Name: fwup_flash_close */
flash_err_t fwup_flash_close (void);

/* Function Name: fwup_flash_set_callback */
flash_err_t fwup_flash_set_callback (flash_interrupt_config_t * cb_func_info);

#if (FWUP_FLASH_BANK_MODE == 0)
/* Function Name: fwup_flash_get_bank_info */
flash_err_t fwup_flash_get_bank_info (flash_bank_t * bank_info);
#endif /* FWUP_FLASH_BANK_MODE == 0 */

/* Function Name: fwup_flash_set_bank_toggle */
void fwup_flash_set_bank_toggle (void);

/* Function Name: fwup_flash_erase */
flash_err_t fwup_flash_erase (flash_block_address_t block_start_address, uint32_t num_blocks);

/* Function Name: fwup_flash_write */
flash_err_t fwup_flash_write (uint32_t src_address, uint32_t dest_address, uint32_t num_bytes);

/* Function Name: fwup_communication_open */
e_comm_err_t fwup_communication_open (void);

/* Function Name: fwup_communication_close */
e_comm_err_t fwup_communication_close (void);

/* Function Name: fwup_update_status */
void fwup_update_status (e_fwup_state_t state);

/* Function Name: fwup_get_status */
e_fwup_state_t fwup_get_status (void);

/* Function Name: fwup_state_monitoring_open */
e_state_monitoring_err_t fwup_state_monitoring_open (void);

/* Function Name: fwup_state_monitoring_start */
e_state_monitoring_err_t fwup_state_monitoring_start (void);

/* Function Name: fwup_state_monitoring_stop */
e_state_monitoring_err_t fwup_state_monitoring_stop (void);

/* Function Name: fwup_state_monitoring_close */
e_state_monitoring_err_t fwup_state_monitoring_close (void);

/* Function Name: fwup_state_monitoring_is_error */
e_state_monitoring_flag_t fwup_state_monitoring_is_error (void);

/* Function Name: fwup_interrupts_disable */
void fwup_interrupts_disable (void);

/* Function Name: fwup_register_protect_disable */
void fwup_register_protect_disable (void);

/* Function Name: fwup_get_boot_protect */
bool fwup_get_boot_protect (void);

/* Function Name: fwup_set_boot_protect */
flash_err_t fwup_set_boot_protect (void);

/* Function Name: my_sw_charget_function */
/* void my_sw_charget_function (void); */

/* Function Name: my_sw_charput_function */
/* void my_sw_charput_function (uint8_t data); */

#if defined(FWUP_UNIT_TEST) && (FWUP_UNIT_TEST == 1)
/* Function Name: g_setting_unit_test_s_fwup_state */
void g_setting_unit_test_s_fwup_state (e_fwup_state_t stat);
#endif /* FWUP_UNIT_TEST == 1 */

#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_BOOTLOADER)
/***********************************************************************************************************************
 * Function Name: fwup_verification_sha256_ecdsa
 **********************************************************************************************************************/
int32_t fwup_verification_sha256_ecdsa (const uint8_t * pucData, uint32_t ulSize,
                                        const uint8_t * pucSignature, uint32_t ulSignatureSize);

extern void my_sci_callback (void * pArgs);

#if defined(FWUP_UNIT_TEST) && (FWUP_UNIT_TEST == 1)
void g_setting_unit_test_s_flash_error_code (uint32_t val);
void g_setting_unit_test_s_sci_buffer_control (void);
void g_setting_unit_test_s_load_fw_control_block (uint32_t addr);
void g_setting_unit_test_s_load_const_data_control_block (uint32_t addr);
#endif /* FWUP_UNIT_TEST == 1 */
#endif /* FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == FWUP_IMPLEMENTATION_BOOTLOADER */

#endif /* FWUP_PRIVATE_H */
