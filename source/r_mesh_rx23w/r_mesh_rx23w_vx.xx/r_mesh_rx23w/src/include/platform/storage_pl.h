
/**
 *  \file storage_pl.h
 *
 * Platform specific header file for persistant storage module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_STORAGE_PL_
#define _H_STORAGE_PL_

/* --------------------------------------------- Header File Inclusion */
#include "MS_common.h"

/* --------------------------------------------- Global Definitions */
/* Storage access modes */
#define STORAGE_OPEN_MODE_WRITE                  0x00
#define STORAGE_OPEN_MODE_READ                   0x01

/* --------------------------------------------- Structures/Data Types */
typedef struct
{
    /* Information on the storage */
    UINT32  base_address;
    UINT16  size;

    /* Random Access APIs */
    UINT8   (*open )(void);
    void    (*close)(void);
    UINT8   (*read )(uint32_t addr, uint8_t* buff, uint16_t len);
    UINT8   (*write)(uint32_t addr, uint8_t* buff, uint16_t len);
} storage_t;

/* --------------------------------------------- Macros */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
API_RESULT storage_init_pl(const storage_t* storage);
API_RESULT storage_shutdown_pl(void);
API_RESULT storage_get_base_address_pl(UCHAR** address);
API_RESULT storage_get_size_pl(UINT16* size);
API_RESULT storage_open_pl(UCHAR type, UCHAR mode);
API_RESULT storage_close_pl(UCHAR type, UCHAR mode);
API_RESULT storage_seek_pl(UCHAR type, UCHAR mode, UINT32 offset);
INT16      storage_write_pl(UCHAR type, void * buffer, UINT16 size);
INT16      storage_read_pl(UCHAR type, void * buffer, UINT16 size);

#endif /* _H_STORAGE_PL_ */
