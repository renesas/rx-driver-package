/**
 *  \file MS_mempool_pl.h
 */

#ifndef _H_MS_MEMPOOL_PL_
#define _H_MS_MEMPOOL_PL_

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stddef.h>
#include "MS_common.h"

/*******************************************************************************
* Macro definitions
*******************************************************************************/
/** \cond definitions for internal module */
#define MEM_POOL_TRC(...)
/* #define MEM_POOL_TRC(...)    cli_printf(__VA_ARGS__) */
/** \endcond */

/*******************************************************************************
* Prototype declarations
*******************************************************************************/
/** \cond definitions for internal module */
void  ms_mempool_reset_pl(void);
void* ms_mempool_alloc_pl(size_t size);
void  ms_mempool_free_pl(void* ptr);
/** \endcond */

/*******************************************************************************
* Macros for Backward Compatibility
*******************************************************************************/
/** \cond backward compatibility */
#define mempool_init_pl         MS_mempool_init_pl
/** \endcond */

/*******************************************************************************
* API declarations
*******************************************************************************/
/**
 * \addtogroup ms_common_api
 * \{
 */

/**
 *  \brief Initialize Memory Pool of Mesh Stack.
 *
 *  \par Description
 *  API to register Memory Pool with Mesh Stack. The memory pool is used for various
 *  cache which size is configured by the configurations specified by the MS_init().
 *
 *  \param [in] pool    memory pool
 *  \param [in] size    memory pool size
 */
void  MS_mempool_init_pl(void* pool, size_t size);

/** \} */

#endif /* _H_MS_MEMPOOL_PL_ */
