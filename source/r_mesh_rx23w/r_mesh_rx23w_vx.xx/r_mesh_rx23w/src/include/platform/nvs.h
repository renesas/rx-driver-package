
/**
 *  \file nvs.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_NVS_
#define _H_NVS_

/* --------------------------------------------- Header File Inclusion */
#include "MS_common.h"
#include "storage_pl.h"

/* --------------------------------------------- Global Definitions */
/* NVS Signature Length */
#define NVS_SIGNATURE_SIZE              10

/** NVS Bank types */
#define NVS_BANK_PERSISTENT             0
#define NVS_NUM_BANKS                   1

/** NVS Access modes */
#define NVS_ACCESS_WRITE                0
#define NVS_ACCESS_READ                 1

/* NVS bank states */
#define NVS_CLOSE                       0x00
#define NVS_WROPEN                      0x01
#define NVS_RDOPEN                      0x02

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */
#define NVS_PL_TRC(...)
/* #define NVS_PL_TRC(...)  printf(__VA_ARGS__) */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
UINT16 nvs_init (UINT8 bank);
void nvs_shutdown (UINT8 bank);
void nvs_reset (UINT8 bank);
INT8 nvs_open (UINT8 bank, UINT8 mode, UINT16 offset);
INT8 nvs_close (UINT8 bank);
INT16 nvs_write (UINT8 bank, void * buffer, UINT16 size);
INT16 nvs_read (UINT8 bank, void * buffer, UINT16 size);
INT16 nvs_seek(UINT8 bank, UINT32 offset);

#endif /* _H_NVS_ */

