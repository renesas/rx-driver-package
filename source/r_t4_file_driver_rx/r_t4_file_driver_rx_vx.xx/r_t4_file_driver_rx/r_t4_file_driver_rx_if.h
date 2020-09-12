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
* Copyright (C) 2014-2016 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_t4_file_driver_rx_if.h
* Description  : TFAT driver Interface.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 01.12.2014 1.00     First Release
*              : 23.01.2015 1.01     Added RX71M support.
*              : 23.01.2015 1.01     Added RX71M support.
*              : 01.04.2016 1.02     XML file update
*******************************************************************************/
#ifndef R_T4_FILE_DRIVER_RX_IF_H
#define R_T4_FILE_DRIVER_RX_IF_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>

/******************************************************************************
Macro definitions
******************************************************************************/
/* Version Number of API. */
#define T4_FILE_DRIVER_VERSION_MAJOR       (1)
#define T4_FILE_DRIVER_VERSION_MINOR       (02)

#define FILE_WRITE   (0x10)
#define FILE_READ    (0x01)

/* File attribute bits for FILE_LIST->file_attr */
#define FILE_ATTR_RDO    0x01    /* Read only */
#define FILE_ATTR_HID    0x02    /* Hidden */
#define FILE_ATTR_SYS    0x04    /* System */
#define FILE_ATTR_VOL    0x08    /* Volume label */
#define FILE_ATTR_DIR    0x10    /* Directory */
#define FILE_ATTR_ARC    0x20    /* Archive */

#define FILE_MAX_NAME_SIZE    255+1
/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct date_info_
{
    uint16_t year;               // 2011, 2012, ...
    uint8_t  month[4];           // Jan, Feb, Mar, ...
    uint8_t  day;                // 1-31
    uint8_t  day_of_the_week[4]; // Sun, Mon, Tus, ...
    uint16_t hour;               // 0-23
    uint16_t min;                // 0-59
    uint16_t sec;                // 0-59
}DATE_INFO;

typedef struct file_list_
{
    uint8_t file_name[13];
    uint32_t file_size;
    uint32_t file_attr;
    DATE_INFO date_info;
}FILE_LIST;

typedef struct _contents
{
    uint8_t file_name[FILE_MAX_NAME_SIZE];
    uint8_t *file_address;
    int32_t file_size;
    uint32_t attr;
}CONTENTS;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
extern int32_t file_open(uint8_t *file_path, uint8_t mode_flag);               // return : -1 == fail, positive == file id
extern int32_t file_close(int32_t file_id);                                    // return : -1 == fail, 0 == success
extern int32_t file_read(int32_t file_id, uint8_t *buf, int32_t read_size);    // return : -1 == fail, positive == read size
extern int32_t file_write(int32_t file_id, uint8_t *buf, int32_t write_size);  // return : -1 == fail, 0 == success
extern int32_t file_delete(uint8_t *file_path);                                // return : -1 == fail, 0 == success
extern int32_t make_dir(uint8_t *dir_path);
extern int32_t remove_dir(uint8_t *dir_path);
extern int32_t file_rename(uint8_t *old_name, uint8_t *new_name);
extern int32_t change_dir(uint8_t *dir_path);
extern int32_t file_exist(uint8_t *file_path);
extern int32_t get_file_size(int32_t file_id);                                 // return : -1 == fail, positive == file size
extern int32_t get_file_list_info(uint8_t *dir_path, FILE_LIST *file_list, uint32_t num_file_list, int32_t read_index);    // return : -1 == fail, positive == number of file
extern int32_t get_file_info(int32_t file_id, DATE_INFO *date_info);           // return : -1 == fail, 0 == success
extern int32_t R_server_file_drv_drive_regist(uint8_t drive);

#endif /* R_T4_FILE_DRIVER_RX_H */
/******************************************************************************
End  of file
******************************************************************************/
