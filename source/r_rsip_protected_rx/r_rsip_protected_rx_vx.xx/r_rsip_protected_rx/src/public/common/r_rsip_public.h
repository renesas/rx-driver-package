/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_rsip_public.h
* Description  : RSIP function public header file.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 15.10.2024 1.00     First Release.
***********************************************************************************************************************/

#ifndef R_RSIP_COMMON_H
#define R_RSIP_COMMON_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdint.h>
#include <string.h>
#include "r_rsip_protected_rx_if.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* "RSIP" in ASCII, used to determine if channel is open. */
#define RSIP_OPEN    (0x52534950U)

/* Various lengths */
/* RSIP_PRV_BYTE_SIZE_AES_BLOCK is defined in r_rsip_protected_rx_if.h */

/* ECC */
#define RSIP_PRV_BYTE_SIZE_ECC_256_PARAM                    (32U)

/* SHA */
#define RSIP_PRV_BYTE_SIZE_DIGEST_SHA224                    (28U)
#define RSIP_PRV_BYTE_SIZE_DIGEST_SHA256                    (32U)
#define RSIP_PRV_BYTE_SIZE_HASH_BLOCK_SHA1_SHA224_SHA256    (64U)

/* RNG */
#define RSIP_PRV_BYTE_SIZE_RNG                              (16U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/

/* r_rsip.c */
fsp_err_t r_rsip_random_number_generate (rsip_ctrl_t * const p_ctrl, uint8_t * const p_random);

/* r_rsip_sha.c */
fsp_err_t r_rsip_sha_init (rsip_ctrl_t * const p_ctrl, rsip_hash_type_t const hash_type);
fsp_err_t r_rsip_sha_update (rsip_ctrl_t * const p_ctrl, uint8_t const * const p_message,
                            uint32_t const message_length);
fsp_err_t r_rsip_sha_finish (rsip_ctrl_t * const p_ctrl, uint8_t * const p_digest);

/*******************************************************************************************************************//**
 * Resets handle.
 *
 * @param[in] handle Pointer to handle.
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE(r_rsip_handle_reset)
void r_rsip_handle_reset(rsip_handle_t * handle)
{
    memset(handle, 0, sizeof(rsip_handle_t));
}

/*******************************************************************************************************************//**
 * Convert to key type to internal algorithm ID.
 *
 * @param[in] key_type Key type.
 *
 * @return Internal algorithm ID.
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE(r_rsip_key_type_to_alg)
rsip_alg_t r_rsip_key_type_to_alg(rsip_key_type_t key_type)
{
    /* cast to rsip_alg_t */
    return (rsip_alg_t) ((key_type >> 8) & 0xff);
}

/*******************************************************************************************************************//**
 * Convert to key type to internal key type (subtype).
 *
 * @param[in] key_type Key type.
 *
 * @return Internal key type.
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE(r_rsip_key_type_to_subtype)
uint8_t r_rsip_key_type_to_subtype(rsip_key_type_t key_type)
{
    /* cast to uint8_t */
    return (uint8_t) (key_type & 0xff);
}

/*******************************************************************************************************************//**
 * Convert to key pair type to internal algorithm ID (public key).
 *
 * @param[in] key_pair_type Key pair type.
 *
 * @return Internal algorithm ID (public key).
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE(r_rsip_key_pair_type_to_public_alg)
rsip_alg_t r_rsip_key_pair_type_to_public_alg(rsip_key_pair_type_t key_pair_type)
{
    /* cast to rsip_alg_t */
    return (rsip_alg_t) ((key_pair_type >> 16) & 0xff);
}

/*******************************************************************************************************************//**
 * Convert to key pair type to internal algorithm ID (private key).
 *
 * @param[in] key_pair_type Key pair type.
 *
 * @return Internal algorithm ID (private key).
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE(r_rsip_key_pair_type_to_private_alg)
rsip_alg_t r_rsip_key_pair_type_to_private_alg(rsip_key_pair_type_t key_pair_type)
{
    /* cast to rsip_alg_t */
    return (rsip_alg_t) ((key_pair_type >> 8) & 0xff);
}

/******************************************************************************************************************//**
 * Convert to key pair type to internal key type (subtype).
 *
 * @param[in] key_pair_type Key pair type.
 *
 * @return Internal key type.
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE(r_rsip_key_pair_type_to_subtype)
uint8_t r_rsip_key_pair_type_to_subtype(rsip_key_pair_type_t key_pair_type)
{
    /* cast to uint8_t */
    return (uint8_t) (key_pair_type & 0xff);
}

#endif                                 /* R_RSIP_COMMON_H */
