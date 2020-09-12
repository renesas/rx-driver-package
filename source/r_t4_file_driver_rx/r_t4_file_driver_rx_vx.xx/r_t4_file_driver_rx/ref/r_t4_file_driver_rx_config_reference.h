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
* File Name    : r_t4_file_driver_rx_config_reference.h
* Description  : Configuration options for the r_t4_ftp_server_rx module.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version Description
*              : 01.12.2014 1.00     First Release
*              : 23.01.2015 1.01     Added RX71M support.
*              : 23.01.2015 1.01     Added RX71M support.
*              : 01.04.2016 1.02     XML file update
*******************************************************************************/
#ifndef R_T4_FILE_DRIVER_RX_CONFIG_REFERENCE_H
#define R_T4_FILE_DRIVER_RX_CONFIG_REFERENCE_H

#include "stdio.h"
#include "platform.h"
#include "r_t4_file_driver_rx_if.h"

/*******************************************************************************
Configuration Options
*******************************************************************************/
/* Only one is effective.
   (priority is given to SERVER_FILE_DRV_USE_EXTERNAL)

   Setting to 1 : used
              0 : unused
*/
#define SERVER_FILE_DRV_USE_EXTERNAL    (0)    // USB, SD card etc
#define SERVER_FILE_DRV_USE_INTERNAL    (0)    // internal-RAM / external-SDRAM / internal-ROM

/* The number of files which manages the open status. */
#define SERVER_MAX_OPEN_FILE    (10)


/* The file system is not using.
   A directory addition isn't specification.
   a file List(contents list) and The written file data is initialized at a restart.
*/
#if SERVER_FILE_DRV_USE_INTERNAL

/*
 ex)
 extern uint8_t test["file size"];    // need setting file size
 extern uint8_t demo["file size"];     // need setting file size
*/
//extern uint8_t ;
/* SERVER_CONTENTS_LIST :
   { '/file name', 'Storage location address of file', 'file size', Attribute}
   (refer to CONTENTS structure )

   <About the attribute of the data stored in internal-ROM>
   Please set to "FILE_ATTR_RDO"

   ex.)
   #define SERVER_CONTENTS_LIST \
       {"/test.jpg", test, sizeof(test), FILE_ATTR_RDO}, \
       {"/demo.htm", demo, sizeof(demo), FILE_ATTR_RDO}, \
*/
/* SERVER_DEF_CONTENTS :
   Please set up the number of the contents in SERVER_CONTENTS_LIST
   Initial value(the number of contents) in a contents list.
 */
#define SERVER_DEF_CONTENTS    (0)
#define SERVER_CONTENTS_LIST \
     
/* Max value(the number of contents) in a contents list.

  Please "SERVER_MAX_CONTENTS_LIST >= SERVER_DEF_CONTENTS".
*/
#define SERVER_MAX_CONTENTS_LIST    (SERVER_DEF_CONTENTS)

/* The memory area which stores the upload data(at FTP server use, etc). */
#define SERVER_CONTENTS_STORE_HEAD_ADR     (NULL)
#define SERVER_CONTENTS_STORE_MEM_SIZE     (0)

#endif /* SERVER_FILE_DRV_USE_INTERNAL */


/* for future expansion
   file system(M3S-TFAT-Tiny) max drive */
#define SERVER_FILE_SYS_MAX_DRV_NUM      (10)

#endif /* R_T4_FILE_DRIVER_RX_CONFIG_REFERENCE_H */
