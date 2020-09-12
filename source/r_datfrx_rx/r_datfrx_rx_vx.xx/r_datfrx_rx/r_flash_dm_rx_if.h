/************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018(2019) Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* File Name    : r_flash_dm_rx_if.h
* Version      : 2.01
* Description  : DATFRX interface header file
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 28.09.2018 2.00     First Release
*              : 25.01.2019 2.01     English PDF added, Fixed blank check processing and Busy check procedure
*************************************************************************************************/

/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
/* Includes board and MCU related header files. */
#include "r_flash_rx_if.h"

/************************************************************************************************
Macro definitions
*************************************************************************************************/
#ifndef R_FLASH_DM_RX_IF_H
#define R_FLASH_DM_RX_IF_H

#ifdef _CDT_PARSER_

#define _RX

#endif

/* Version Number of API. */
#define FLASH_DM_VERSION_MAJOR           (2)
#define FLASH_DM_VERSION_MINOR           (01)

/* Activity flag. */
#define FLASH_DM_ACT_IDLE                   ((uint16_t)(0x00))
#define FLASH_DM_ACT_WRITING                ((uint16_t)(0x01))
#define FLASH_DM_ACT_RECLAIMING             ((uint16_t)(0x02))
#define FLASH_DM_ACT_ERASING                ((uint16_t)(0x04))
#define FLASH_DM_ACT_FORMATTING             ((uint16_t)(0x08))
#define FLASH_DM_ACT_INITIALIZING           ((uint16_t)(0x10))
#define FLASH_DM_ACT_PROTECTING             ((uint16_t)(0x20))

/************************************************************************************************
Global Typedef definitions
*************************************************************************************************/
/* Protect */
typedef enum _dm_ctrl
{
    FLASH_DM_PROTECTION_DISABLE,
    FLASH_DM_PROTECTION_ENABLE,
    FLASH_DM_ROM_CACHE_DISABLE,
    FLASH_DM_ROM_CACHE_ENABLE
} e_dm_ctrl_t;

/* definition of command id */
typedef enum _dm_cmd
{
    DATF_PROGRAM_CMD,
    DATF_ERASE_CMD,
    DATF_BLANK_CHECK_CMD,
    DATF_CLEAR_STATUS_CMD
} e_dm_cmd_t;

/* The block types */
typedef enum _dm_block
{
    DATF_INVALID_BLOCK,
    DATF_ERASE_BLOCK,
    DATF_VALID_BLOCK
} e_dm_block_t;

/* Flash_DM API return codes*/
typedef enum _dm_cb_ret
{
    DATF_API_IDLE,
    DATF_API_CALL,
    DATF_FLASH_IDLE,
    DATF_FLASH_CALL
} e_dm_cb_ret_t; //g_callback_retrun

/* dispatch */
typedef enum _dm_adv
{
    DATF_IDLE_NORMAL,
    DATF_RESTART,
    DATF_TERMINATE,
    DATF_RAISE,
    DATF_CATCH_ERROR,
    DATF_ERASE_CMD_SETTING,
    DATF_PROGRAM_DATA_CMD_SETTING,
    DATF_BLANK_CMD_SETTING,
    DATF_FCLK_CMD_SETTING,
    DATF_COMMAND,
    DATF_ILLEGAL_CHECK,
    DATF_PROGRAM_CHECK,
    DATF_ERASE_CHECK,
    DATF_BLANK_CHECK,
    DATF_ERROR_PROCESS,
    DATF_PROGRAM_NEXT_SETTING,
    DATF_USER_FORMAT,
    DATF_FORMAT_SETTING,
    DATF_FORMAT_NEXT_BLK_SETTING,
    DATF_FORMAT_FINALIZE,
    DATF_FORMAT_FAILURE,
    DATF_USER_WRITE,
    DATF_FORMAT1_PROGRAM_SETUP,
    DATF_FORMAT2_PROGRAM_SETUP,
    DATF_PROGRAM_WRITE_END_FLAG,
    DATF_PROGRAM_NEXT_BLOCK,
    DATF_WRITE_FINALIZE,
    DATF_WRITE_FAILURE,
    DATF_USER_ERASE,
    DATF_PROGRAM_ERASE_START_FLAG,
    DATF_PROGRAM_ERASE_END_FLAG,
    DATF_BLOCK_FLAG_PROGRAM_SETTING,
    DATF_ERASE_FINALIZE,
    DATF_ERASE_FAILURE
} e_dm_adv_t;

typedef enum _dm_blank
{
    DATF_BLANKCHECK_CMD_ERROR,
    DATF_BLANKCHECK_FAIL,
    DATF_BLANKCHECK_PASS,
    DATF_BLANKCHECK_BLANK,
    DATF_BLANKCHECK_NONBLANK
} e_dm_blank_t;

/************************************************************************************************
Typedef definitions
*************************************************************************************************/
/* Enumeration for return values, error_code and advance */
typedef enum _flash_dm_status
{
    FLASH_DM_SUCCESS,
    FLASH_DM_ACCEPT,
    FLASH_DM_SUCCESS_REQUEST_ERASE,
    FLASH_DM_ADVANCE,
    FLASH_DM_FINISH_FORMAT,
    FLASH_DM_FINISH_WRITE,
    FLASH_DM_FINISH_ERASE,
    FLASH_DM_FINISH_RECLAIM,
    FLASH_DM_FINISH_INITIALIZE,
    FLASH_DM_NO_INVALID_BLOCK,
    FLASH_DM_ERR_INIT,
    FLASH_DM_ERR_BUSY,
    FLASH_DM_ERR_ARGUMENT,
    FLASH_DM_ERR_REQUEST_INIT,
    FLASH_DM_ERR_REQUEST_FORMAT,
    FLASH_DM_ERR_REQUEST_ERASE,
    FLASH_DM_ERR_DATA_NOT_PRESENT,
    FLASH_DM_ERR_CANT_RECLAIM,
    FLASH_DM_ERR_REQUEST_RECLAIM,
    FLASH_DM_ERR_FORMAT,
    FLASH_DM_ERR_WRITE,
    FLASH_DM_ERR_ERASE,
    FLASH_DM_ERR_RECLAIM,
    FLASH_DM_ERR_OPEN,
    FLASH_DM_ERR_CLOSE,
    FLASH_DM_ERR
} e_flash_dm_status_t;

typedef enum _flash_dm_cmd
{
    FLASH_DM_GET_WRITABLE_SIZE,
    FLASH_DM_GET_STATUS,
    FLASH_DM_GET_DATA_SIZE,
    FLASH_DM_GET_DATA_NUM,
    FLASH_DM_CF_PROTECT,
    FLASH_DM_ROM_CACHE
} e_flash_dm_cmd_t;

/* Enumeration for return values of state */
typedef enum _flash_dm_state
{
    FLASH_DM_FALSE,
    FLASH_DM_TRUE,
    FLASH_DM_PROGRAM_END,
    FLASH_DM_COMMAND_END,
    FLASH_DM_COMMAND_FALSE,
    FLASH_DM_BUSY,
    FLASH_DM_PROGRAM_ERROR,
    FLASH_DM_PROGRAM_CHECK_END,
    FLASH_DM_ERASE_ERROR,
    FLASH_DM_ERASE_RETRY,
    FLASH_DM_ERASE_END
} e_flash_dm_state_t;

/* Enumeration for return values of block status */
typedef enum _flash_dm_block_status
{
    FLASH_DM_BLOCK_STATUS_ERROR,
    FLASH_DM_BLOCK_STATUS_ERROR_GARBAGE,
    FLASH_DM_BLOCK_STATUS_GARBAGE,
    FLASH_DM_BLOCK_STATUS_READY,
    FLASH_DM_BLOCK_STATUS_ACTIVE,
    FLASH_DM_BLOCK_STATUS_FULL,
    FLASH_DM_BLOCK_STATUS_RECLAIM,
    FLASH_DM_BLOCK_STATUS_UNKNOWN,
    FLASH_DM_BLOCK_STATUS_BAD
} e_flash_dm_block_status_t;


/* Enumeration for return values of driver status */
typedef enum _flash_dm_driver_status
{
    FLASH_DM_IDLE_SETUP,
    FLASH_DM_IDLE_NORMAL,
    FLASH_DM_IDLE_REQ_ERASE,
    FLASH_DM_RESTART,
    FLASH_DM_TERMINATE,
    FLASH_DM_CATCH_ERROR,
    FLASH_DM_RAISE,
    FLASH_DM_STACK_ERROR,
    FLASH_DM_COMMAND,
    FLASH_DM_PROGRAM_CHECK,
    FLASH_DM_ERASE_CHECK,
    FLASH_DM_PROGRAM_BLOCK_ERASE_FLAG,
    FLASH_DM_PROGRAM_BLOCK_ERASE_FLAG_CHECK,
    FLASH_DM_PROGRAM_BLOCK_FLAG,
    FLASH_DM_PROGRAM_BLOCK_FLAG_CHECK,
    FLASH_DM_USER_ERASE_SETUP,
    FLASH_DM_USER_ERASE,
    FLASH_DM_USER_ERASE_FINALIZE_SETUP,
    FLASH_DM_USER_ERASE_FINALIZE,
    FLASH_DM_USER_ERASE_FAILURE,
    FLASH_DM_USER_WRITE_SETUP,
    FLASH_DM_WRITE_SETUP,
    FLASH_DM_WRITE_HEADER_VALIDATE,
    FLASH_DM_WRITE_DATA,
    FLASH_DM_WRITE_DATA_VALIDATE,
    FLASH_DM_WRITE_VALIDATE,
    FLASH_DM_WRITE_VALIDATE_CHECK,
    FLASH_DM_WRITE_FINALIZE,
    FLASH_DM_USER_WRITE_FINALIZE,
    FLASH_DM_USER_WRITE_FAILURE,
    FLASH_DM_USER_RECLAIM_SETUP,
    FLASH_DM_RECLAIM_CREATE_BLOCK,
    FLASH_DM_RECLAIM_COPY_SETUP,
    FLASH_DM_RECLAIM_FINALIZE_SETUP,
    FLASH_DM_USER_RECLAIM_FINALIZE,
    FLASH_DM_USER_RECLAIM_FAILURE,
    FLASH_DM_COPY,
    FLASH_DM_FORMAT_SETUP,
    FLASH_DM_FORMAT_ERASE,
    FLASH_DM_FORMAT_ERASE_FINALIZE_SETUP,
    FLASH_DM_FORMAT_CREATE_BLOCK,
    FLASH_DM_FORMAT_FINALIZE,
    FLASH_DM_FORMAT_FAILURE
} e_flash_dm_driver_status_t;

typedef struct _flash_dm_info
{
    uint8_t     data_no;
    uint8_t     rsv[3];
    uint8_t   * p_data;
} st_flash_dm_info_t;

typedef void (* p_flash_dm_callback)(void *);


/************************************************************************************************
Exported global variables
*************************************************************************************************/

/************************************************************************************************
Exported global functions (to be accessed by other files)
*************************************************************************************************/
e_flash_dm_status_t R_FLASH_DM_Open(uint32_t* p_flash_dm_work, p_flash_dm_callback func);
e_flash_dm_status_t R_FLASH_DM_Close(void);
e_flash_dm_status_t R_FLASH_DM_Init(void);
#if(FLASH_TYPE == FLASH_TYPE_2 || FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4)
e_flash_dm_status_t R_FLASH_DM_InitAdvance(void);
#endif /* FLASH_TYPE == FLASH_TYPE_2 || FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4 */
e_flash_dm_status_t R_FLASH_DM_Format(void);
e_flash_dm_status_t R_FLASH_DM_Read(st_flash_dm_info_t * p_flash_dm_info);
e_flash_dm_status_t R_FLASH_DM_Write(st_flash_dm_info_t * p_flash_dm_info);
e_flash_dm_status_t R_FLASH_DM_Erase(void);
#if(FLASH_TYPE == FLASH_TYPE_1)
e_flash_dm_status_t R_FLASH_DM_Reclaim(void);
#endif /* FLASH_TYPE == FLASH_TYPE_1 */
e_flash_dm_status_t R_FLASH_DM_Control(e_flash_dm_cmd_t cmd, uint32_t* pcfg);
uint32_t R_FLASH_DM_GetVersion(void);

#endif    /* R_FLASH_DM_RX_IF_H */

/* End of File */
