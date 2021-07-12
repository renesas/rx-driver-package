
/**
 *  \file prov_pl.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_PROV_PL_
#define _H_PROV_PL_

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "MS_common.h"

/*******************************************************************************
* Macro definitions
*******************************************************************************/
/**
 * \addtogroup prov_constants
 * \{
 * \name OOB (Out of Band)
 * \{
 */

/** OOB Public Key Size */
#define PROV_PUBKEY_SIZE_PL                     (64)

/** Static OOB Authentication Size */
#define PROV_AUTHVAL_SIZE_PL                    (16)

/** \} */
/** \} */

/*******************************************************************************
* Prototype declarations
*******************************************************************************/
/** \cond definitions for internal module */
void prov_read_device_oob_pubkey_pl (UCHAR * key, UINT16 size);
void prov_read_static_oob_auth_pl(UCHAR * auth, UINT16 size);
/** \endcond */

/*******************************************************************************
* Macros for Backward Compatibility
*******************************************************************************/
/** \cond backward compatibility */
#define prov_set_device_oob_pubkey_pl           MS_prov_set_device_oob_pubkey_pl
#define prov_set_static_oob_auth_pl             MS_prov_set_static_oob_auth_pl
#define prov_clear_device_oob_pubkey_pl         MS_prov_clear_device_oob_pubkey_pl
#define prov_clear_static_oob_auth_pl           MS_prov_clear_static_oob_auth_pl
/** \endcond */

/*******************************************************************************
* API declarations
*******************************************************************************/
/**
 * \addtogroup prvsng_api_defs
 * \{
 * \name OOB (Out of Band) Configuration
 * \{
 */

/**
 *  \brief Set peer Unprovisioned Device's OOB Public Key.
 *
 *  \par Description
 *  API to set peer Unprovisioned Device's OOB Public Key. This routine is for only Provisioner.
 *
 *  \param [in] key     oob public key
 *  \param [in] size    oob public key size
 *
 *  \note This routine allocates the PROV_PUBKEY_SIZE_PL bytes area dynamically from heap memory to store the OOB Public Key.
 *  If the OOB Public Key is no longer used, free the memory by invoking the MS_prov_clear_device_oob_pubkey_pl().
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_prov_set_device_oob_pubkey_pl(UCHAR * key, UINT16 size);

/**
 *  \brief Clear the OOB Public Key.
 *
 *  \par Description
 *  API to clear the OOB Public Key. Also, this routine frees the area allocated by the MS_prov_set_device_oob_pubkey_pl().
 */
void MS_prov_clear_device_oob_pubkey_pl(void);

/**
 *  \brief Set the Static OOB Authentication.
 *
 *  \par Description
 *  API to set the Static OOB Authentication.
 *
 *  \param [in] auth    static oob authentication
 *  \param [in] size    static oob authentication size
 *
 *  \note This routine allocates the PROV_AUTHVAL_SIZE_PL bytes area dynamically from heap memory to store the OOB Authentication.
 *  If the OOB Authentication is no longer used, free the memory by invoking the MS_prov_clear_static_oob_auth_pl().
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_prov_set_static_oob_auth_pl(UCHAR * auth, UINT16 size);

/**
 *  \brief Clear the Static OOB Authentication.
 *
 *  \par Description
 *  API to clear the Static OOB Authentication. Also, this routine frees the area allocated by the MS_prov_set_static_oob_auth_pl().
 */
void MS_prov_clear_static_oob_auth_pl(void);

/** \} */
/** \} */

#endif /* _H_PROV_PL_ */
