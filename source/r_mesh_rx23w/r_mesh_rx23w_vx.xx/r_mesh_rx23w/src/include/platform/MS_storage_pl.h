
/**
 *  \file MS_storage_pl.h
 *
 * Platform specific header file for persistant storage module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_MS_STORAGE_PL_
#define _H_MS_STORAGE_PL_

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "MS_common.h"

/*******************************************************************************
* Macro definitions
*******************************************************************************/
/** \cond definitions for internal module */
/** Storage access modes */
#define MS_STORAGE_OPEN_MODE_WRITE              0x00
#define MS_STORAGE_OPEN_MODE_READ               0x01
/** \endcond */

/*******************************************************************************
* Type definitions
*******************************************************************************/
/**
 * \addtogroup ms_common_structures
 * \{
 */

typedef struct
{
    /** Storage Base Address */
    UINT32  base_address;

    /** Storage Size */
    UINT16  size;

    /** Storage Open Function */
    UINT8   (*open)(void);

    /** Storage Close Function */
    void    (*close)(void);

    /** Storage Read Function */
    UINT8   (*read)(UINT32 addr, UINT8* buff, UINT16 len);

    /** Storage Write Function */
    UINT8   (*write)(UINT32 addr, UINT8* buff, UINT16 len);
} MS_STORAGE_PL;

/** \} */

/*******************************************************************************
* Prototype declarations
*******************************************************************************/
/** \cond definitions for internal module */
API_RESULT ms_storage_shutdown_pl(void);
API_RESULT ms_storage_get_base_address_pl(UCHAR** address);
API_RESULT ms_storage_get_size_pl(UINT16* size);
API_RESULT ms_storage_open_pl(UCHAR type, UCHAR mode);
API_RESULT ms_storage_close_pl(UCHAR type, UCHAR mode);
API_RESULT ms_storage_seek_pl(UCHAR type, UCHAR mode, UINT32 offset);
INT16      ms_storage_write_pl(UCHAR type, void * buffer, UINT16 size);
INT16      ms_storage_read_pl(UCHAR type, void * buffer, UINT16 size);
/** \endcond */

/*******************************************************************************
* Macros for Backward Compatibility
*******************************************************************************/
/** \cond backward compatibility */
#define storage_init_pl         MS_storage_register_pl
/** \endcond */

/*******************************************************************************
* API declarations
*******************************************************************************/
/**
 * \addtogroup ms_common_api
 * \{
 */

/**
 *  \brief Initialize Storage Access of Mesh Stack.
 *
 *  \par Description
 *  API to register Platform Storage Access functions with Mesh Stack.
 *  Register the following access functions.
 *    - open
 *    - close
 *    - read
 *    - write
 *
 *  \param [in] storage     platform storage information and functions
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_storage_init_pl(const MS_STORAGE_PL* storage);

/** \} */

#endif /* _H_MS_STORAGE_PL_ */
