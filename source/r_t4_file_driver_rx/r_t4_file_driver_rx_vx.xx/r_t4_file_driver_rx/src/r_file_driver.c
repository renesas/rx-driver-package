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
* File Name    : r_file_driver.c
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
#include "r_file_driver.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/



/******************************************************************************
 Called API from Web server and FTP server
******************************************************************************/

/******************************************************************************
Function Name   : file_open
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
int32_t file_open(uint8_t *file_path, uint8_t mode_flag)
{
    static int32_t ret = -1;

    ret = file_open_sub(file_path, mode_flag);

    return ret;
}

/******************************************************************************
Function Name   : file_open
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
int32_t file_close(int32_t file_id)
{
    int32_t ret = 0;

    ret = file_close_sub(file_id);

    return ret;

}

/******************************************************************************
Function Name   : file_read
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
int32_t file_read(int32_t file_id, uint8_t *buf, int32_t read_size)
{
    int32_t read_byte = 0;

    read_byte = file_read_sub(file_id, buf, read_size);

    return read_byte;
}

/******************************************************************************
Function Name   : file_write
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
int32_t file_write(int32_t file_id, uint8_t *buf, int32_t write_size)
{
    int32_t ret = 0;

    ret = file_write_sub(file_id, buf, write_size);

    return ret;
}

/******************************************************************************
Function Name   : file_delete
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
int32_t file_delete(uint8_t *file_path)
{
    int32_t ret = 0;

    ret = file_delete_sub(file_path);

    return ret;
}

/******************************************************************************
Function Name   : make_dir
Description     : This function makes the directory. The argument is specified
                  in full path from root directory.
Arguments       : dir_path -
                      pointer to file path to make
Return Value    : (-1) -
                      Error
                  (0) -
                      Normal completion
******************************************************************************/
int32_t make_dir(uint8_t *dir_path)
{
    int32_t ret = 0;

    ret = make_dir_sub(dir_path);

    return ret;
}

/******************************************************************************
Function Name   : remove_dir
Description     : This function removes the directory. The argument is
                  specified in full path from root directory.
Arguments       : dir_path -
                      pointer to file path to remove
Return Value    : (-1) -
                      Error
                  (0) -
                      Normal completion
******************************************************************************/
int32_t remove_dir(uint8_t *dir_path)
{
    int32_t ret = 0;

    ret = remove_dir_sub(dir_path);

    return ret;
}

/******************************************************************************
Function Name   : change_dir
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
int32_t change_dir(uint8_t *dir_path)
{
    int32_t ret = 0;

    ret = change_dir_sub(dir_path);

    return ret;
}

/******************************************************************************
Function Name   : file_exist
Description     : This function verifies the file or directory existing.
                  The argument is specified in full path from root directory.
Arguments       : file_path -
                      Pointer to file or directory path
Return Value    : (-1) -
                      Not exist
                  (0) -
                      Exist
******************************************************************************/
int32_t file_exist(uint8_t *file_path)
{
    int32_t ret = 0;

    ret = file_exist_sub(file_path);

    return ret;
}

/******************************************************************************
Function Name   : file_rename
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
int32_t file_rename(uint8_t *old_name, uint8_t *new_name)
{
    int32_t ret = 0;

    ret = file_rename_sub(old_name, new_name);

    return ret;
}

/******************************************************************************
Function Name   : get_file_size
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
int32_t get_file_size(int32_t file_id)
{
    int32_t ret = -1;

    ret = get_file_size_sub(file_id);

    return ret;
}

/******************************************************************************
Function Name   : get_file_list_info
Description     : This function writes the file list stored at the directory
                  path specified as an argument to a file list structure.
Arguments       : dir_path -
                      pointer to directory path to read
                  file_list -
                      pointer to file list to store.
                      This function stores Åe\0Åf to end of structure
                  num_file_list -
                      Max number of file list to read at one time
                  read_index -
                      Index of read starting
Return Value    : (-1) -
                      Error
                  (0) and positive integer -
                      Number of file
******************************************************************************/
int32_t get_file_list_info(uint8_t *dir_path, FILE_LIST *file_list, uint32_t num_file_list, int32_t read_index)
{
    int32_t ret = -1;

    ret = get_file_list_info_sub(dir_path, file_list, num_file_list, read_index);

    return ret;
}

/******************************************************************************
Function Name   : get_file_info
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
int32_t get_file_info(int32_t file_id, DATE_INFO *date_info)
{
    int32_t ret = 0;

    ret = get_file_info_sub(file_id, date_info);

    return ret;
}

/******************************************************************************
 Called API from user application
******************************************************************************/

/******************************************************************************
Function Name   : R_server_file_drv_drive_regist
Description     : drive number registration
Arguments       : drive -
                        0 - VOLUMES : drive number
                        0xff        : drive information reset(drive name isn't added to the path.)
Return Value    : (-1) -
                      Error or unsupported
                  (0) -
                      Normal completion
******************************************************************************/
int32_t R_server_file_drv_drive_regist(uint8_t drive)
{
    int32_t ret = 0;

    ret = R_server_file_drv_drive_regist_sub( drive );

    return ret;
}

/******************************************************************************
End of file
******************************************************************************/
