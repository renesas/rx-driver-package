
/**
 *  \file MS_prov_pl.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_MS_PROV_PL_
#define _H_MS_PROV_PL_

/***********************************************************************************************************************
* Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "MS_common.h"
#include "MS_prov_api.h"

/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/
/**
 * \addtogroup prov_constants
 * \{
 * \name OOB (Out of Band) Information
 * \{
 */

/** \} */
/** \} */

/***********************************************************************************************************************
* Macros for Backward Compatibility
***********************************************************************************************************************/
/** \cond backward compatibility */
#define PROV_PUBKEY_SIZE_PL                     PROV_PUBKEY_SIZE
#define PROV_AUTHVAL_SIZE_PL                    PROV_AUTHVAL_SIZE
#define prov_set_device_oob_pubkey_pl           MS_prov_set_device_oob_pubkey_pl
#define prov_set_static_oob_auth_pl             MS_prov_set_static_oob_auth_pl
#define prov_clear_device_oob_pubkey_pl         MS_prov_clear_device_oob_pubkey_pl
#define prov_clear_static_oob_auth_pl           MS_prov_clear_static_oob_auth_pl
/** \endcond */

/***********************************************************************************************************************
* API declarations
***********************************************************************************************************************/
/**
 * \addtogroup prov_api_defs
 * \{
 */

/**
 * \name ECDH Key Generation
 * \{
 */

/**
 *  \brief API to generate Public Key and Private Key for Provisioning.
 *
 *  \par Description
 *  THis API can be used by the Provisionee application to generate Public Key and Private Key from
 *  FIPS P-256 Elliptic Curve Algorithm.
 *  Generated Public Key is copied to the argument "public_key".
 *  Generated Private Key is stored in the Mesh Stack automatically.
 *
 *  \param [in] public_key  P-256 ECDH Public Key
 *
 *  \note This API is for the Provisionee (Unprovisioned Device) application only.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_prov_generate_ecdh_key_pl(UINT8 * public_key);

/** \} */

/**
 * \name OOB Public Key
 * \{
 */

/**
 *  \brief API to set OOB (Out-Of-Band) Public Key from the Provisionee (Unprovisioned Device).
 *
 *  \par Description
 *  This API can be used by the Provisioner application to set OOB Public Key informed from the Provisionee
 *  (Unprovisioned Device).
 *
 *  \param [in] key     OOB P-256 ECDH Public Key
 *  \param [in] size    size of OOB P-256 ECDH Public Key
 *                      \ref PROV_PUBKEY_SIZE
 *
 *  \note This API is for the Provisioner application only.
 *
 *  \note This routine allocates the PROV_PUBKEY_SIZE bytes area dynamically from heap memory to store the Public Key.
 *  If the Public Key is no longer used, free the memory by invoking the MS_prov_clear_device_oob_pubkey_pl().
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_prov_set_device_oob_pubkey_pl(UCHAR * key, UINT16 size);

/**
 *  \brief Clear the OOB Public Key.
 *
 *  \par Description
 *  API to clear the OOB Public Key. Also, this routine frees the area allocated by
 *  the MS_prov_set_device_oob_pubkey_pl().
 *
 *  \note This API is for the Provisioner application only.
 */
void MS_prov_clear_device_oob_pubkey_pl(void);

/** \} */

/**
 * \name Static OOB Authentication
 * \{
 */

/**
 *  \brief API to set Static OOB (Out-Of-Band) Authentication value from the Provisionee (Unprovisioned Device).
 *
 *  \par Description
 *  This API can be used by the Provisioner application to set Static OOB Authentication Value informed from the
 *  Provisionee (Unprovisioned Device).
 *
 *  \param [in] auth    Static OOB Authentication value
 *  \param [in] size    size of Static OOB Authentication value \ref PROV_AUTHVAL_SIZE
 *
 *  \note This API is for the Provisioner application only.
 *
 *  \note This routine allocates the PROV_AUTHVAL_SIZE bytes area dynamically from heap memory
 *  to store the OOB Authentication. If the OOB Authentication is no longer used, free the memory
 *  by invoking the MS_prov_clear_static_oob_auth_pl().
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_prov_set_static_oob_auth_pl(UCHAR * auth, UINT16 size);

/**
 *  \brief Clear the Static OOB Authentication.
 *
 *  \par Description
 *  API to clear the Static OOB Authentication. Also, this routine frees the area allocated by
 *  the MS_prov_set_static_oob_auth_pl().
 *
 *  \note This API is for the Provisioner application only.
 */
void MS_prov_clear_static_oob_auth_pl(void);

/** \} */
/** \} */

#endif /* _H_MS_PROV_PL_ */
