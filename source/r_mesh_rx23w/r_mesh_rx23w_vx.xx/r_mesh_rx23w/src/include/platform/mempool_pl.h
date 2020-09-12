/**
 *  \file mempool_pl.h
 */

#ifndef _H_MEMPOOL_
#define _H_MEMPOOL_

#include <stddef.h>
#include "MS_common.h"

#define MEM_POOL_TRC(...)
/* #define MEM_POOL_TRC(...)    cli_printf(__VA_ARGS__) */

void  mempool_init_pl(void* pool, size_t size);
void  mempool_reset_pl(void);
void* mempool_alloc_pl(size_t size);

#endif /* _H_MEMPOOL_ */
