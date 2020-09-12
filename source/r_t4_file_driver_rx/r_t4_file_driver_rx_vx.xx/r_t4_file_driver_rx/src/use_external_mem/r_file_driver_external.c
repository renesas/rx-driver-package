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
* File Name    : r_file_driver_external.c
* Description  : This is the file driver code.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 01.12.2014 1.00     First Release
*******************************************************************************/

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "r_t4_file_driver_rx_config.h"

#if SERVER_FILE_DRV_USE_EXTERNAL

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ff.h"
#include "r_file_driver.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define MASK_MONTH 0x01e0
#define MASK_DAY   0x001f
#define MASK_MIN   0x07e0
#define MASK_SEC   0x001f

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static uint8_t *conv_month(int32_t file_month);
static uint8_t *conv_day(int32_t tmp_year, int32_t tmp_month, int32_t tmp_day);

static FIL fp[SERVER_MAX_OPEN_FILE] = {0};
static FILINFO finfo[SERVER_MAX_OPEN_FILE] = {0};
static int32_t id[SERVER_MAX_OPEN_FILE] = {0};

static const uint8_t day_data[7][4] = {{"Sun"}, {"Mon"}, {"Tue"}, {"Wed"}, {"Thu"}, {"Fri"}, {"Sat"}};
static const uint8_t month_data[12][4] = {{"Jan"}, {"Feb"}, {"Mar"}, {"Apr"}, {"May"}, {"Jun"}, {"Jul"}, {"Aug"}, {"Sep"}, {"Oct"}, {"Nov"}, {"Dec"}};

#if (SERVER_FILE_SYS_MAX_DRV_NUM > 10)
static char drv_num_char_tfat[3+1] = {0};
#else
static char drv_num_char_tfat[2+1] = {0};
#endif /* SERVER_FILE_SYS_MAX_DRV_NUM > 10 */
static int32_t chg_drive_path_for_tfatmodule(uint8_t *org_path, uint8_t *tfat_path);

/******************************************************************************
Function Name   : file_open_sub
Description     : This function opens the file specified in its argument in
                  exclusive read mode and saves file management information
                  independently. It also specifies an ID value for this file
                  management information as the return value so that the web
                  server can reference the saved file management information by
                  ID. The saved file management information must be saved until
                  this ID value is passed to the file close function.
Arguments       : file_path -
                      input pointer to file path to open
                  mode_flag -
                      input Mode value of file open (FILE_WRITE or FILE_READ)
Return Value    : (-1) -
                      Error
                  (0) and positive integer -
                      The ID value for the opened file
******************************************************************************/
int32_t file_open_sub(uint8_t *file_path, uint8_t mode_flag)
{
    FRESULT frslt;
    uint8_t open_mode;
    static uint16_t index = 0;
    uint8_t tmp_file_path[FILE_MAX_NAME_SIZE];
    uint8_t *p_file_path = tmp_file_path;

    for (index = 0; index < SERVER_MAX_OPEN_FILE; index++)
    {
        if (id[index] == 0)
        {
            id[index] = 1;
            break;
        }
    }
    if (index == SERVER_MAX_OPEN_FILE)
    {
        return -1;
    }
    if (mode_flag == FILE_WRITE)
    {
        open_mode = (FA_WRITE | FA_CREATE_ALWAYS);
    }
    else
    {
        open_mode = (FA_READ);
    }

    if (0 != chg_drive_path_for_tfatmodule(file_path, p_file_path) )
    {
        return -1;
    }

    frslt = f_open(&fp[index], (const char *)p_file_path, open_mode);
    if (frslt != FR_OK)
    {
        id[index] = 0;
        return -1;
    }

    f_stat((const char *)p_file_path, &finfo[index]);

    return index;
}


/******************************************************************************
Function Name   : file_open_sub
Description     : This function closes the file corresponding to the ID
                  specified by the argument and discards the file management
                  information.
Arguments       : file_id -
                      ID value of the file to close
Return Value    : (-1) -
                      Error
                  (0) -
                      Normal completion
******************************************************************************/
int32_t file_close_sub(int32_t file_id)
{
    if (file_id >= SERVER_MAX_OPEN_FILE)
    {
        return -1;
    }

    if (id[file_id] == 1)
    {
        f_close(&fp[file_id]);
        id[file_id] = 0;
    }

    return 0;

}

/******************************************************************************
Function Name   : file_read_sub
Description     : This function reads the file corresponding to the ID value
                  passed as an argument and advances the file pointer by the
                  amount read. The file pointer is recorded in the file
                  management information for each ID value and is maintained
                  until the file close function is called.
Arguments       : file_id -
                      ID value of the file to read
                  buf -
                      Storage area for the file data read
                  read_size -
                      Size of file to read
Return Value    : (-1) -
                      Error
                  (0) and positive integer -
                      Data size of receiving
******************************************************************************/
int32_t file_read_sub(int32_t file_id, uint8_t *buf, int32_t read_size)
{
    FRESULT frslt;
    uint16_t read_byte;

    frslt = f_read(&fp[file_id], buf, (uint16_t)read_size, &read_byte);
    if (frslt != FR_OK)
    {
        return -1;
    }
    return (int32_t)read_byte;
}

/******************************************************************************
Function Name   : file_write_sub
Description     : This function writes the file corresponding to the ID value
                  passed as an argument and advances the file pointer by the
                  amount write. The file pointer is recorded in the file
                  management information for each ID value and is maintained
                  until the file close function is called.
Arguments       : file_id -
                      ID value of the file to write
                  buf -
                      Storage area for the file data write
                  write_size -
                      Size of the file to write
Return Value    : (-1) -
                      Error
                  (0) -
                      Normal completion
******************************************************************************/
int32_t file_write_sub(int32_t file_id, uint8_t *buf, int32_t write_size)
{
    /* this function is not called from sample program */
    FRESULT frslt;
    uint16_t write_byte;
    frslt = f_write(&fp[file_id], buf, (uint16_t)write_size, &write_byte);
    if (frslt != FR_OK)
    {
        return -1;
    }
    if ((uint16_t)write_size != write_byte)
    {
        return -1;
    }
    return 0;
}

/******************************************************************************
Function Name   : file_delete_sub
Description     : This function deletes the file corresponding to the ID
                  specified by the argument. The specification of file is full
                  path from root directory.
Arguments       : file_path -
                      pointer to file path to delete
Return Value    : (-1) -
                      Error
                  (0) -
                      Normal completion
******************************************************************************/
int32_t file_delete_sub(uint8_t *file_path)
{
    FRESULT frslt;
    FILINFO tmpfinfo;
    uint8_t tmp_file_path[FILE_MAX_NAME_SIZE];
    uint8_t *p_file_path = tmp_file_path;

    if (0 != chg_drive_path_for_tfatmodule(file_path, p_file_path) )
    {
        return -1;
    }

    frslt  = f_stat((const char *)p_file_path, &tmpfinfo);
    frslt += f_chmod((const char *)p_file_path, 0, AM_RDO | AM_ARC | AM_SYS | AM_HID);
    frslt += f_unlink((const char *)p_file_path);
    if (frslt != FR_OK)
    {
        frslt += f_chmod((const char *)p_file_path, tmpfinfo.fattrib, AM_RDO | AM_ARC | AM_SYS | AM_HID);
        return -1;
    }
    return 0;
}

/******************************************************************************
Function Name   : make_dir_sub
Description     : This function makes the directory. The argument is specified
                  in full path from root directory.
Arguments       : dir_path -
                      pointer to file path to make
Return Value    : (-1) -
                      Error
                  (0) -
                      Normal completion
******************************************************************************/
int32_t make_dir_sub(uint8_t *dir_path)
{
    FRESULT frslt;
    int32_t ret;
    uint8_t tmp_dir_path[FILE_MAX_NAME_SIZE];
    uint8_t *p_dir_path = tmp_dir_path;

    if (0 != chg_drive_path_for_tfatmodule(dir_path, p_dir_path) )
    {
        return -1;
    }

    frslt = f_mkdir((const char *)p_dir_path);
    switch (frslt)
    {
        case FR_OK:
            ret = 0;
            break;
        case FR_DENIED:
            ret = -1;
            break;
        default:
            ret = -1;
            break;
    }
    return ret;
}

/******************************************************************************
Function Name   : remove_dir_sub
Description     : This function removes the directory. The argument is
                  specified in full path from root directory.
Arguments       : dir_path -
                      pointer to file path to remove
Return Value    : (-1) -
                      Error
                  (0) -
                      Normal completion
******************************************************************************/
int32_t remove_dir_sub(uint8_t *dir_path)
{
    FRESULT frslt;
    uint8_t tmp_dir_path[FILE_MAX_NAME_SIZE];
    uint8_t *p_dir_path = tmp_dir_path;

    if (0 != chg_drive_path_for_tfatmodule(dir_path, p_dir_path) )
    {
        return -1;
    }

    frslt = f_unlink((const char *)p_dir_path);
    if (frslt != FR_OK)
    {
        return -1;
    }
    return 0;
}

/******************************************************************************
Function Name   : change_dir_sub
Description     : This function sets current directory using specified argument.
                  The argument specifies directory path in full path.
                  Information of current directory is managed in each
                  communication endpoint.
Arguments       : dir_path -
                      Pointer to directory path
Return Value    : (-1) -
                      Error
                  (0) -
                      Normal completion
******************************************************************************/
int32_t change_dir_sub(uint8_t *dir_path)
{
    FRESULT frslt;
    DIR     dir;
    uint8_t tmp_dir_path[FILE_MAX_NAME_SIZE];
    uint8_t *p_dir_path = tmp_dir_path;

    if (0 != chg_drive_path_for_tfatmodule(dir_path, p_dir_path) )
    {
        return -1;
    }

    frslt = f_opendir(&dir, (const char *)p_dir_path);
    if (frslt != FR_OK)
    {
        return -1;
    }
    return 0;

}

/******************************************************************************
Function Name   : file_exist_sub
Description     : This function verifies the file or directory existing.
                  The argument is specified in full path from root directory.
Arguments       : file_path -
                      Pointer to file or directory path
Return Value    : (-1) -
                      Not exist
                  (0) -
                      Exist
******************************************************************************/
int32_t file_exist_sub(uint8_t *file_path)
{
    FRESULT frslt;
    FILINFO tmpfinfo;
    uint8_t tmp_file_path[FILE_MAX_NAME_SIZE];
    uint8_t *p_file_path = tmp_file_path;

    if (0 != chg_drive_path_for_tfatmodule(file_path, p_file_path) )
    {
        return -1;
    }

    frslt  = f_stat((const char *)p_file_path, &tmpfinfo);
    if (frslt != FR_OK)
    {
        return -1;
    }
    return 0;
}

/******************************************************************************
Function Name   : file_rename_sub
Description     : This function renames the file specified first argument to
                  second argument. These arguments are specified in full path
                  from root directory.
Arguments       : old_name -
                      pointer to target file name
                  new_name -
                      pointer to after file name
Return Value    : (-1) -
                      Error
                  (0) -
                      Normal completion
******************************************************************************/
int32_t file_rename_sub(uint8_t *old_name, uint8_t *new_name)
{
    FRESULT frslt;
    uint8_t tmp_old_name[FILE_MAX_NAME_SIZE];
    uint8_t tmp_new_name[FILE_MAX_NAME_SIZE];
    uint8_t *p_old_name;
    uint8_t *p_new_name;

    /* Skips first character '/'. */
    old_name++;
    new_name++;
    p_old_name = tmp_old_name;
    p_new_name = tmp_new_name;

    if ( 0 != chg_drive_path_for_tfatmodule( old_name, p_old_name ) )
    {
        return -1;
    }
    if ( 0 != chg_drive_path_for_tfatmodule( new_name, p_new_name ) )
    {
        return -1;
    }

    frslt  = f_rename((const char *)p_old_name, (const char *)p_new_name);
    if (frslt != FR_OK)
    {
        return -1;
    }
    return 0;
}

/******************************************************************************
Function Name   : get_file_size_sub
Description     : This function reads the file management information for the
                  file corresponding to the ID value specified as an argument
                  and returns the file size.
Arguments       : file_id -
                      ID value of the file to read
Return Value    : (-1) -
                      Error
                  (0) and positive integer -
                      File size
******************************************************************************/
int32_t get_file_size_sub(int32_t file_id)
{
    if (id[file_id] == 1)
    {
        return finfo[file_id].fsize;
    }
    return -1;
}

/******************************************************************************
Function Name   : get_file_list_info_sub
Description     : This function writes the file list stored at the directory
                  path specified as an argument to a file list structure.
Arguments       : dir_path -
                      pointer to directory path to read
                  file_list -
                      pointer to file list to store.
                      This function stores '/0' to end of structure
                  num_file_list -
                      Max number of file list to read at one time
                  read_index -
                      Index of read starting
Return Value    : (-1) -
                      Error
                  (0) and positive integer -
                      Number of file
******************************************************************************/
int32_t get_file_list_info_sub(uint8_t *dir_path, FILE_LIST *file_list, uint32_t num_file_list, int32_t read_index)
{
    FRESULT frsrt;
    DIR     dir;
    int32_t i = 0;
    FILINFO file_info;
    uint8_t tmp_path[FILE_MAX_NAME_SIZE];
    int32_t readcount = 0;
    size_t  ln;
    tmp_path[0] = '\0';
    uint8_t tmp_dir_path[FILE_MAX_NAME_SIZE];
    uint8_t *p_dir_path = tmp_dir_path;

    if (0 != chg_drive_path_for_tfatmodule(dir_path, p_dir_path) )
    {
        return -1;
    }

    /* delete '/' in end of line */
    strcpy((char *)tmp_path, (const char *)p_dir_path);
    ln = strlen((char *)tmp_path);
    if (tmp_path[ln-1] == '/')
    {
        tmp_path[ln-1] = '\0';
    }

    frsrt = f_opendir(&dir, (const char *)tmp_path);
    if (frsrt != FR_OK)
    {
        return -1;
    }

    while (readcount < read_index)
    {
        frsrt = f_readdir(&dir, &file_info);
        if (frsrt != FR_OK)
        {
            return -1;
        }

        if (file_info.fname[0] == 0)
        {
            file_list->file_name[0] = 0;
            return 0;
        }
        readcount++;
    }

    while (1)
    {
        if (i == num_file_list)
        {
            break;
        }

        frsrt = f_readdir(&dir, &file_info);
        if (frsrt != FR_OK)
        {
            i = -1;
            break;
        }

        if (file_info.fname[0] != 0)
        {
            strcpy((char *)file_list->file_name, (char *)file_info.fname);
            file_list->file_size = file_info.fsize;
            file_list->file_attr = 0;
            if ((file_info.fattrib & AM_RDO) == AM_RDO)
            {
                /* ReadOnly */
                file_list->file_attr |= FILE_ATTR_RDO;
            }
            if ((file_info.fattrib & AM_HID) == AM_HID)
            {
                /* Hidden */
                file_list->file_attr |= FILE_ATTR_HID;
            }
            if ((file_info.fattrib & AM_SYS) == AM_SYS)
            {
                /* System */
                file_list->file_attr |= FILE_ATTR_SYS;
            }
            if ((file_info.fattrib & AM_DIR) == AM_DIR)
            {
                /* Directory */
                file_list->file_attr |= FILE_ATTR_DIR;
            }
            if ((file_info.fattrib & AM_ARC) == AM_ARC)
            {
                /* Arcive */
                file_list->file_attr |= FILE_ATTR_ARC;
            }
            file_list->date_info.year = ((file_info.fdate >> 9) + 1980);
            strcpy((char *)file_list->date_info.month, (const char *)conv_month((file_info.fdate & MASK_MONTH) >> 5));
            strcpy((char *)file_list->date_info.day_of_the_week, (const char *)conv_day((file_info.fdate >> 9) + 1980, (file_info.fdate & MASK_MONTH) >> 5, file_info.fdate & MASK_DAY));
            file_list->date_info.day = (file_info.fdate & MASK_DAY);
            file_list->date_info.hour = (file_info.ftime >> 11);
            file_list->date_info.min  = ((file_info.ftime & MASK_MIN) >> 5);
            file_list->date_info.sec  = (file_info.ftime & MASK_SEC);
            file_list++;
            i++;
        }
        else
        {
            file_list->file_name[0] = 0;
            break;
        }
    }
    return(i);
}

/******************************************************************************
Function Name   : get_file_info_sub
Description     : This function reads the file management information for the
                  file corresponding to the ID value specified as an argument
                  and writes the file date information to a date information
                  structure.
Arguments       : file_id -
                      ID value of the file to read
                  date_info -
                      pointer to information of date structure to store
Return Value    : (-1) -
                      Error
                  (0) and positive integer -
                      Number of file
******************************************************************************/
int32_t get_file_info_sub(int32_t file_id, DATE_INFO *date_info)
{
    date_info->year = ((finfo[file_id].fdate >> 9) + 1980);
    strcpy((char *)date_info->month, (const char *)conv_month((finfo[file_id].fdate & MASK_MONTH) >> 5));
    date_info->day = finfo[file_id].fdate & MASK_DAY;
    strcpy((char *)date_info->day_of_the_week, (const char *)conv_day((finfo[file_id].fdate >> 9) + 1980, (finfo[file_id].fdate & MASK_MONTH) >> 5, finfo[file_id].fdate & MASK_DAY));
    date_info->hour = (finfo[file_id].ftime >> 11);
    date_info->min  = ((finfo[file_id].ftime & MASK_MIN) >> 5);
    date_info->sec  = (finfo[file_id].ftime & MASK_SEC);
    return 0;
}

static uint8_t *conv_month(int32_t file_month)
{
    return (uint8_t*)month_data[file_month-1];
}


static uint8_t *conv_day(int32_t tmp_year, int32_t tmp_month, int32_t tmp_day)
{
    if ((tmp_month == 1) || (tmp_month == 2))
    {
        tmp_year -= 1;
        tmp_month += 12;
    }
    return (uint8_t*)day_data[((tmp_year + (tmp_year/4) - (tmp_year/100) + (tmp_year/400) + ((13*tmp_month + 8)/5) + tmp_day) % 7)];
}

/******************************************************************************
Function Name   : R_server_file_drv_drive_regist
Description     : drive number registration
Arguments       : drive -
                        0 - VOLUMES  : drive number
                        0xff         : drive information reset(drive name isn't added to the path.)
Return Value    : (-1) -
                      Error
                  (0) -
                      Normal completion
******************************************************************************/
int32_t R_server_file_drv_drive_regist_sub(uint8_t drive)
{
    int32_t ret = 0;

    if ( drive == 0xff )
    {
        drv_num_char_tfat[0] = '\0';
    }
    else if ( drive < SERVER_FILE_SYS_MAX_DRV_NUM )
    {
#if (SERVER_FILE_SYS_MAX_DRV_NUM > 10)
        if ( 9 < drive )
        {
            drv_num_char_tfat[0] = '0' + (drive / 10);
            drv_num_char_tfat[1] = '0' + (drive % 10);
            drv_num_char_tfat[2] = ':';
        }
        else
#endif /* SERVER_FILE_SYS_MAX_DRV_NUM > 10)*/
        {
            drv_num_char_tfat[0] = '0' + drive;
            drv_num_char_tfat[1] = ':';
#if (SERVER_FILE_SYS_MAX_DRV_NUM > 10)
            drv_num_char_tfat[2] = '\0';
#endif /* SERVER_FILE_SYS_MAX_DRV_NUM > 10)*/
        }
    }
    else
    {
        ret = -1;
    }

    return ret;
}

/******************************************************************************
Local Function
******************************************************************************/

/******************************************************************************
Function Name   :chg_drive_path_for_tfatmodule
Description     :convert drive path to tfat spec.
Arguments       : org_path -
                      pointer to path to read
                  tfat_path -
                      pointer to path to read
Return Value    : (-1) -
                      Error
                  (0) -
                      Normal completion
******************************************************************************/
static int32_t chg_drive_path_for_tfatmodule(uint8_t *org_path, uint8_t *tfat_path)
{
    if ( drv_num_char_tfat[0] != '\0' )
    {
        strcpy( (char *)tfat_path, (const char *)&drv_num_char_tfat );
        strcat( (char *)tfat_path, (const char *)(org_path + 1) );
    }
    else
    {
        strcpy( (char *)tfat_path, (const char *)org_path );
    }

    return 0;
}

#endif /* SERVER_FILE_DRV_USE_EXTERNAL */
/******************************************************************************
End of file
******************************************************************************/
