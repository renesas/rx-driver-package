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
/***********************************************************************************************************************
 * File Name    : r_fwup_if.h
 * Description  : Functions for using Firmware update.
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           16.02.2021 1.00    First Release
 *           19.05.2021 1.01    Added support for RX72N,RX66T,RX130
 *           08.07.2021 1.02    Added support for RX671 and GCC
 *           10.08.2021 1.03    Added support for IAR
**********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "platform.h"
#include "r_fwup_config.h"  /* Firmware update config definitions */


#ifndef FWUP_IF_H
#define FWUP_IF_H

/***********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/
/* Version Number of API. */
#define FWUP_VERSION_MAJOR  (1)
#define FWUP_VERSION_MINOR  (03)

/*****************************************************************************
 Typedef definitions
 ******************************************************************************/
typedef enum e_fwup_err
{
    FWUP_SUCCESS = 0,               // Normally terminated.
    FWUP_FAIL,                      // Illegal terminated.
    FWUP_IN_PROGRESS,               // Firmware update is in progress.
    FWUP_END_OF_LIFE,               // End Of Life process finised.
    FWUP_ERR_ALREADY_OPEN, // Firmware Update module is in use by another process.
    FWUP_ERR_NOT_OPEN,              // R_FWUP_Open function is not executed yet.
    FWUP_ERR_IMAGE_STATE, // Platform image status not suitable for firmware update.
    FWUP_ERR_LESS_MEMORY,           // Out of memory.
    FWUP_ERR_FLASH,                 // Detect error of r_flash module.
    FWUP_ERR_COMM,                  // Detect error of communication module.
    FWUP_ERR_STATE_MONITORING,      // Detect error of state monitoring module.
} fwup_err_t;

/*****************************************************************************
 Public Functions
 ******************************************************************************/
#if (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == 0)  /* Bootloader */
fwup_err_t R_FWUP_Open(void);
fwup_err_t R_FWUP_Close(void);
int32_t R_FWUP_SecureBoot (void);
void R_FWUP_ExecuteFirmware (void);
#elif (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == 1)  /* Firmware update w/o OS */
fwup_err_t R_FWUP_Open(void);
fwup_err_t R_FWUP_Close(void);
fwup_err_t R_FWUP_Operation(void);
fwup_err_t R_FWUP_SetEndOfLife(void);
void R_FWUP_SoftwareReset(void);
#elif (FWUP_CFG_IMPLEMENTATION_ENVIRONMENT == 2)  /* Firmware update with Amazon FreeRTOS(OTA) */
OTA_Err_t R_FWUP_CreateFileForRx( OTA_FileContext_t * const C );
OTA_Err_t R_FWUP_Abort( OTA_FileContext_t * const C );
int16_t R_FWUP_WriteBlock( OTA_FileContext_t * const C, uint32_t ulOffset,
                            uint8_t * const pacData, uint32_t ulBlockSize );
OTA_Err_t R_FWUP_CloseFile( OTA_FileContext_t * const C );
OTA_Err_t R_FWUP_CheckFileSignature( OTA_FileContext_t * const C );
uint8_t * R_FWUP_ReadAndAssumeCertificate( const uint8_t * const pucCertName, uint32_t * const ulSignerCertSize );
OTA_Err_t R_FWUP_ResetDevice( void );
OTA_Err_t R_FWUP_ActivateNewImage( void );
OTA_Err_t R_FWUP_SetPlatformImageState( OTA_ImageState_t eState );
OTA_PAL_ImageState_t R_FWUP_GetPlatformImageState( void );
#endif
uint32_t R_FWUP_GetVersion(void);

#endif /* FWUP_IF_H */

