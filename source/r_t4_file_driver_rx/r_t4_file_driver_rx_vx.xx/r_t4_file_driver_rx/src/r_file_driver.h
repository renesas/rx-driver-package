/*******************************************************************************
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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_file_driver.h
* Description  : This is file header defitination file.
*******************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version Description
*         : 01.12.2014 1.00    First Release
******************************************************************************/
#ifndef R_FILE_HEADER_H
#define R_FILE_HEADER_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_t4_file_driver_rx_if.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
extern int32_t file_open_sub(uint8_t *file_path, uint8_t mode_flag);               // return : -1 == fail, positive == file id
extern int32_t file_close_sub(int32_t file_id);                                    // return : -1 == fail, 0 == success
extern int32_t file_read_sub(int32_t file_id, uint8_t *buf, int32_t read_size);    // return : -1 == fail, positive == read size
extern int32_t file_write_sub(int32_t file_id, uint8_t *buf, int32_t write_size);  // return : -1 == fail, 0 == success
extern int32_t file_delete_sub(uint8_t *file_path);                                // return : -1 == fail, 0 == success
extern int32_t make_dir_sub(uint8_t *dir_path);
extern int32_t remove_dir_sub(uint8_t *dir_path);
extern int32_t file_rename_sub(uint8_t *old_name, uint8_t *new_name);
extern int32_t change_dir_sub(uint8_t *dir_path);
extern int32_t file_exist_sub(uint8_t *file_path);
extern int32_t get_file_size_sub(int32_t file_id);                                 // return : -1 == fail, positive == file size
extern int32_t get_file_list_info_sub(uint8_t *dir_path, FILE_LIST *file_list, uint32_t num_file_list, int32_t read_index);    // return : -1 == fail, positive == number of file
extern int32_t get_file_info_sub(int32_t file_id, DATE_INFO *date_info);           // return : -1 == fail, 0 == success
extern int32_t R_server_file_drv_drive_regist_sub(uint8_t drive);

#endif /* R_FILE_DRIVER_H */
/******************************************************************************
End  Of File
******************************************************************************/
