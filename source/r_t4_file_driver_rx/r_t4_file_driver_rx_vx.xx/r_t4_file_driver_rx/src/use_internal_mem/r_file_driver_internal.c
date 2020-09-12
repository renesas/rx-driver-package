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
* File Name    : r_file_driver_internal.c
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

#if (SERVER_FILE_DRV_USE_INTERNAL==1) && (SERVER_FILE_DRV_USE_EXTERNAL==0)

#include <stdio.h>
#include <string.h>
#include "r_t4_file_driver_rx_if.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define FILE_CLOSED 0
#define FILE_OPENED 1

#define SERVER_CONTENTS_STORE_END_ADR     (SERVER_CONTENTS_STORE_HEAD_ADR + SERVER_CONTENTS_STORE_MEM_SIZE)

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
typedef struct _file_info
{
    int32_t status;
    uint8_t *file_name;
    uint8_t *file_address;
    int32_t file_size;
    int32_t read_pointer;
    int32_t write_pointer;
    uint32_t attr;
}FILE_INFO;

static FILE_INFO finfo[SERVER_MAX_OPEN_FILE] = {0};

/* contents list */
static CONTENTS contents[SERVER_MAX_CONTENTS_LIST] = {SERVER_CONTENTS_LIST};
static uint16_t now_contents_num = SERVER_DEF_CONTENTS;

/* store memory */
static uint32_t store_mem_current_pos = 0; /* MAX:SERVER_CONTENTS_STORE_MEM_SIZE */

static int32_t recode_contents( uint8_t *buf, int32_t size );
static void delete_contents( uint32_t contents_num );

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
    uint16_t index = 0;
    uint32_t i;

    /* search empty file structure */
    for (i = 0; i < SERVER_MAX_OPEN_FILE; i++)
    {
        if (finfo[i].status == FILE_CLOSED)
        {
            finfo[i].status = FILE_OPENED;
            index = i;
            break;
        }
    }
    if (i == SERVER_MAX_OPEN_FILE)
    {
        return -1;
    }

    /* search contents */
    for (i = 0; i < now_contents_num; i++)
    {
        if (!strcmp((char*)file_path, (char*)contents[i].file_name))
        {
            finfo[index].file_name     = (uint8_t*)contents[i].file_name;
            finfo[index].file_address  = contents[i].file_address;
            finfo[index].file_size     = contents[i].file_size;
            finfo[index].write_pointer = contents[i].file_size;
            finfo[index].attr          = contents[i].attr;
            break;
        }
    }

    /* The file name which wasn't in a contents list
       ( write request may come at next time.) */
    if (i == now_contents_num)
    {
//        if (i == 0)
//        {
//            finfo[index].status = FILE_CLOSED;
//            return -1;
//        }
        /* list over */
        if (i > (SERVER_MAX_CONTENTS_LIST - 1) )
        {
            finfo[index].status = FILE_CLOSED;
            return -1;
        }

        /* memory over */
        if (  (SERVER_CONTENTS_STORE_HEAD_ADR != NULL)
            &&(store_mem_current_pos >= SERVER_CONTENTS_STORE_MEM_SIZE ) )
        {
            return -1;
        }

        /* Short file name (8.3 format) */
        if (strlen((char*)file_path) > (1 + 8 + 1 + 3)) /* "/" + "file name" + "." + "extension" */
        {
            return -1;
        }

        strcpy((char*)contents[i].file_name, (char*)file_path);
        contents[i].file_address = (uint8_t*)(SERVER_CONTENTS_STORE_HEAD_ADR
                                              + store_mem_current_pos); /* The current address of memory area */
        contents[i].file_size    = 0;
        contents[i].attr         = FILE_ATTR_ARC;

        finfo[index].file_name     = contents[i].file_name;
        finfo[index].file_address  = contents[i].file_address;
        finfo[index].file_size     = NULL;
        finfo[index].attr          = FILE_ATTR_ARC;
        finfo[index].write_pointer = 0;
        now_contents_num++;
    }

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

    if (finfo[file_id].status == FILE_OPENED)
    {
        finfo[file_id].status = FILE_CLOSED;
        finfo[file_id].read_pointer = 0;
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
    int32_t read_byte = -1;

    if ( read_size < 0 )
    {
        return -1;
    }

    if (finfo[file_id].status == FILE_OPENED)
    {
        if (finfo[file_id].read_pointer < finfo[file_id].file_size)
        {
            memcpy(buf, finfo[file_id].file_address + finfo[file_id].read_pointer, read_size);
            finfo[file_id].read_pointer += read_size;

            /* Actual reading size setting */
            if (finfo[file_id].read_pointer < finfo[file_id].file_size)
            {
                read_byte = read_size;
            }
            else
            {
                read_byte = read_size - (finfo[file_id].read_pointer - finfo[file_id].file_size);
            }
        }
        else
        {
            read_byte = 0;
        }
    }
    return read_byte;
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
    int32_t ret = -1;
    uint32_t i;

    if (finfo[file_id].attr != FILE_ATTR_ARC)
    {
        return -1;
    }

    if (finfo[file_id].status == FILE_OPENED)
    {
        /* search contents */
        for (i = 0; i < now_contents_num; i++)
        {
            if (!strcmp( (char*)finfo[file_id].file_name, (char*)contents[i].file_name))
            {
                /* Overwrite case */
                if (finfo[file_id].write_pointer != 0)
                {
                    /* contents size may change.
                       list reorganization.
                       a now data delete and a new data go back to the end of the line again. */
                    delete_contents(i);

                    /* A contents list is rewritten. */
                    contents[i].file_address = (uint8_t*)(SERVER_CONTENTS_STORE_HEAD_ADR
                                                          + store_mem_current_pos); /* The current address of memory area */
                    contents[i].file_size    = 0;

                    finfo[file_id].file_address  = contents[i].file_address;
                    finfo[file_id].write_pointer = 0;
                }

                if (recode_contents(buf, write_size) != 0)
                {
                    if (contents[i].file_size == 0)
                    {
                        /* file list decrement */
                        now_contents_num--;
                    }
                    return -1;
                }

                contents[i].file_size += write_size;
                finfo[file_id].file_size = contents[i].file_size;
                ret = 0;
                break;
            }
        }
    }

    return ret;
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
    int32_t ret = -1;
    uint32_t i;

    /* search contents */
    for (i = 0; i < now_contents_num; i++)
    {
        if (!strcmp((char*)file_path, (char*)contents[i].file_name))
        {
            if (contents[i].attr == FILE_ATTR_RDO)
            {
                return -1;
            }

            /* memory area delete & update */
            delete_contents(i);

            /* contents list delete & update */
            memcpy( &contents[i], &contents[i+1], ((now_contents_num - i)*sizeof(CONTENTS)) );

            now_contents_num--;
            ret = 0;
            break;
        }
    }

    return ret;
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
    return 0;
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
    int32_t ret = -1;
    uint32_t i, j;

    /* Short file name (8.3 format) */
    if (strlen((char*)new_name) > (1 + 8 + 1 + 3)) /* "/" + "file name" + "." + "extension" */
    {
        return -1;
    }

    /* search contents */
    for (i = 0; i < now_contents_num; i++)
    {
        if (!strcmp((char*)old_name, (char*)contents[i].file_name))
        {
            if (contents[i].attr == FILE_ATTR_RDO)
            {
                return -1;
            }
            /* rename */
            strcpy((char*)contents[i].file_name, (char*)new_name);
            for (j = 0; j < SERVER_MAX_OPEN_FILE; j++)
            {
                if (!strcmp( (char*)finfo[j].file_name, (char*)old_name))
                {
                    finfo[j].file_name = (uint8_t*)contents[i].file_name;
                }
            }
            ret = 0;
        }
    }

    return ret;
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
    if (finfo[file_id].status == FILE_OPENED)
    {
        return finfo[file_id].file_size;
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
                      This function stores �e\0�f to end of structure
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
    uint8_t    tmp_path[FILE_MAX_NAME_SIZE];
    int32_t    i;
    size_t     ln;

    /* delete '/' in end of line */
    tmp_path[0] = '\0';
    strcpy((char *)tmp_path, (const char *)dir_path);
    ln = strlen((char*)tmp_path);
    if (tmp_path[ln-1] == '/')
    {
        tmp_path[ln-1] = '\0';
    }

    /* root directory only */
    if (strlen((char*)tmp_path) != 0)
    {
        return -1;
    }

    if ( 0 > read_index )
    {
        return -1;
    }

    for (i = read_index; i < now_contents_num; i++, file_list++)
    {
        if (contents[i].file_name[0] == 0)
        {
            break;
        }
        strcpy((char*)file_list->file_name, (char*)contents[i].file_name + 1);
        file_list->file_size = contents[i].file_size;
        file_list->file_attr = contents[i].attr;
        get_file_info(0, &file_list->date_info);
    }

    return (i - read_index);
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
                  (0) -
                      Normal completion
******************************************************************************/
int32_t get_file_info_sub(int32_t file_id, DATE_INFO *date_info)
{
    date_info->year = 2012;
    strcpy((char *)date_info->month, "Oct");
    date_info->day = 17;
    strcpy((char *)date_info->day_of_the_week, "Wed");
    date_info->hour = 20;
    date_info->min  = 0;
    date_info->sec  = 0;
    return 0;
}

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
int32_t R_server_file_drv_drive_regist_sub(uint8_t drive)
{
    /* unsupported */
    return -1;
}


/******************************************************************************
Local Function
******************************************************************************/

/******************************************************************************
Function Name   : recode_contents
Description     : the contents data is written in a Memory area.
Arguments       : buf -
                      pointer to contents data
                  size -
                      contents data size
Return Value    : (-1) -
                      Error
                  (0) -
                      Normal completion
******************************************************************************/
static int32_t recode_contents( uint8_t *buf, int32_t size )
{
    if ( (store_mem_current_pos + size) > SERVER_CONTENTS_STORE_MEM_SIZE )
    {
        return -1;
    }
    memcpy( (char*)(SERVER_CONTENTS_STORE_HEAD_ADR + store_mem_current_pos), buf, size );
    store_mem_current_pos += size;

    return 0;
}

/******************************************************************************
Function Name   : delete_contents
Description     : memory area delete and  close up
Arguments       : contents_num - List number
Return Value    : none
******************************************************************************/
static void delete_contents( uint32_t contents_num )
{
    uint32_t i;
    uint32_t delete_size = contents[contents_num].file_size;
    uint8_t* delete_contents_addr = contents[contents_num].file_address;
    uint32_t tmp1,tmp2;

    /* memory area delete & update */
    if ( (SERVER_CONTENTS_STORE_HEAD_ADR <= delete_contents_addr)
            && (delete_contents_addr < SERVER_CONTENTS_STORE_END_ADR) )
    {
        /* memory area delete and close up */
        tmp1 = ((uint32_t)SERVER_CONTENTS_STORE_HEAD_ADR + store_mem_current_pos);
        tmp2 = ((uint32_t)delete_contents_addr + delete_size);
        if( tmp1 < tmp2 )
        {
           return;
        }
        memcpy( delete_contents_addr,
                (delete_contents_addr + delete_size),
                (tmp1 - tmp2));
        store_mem_current_pos -= delete_size;

        /* file address Infomation modify */
        for (i = 0; i < now_contents_num; i++)
        {
            if ( (delete_contents_addr < contents[i].file_address)
                    && (contents[i].file_address < SERVER_CONTENTS_STORE_END_ADR) )
            {
                contents[i].file_address -= delete_size;
            }
        }
    }
}

#endif // (SERVER_FILE_DRV_USE_INTERNAL==1) && (SERVER_FILE_DRV_USE_EXTERNAL==0)
/******************************************************************************
End of file
******************************************************************************/
