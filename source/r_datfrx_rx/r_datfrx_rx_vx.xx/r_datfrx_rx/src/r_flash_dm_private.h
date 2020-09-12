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
* File Name    : r_flash_dm_private.h
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
#include "r_flash_rx_config.h"
#include "r_datfrx_rx_config.h"

#if(FLASH_TYPE == FLASH_TYPE_1)
#if   (FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CF_BLOCK_SIZE == 1024)
#include "r_dispatch_1_cf_1k.h"
#elif (FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CF_BLOCK_SIZE == 2048)
#include "r_dispatch_1_cf_2k.h"
#elif (FLASH_CFG_CODE_FLASH_ENABLE == 0)
#include "r_dispatch_1_df_1k.h"
#endif/* FLASH_CFG_CODE_FLASH_ENABLE and FLASH_DM_CFG_CF_BLOCK_SIZE_2K settings */
#elif(FLASH_TYPE == FLASH_TYPE_2 || FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4)
#include "r_dispatch_2_df.h"
#endif/* FLASH_TYPE */

/************************************************************************************************
Macro definitions
*************************************************************************************************/
#ifndef R_FLASH_DM_PRIVATE_H
#define R_FLASH_DM_PRIVATE_H

/* driver status. */
#define FLASH_DM_INITIALIZED_ID             ((uint8_t)(0xdf))
#define FLASH_DM_FORMATTING_ID              ((uint8_t)(0xf0))

/* API call flag. */
#define FLASH_DM_API_IDLE                   ((int32_t)(false))
#define FLASH_DM_API_CALL                   ((int32_t)(true))

/* Driver status. */
#define FLASH_DM_CHECK_INITIALIZED          ((uint16_t)(0x0001))
#define FLASH_DM_CHECK_ADVANCE              ((uint16_t)(0x0008))
#define FLASH_DM_CHECK_ACT_IDLE             ((uint16_t)(0x0020))
#define FLASH_DM_CHECK_READ                 ((uint16_t)(0x0040))

/* used by r_flash_dm_1st_check() */
#define FLASH_DM_INIT_1ST_CHECK             ((FLASH_DM_CHECK_ADVANCE     | FLASH_DM_CHECK_ACT_IDLE))
#define FLASH_DM_FORMAT_1ST_CHECK           ((FLASH_DM_CHECK_ADVANCE     | FLASH_DM_CHECK_ACT_IDLE))
#define FLASH_DM_WRITE_1ST_CHECK            ((FLASH_DM_CHECK_INITIALIZED | FLASH_DM_CHECK_ADVANCE))
#define FLASH_DM_READ_1ST_CHECK             ((FLASH_DM_CHECK_INITIALIZED \
        | FLASH_DM_CHECK_ADVANCE \
        | FLASH_DM_CHECK_ACT_IDLE \
        | FLASH_DM_CHECK_READ))
#define FLASH_DM_RECLAIM_1ST_CHECK          ((FLASH_DM_CHECK_INITIALIZED | FLASH_DM_CHECK_ADVANCE))
#define FLASH_DM_ERASE_1ST_CHECK            ((FLASH_DM_CHECK_INITIALIZED | FLASH_DM_CHECK_ADVANCE))
#define FLASH_DM_ADVANCE_1ST_CHECK          ((FLASH_DM_CHECK_INITIALIZED))
#define FLASH_DM_WRITABLE_SIZE_1ST_CHECK    ((FLASH_DM_CHECK_INITIALIZED | FLASH_DM_CHECK_ADVANCE))

/************************************************************************************************
Global Typedef definitions
*************************************************************************************************/
#if(FLASH_TYPE == FLASH_TYPE_1)
typedef enum
{
    FLASH_DM_DELAY_MILLISECS = 1000,     /* Requested delay amount is in milliseconds */
    FLASH_DM_DELAY_SECS = 1              /* Requested delay amount is in seconds */
} e_flash_dm_delay_units_t;

#endif /* FLASH_TYPE == FLASH_TYPE_1 */

/************************************************************************************************
Exported global variables
*************************************************************************************************/
extern uint32_t r_flash_dm_drvif_get_datanum(void);
extern uint16_t r_flash_dm_drvif_get_datasize(uint32_t data_no);
e_flash_dm_status_t r_flash_dm_advance(void);
uint8_t          r_flash_dm_drvif_flash_busy(void);
extern const uint16_t gc_dm_data_size[];

/************************************************************************************************
Exported global functions (to be accessed by other files)
*************************************************************************************************/
void r_flash_dm_set_dm(uint32_t handle);
void r_flash_dm_set_dispatch(uint32_t handle);
void FlashBlankCheckDone(uint8_t result);

#if(FLASH_TYPE == FLASH_TYPE_1)

/* r_flash_dm.c */
bool             r_flash_dm_softwaredelay(uint32_t delay, e_flash_dm_delay_units_t units);

/* r_flash_dm_block.c */
uint8_t          r_flash_dm_block_init(void);
void             r_flash_dm_block_program_init(flash_dm_ptr_t src, uint16_t counter, flash_dm_t data);
void             r_flash_dm_block_erase_init(void);
e_flash_dm_state_t r_flash_dm_block_command(st_flash_dispatch_1_hndl_t * p_hndl);
e_flash_dm_state_t r_flash_dm_block_program_check(void);
e_flash_dm_state_t r_flash_dm_block_erase_check(void);

/* r_flash_dm_cf.c or r_flash_dm_df.c */
uint8_t          r_flash_dm_drvif_open(void);
uint8_t          r_flash_dm_drvif_init_device(void);
uint8_t          r_flash_dm_drvif_erase_error(void);
uint8_t          r_flash_dm_drvif_program_error(void);
uint8_t          r_flash_dm_drvif_overwrite_block_flag(flash_dm_ptr_t p_addr);
uint8_t          r_flash_dm_drvif_api_flag(st_flash_dispatch_1_hndl_t * p_hndl);
e_flash_dm_status_t          r_flash_dm_drvif_erase(flash_dm_ptr_t p_addr);
e_flash_dm_status_t          r_flash_dm_drvif_write(flash_dm_ptr_t p_addr, flash_dm_ptr_t p_value);
uint8_t          r_flash_dm_drvif_protect(uint8_t protect);
uint8_t          r_flash_dm_drvif_set_romcache(uint8_t romcache);
uint8_t          r_flash_dm_drvif_get_romcache(uint8_t * p_status);
uint8_t          r_flash_dm_drvif_get_block_num(void);
flash_dm_block_ptr_t  r_flash_dm_drvif_get_block_info(uint8_t index);
flash_dm_ptr_t   r_flash_dm_drvif_get_block_addr(uint8_t index);
void             r_flash_dm_drvif_set_addr_h(st_flash_dispatch_1_hndl_t * p_hndl);
uint32_t         r_flash_dm_drvif_get_addr(flash_dm_dh_ptr_t p_hndl);
void             r_flash_dm_drvif_set_dataaddr(uint8_t data_no, flash_dm_ptr_t p_addr);
flash_dm_ptr_t   r_flash_dm_drvif_get_block_erase_flag(void);
void             r_flash_dm_drvif_set_dh_flag(flash_dm_t * p_flag, flash_dm_t data);

/* r_flash_dm_drvif.c */
void             r_flash_dm_drvif_callback(void * event);

void              r_flash_dm_memcopy(void * p_dest, const void * p_src, uint16_t size);
int16_t           r_flash_dm_free_area(void);
uint8_t           r_flash_dm_init_env(uint8_t init_code, uint8_t act);
e_flash_dm_status_t r_flash_dm_1st_check(uint16_t criteria);

#endif /* FLASH_TYPE == FLASH_TYPE_1 */
#endif /* R_FLASH_DM_PRIVATE_H */

/* End of File */
