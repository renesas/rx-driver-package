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
* File Name    : r_rsip_ecc.c
* Description  : Interface definition for the r_rsip_ecc RSIP module.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 15.10.2024 1.00     First Release.
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_rsip_protected_rx_if.h"
#include "r_rsip_public.h"
#include "r_rsip_private.h"
#include "../primitive/rx261/r_rsip_primitive.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup RSIP_PROTECTED
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Signs a hashed message. The message hash should be generated in advance.<br>
 * Implements @ref rsip_api_t::ecdsaSign.
 *
 * \<Usage Precautions>
 * @arg The key type of p_wrapped_private_key must be RSIP_KEY_TYPE_ECC_xxx_PRIVATE.
 * @arg This version supports the following key types:
 * <table>
 * <caption id="ecdsaSignLen"> </caption>
 * <tr><th>Key Length<th>Key Type</tr>
 * <tr><td>256 bit<td>RSIP_KEY_TYPE_ECC_SECP256R1_PRIVATE</td>
 * <tr><td>256 bit<td>RSIP_KEY_TYPE_ECC_BRAINPOOLP256R1_PRIVATE</td>
 * <tr><td>256 bit<td>RSIP_KEY_TYPE_ECC_SECP256K1_PRIVATE</td>
 * </table>
 * @arg The hash value must be computed and passed to the argument p_hash before executing this API.<br>
 * In the case of hash length is less than the key length, padding is required to make it the same as the key length.
 *
 * \<Operational State><br>
 * This API can only be executed in the STATE_MAIN, and there are no state transitions.
 *
 * @retval FSP_SUCCESS                           Normal termination.
 * @retval FSP_ERR_ASSERTION                     A required parameter is NULL.
 * @retval FSP_ERR_NOT_OPEN                      Module is not open.
 * @retval FSP_ERR_INVALID_STATE                 Internal state is illegal.
 * @retval FSP_ERR_NOT_ENABLED                   Input key type is disabled in this function by configuration.
 * @retval FSP_ERR_INVALID_ARGUMENT              Input key type or mode is illegal.
 * @retval FSP_ERR_CRYPTO_RSIP_KEY_SET_FAIL      Input key value is illegal.
 * @retval FSP_ERR_CRYPTO_RSIP_FAIL              @arg Input parameter is illegal.
 *                                               @arg Signature generation is failed.
 *
 * @retval FSP_ERR_CRYPTO_RSIP_RESOURCE_CONFLICT A resource conflict occurred because a hardware resource required
 *                                               by the processing is in use by other processing.
 * @retval FSP_ERR_CRYPTO_RSIP_FATAL             Software corruption is detected.
 **********************************************************************************************************************/
fsp_err_t R_RSIP_ECDSA_Sign(rsip_ctrl_t * const              p_ctrl,
                            rsip_wrapped_key_t const * const p_wrapped_private_key,
                            uint8_t const * const            p_hash,
                            uint8_t * const                  p_signature)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;

#if RSIP_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_wrapped_private_key);
    FSP_ASSERT(p_hash);
    FSP_ASSERT(p_signature);
    FSP_ERROR_RETURN(RSIP_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    /* Check key type */
    FSP_ERROR_RETURN(RSIP_ALG_ECC_PRIVATE == p_wrapped_private_key->alg, FSP_ERR_CRYPTO_RSIP_KEY_SET_FAIL);

    /* Check configuration */
    FSP_ERROR_RETURN(g_func.p_ecdsa_sign[p_wrapped_private_key->subtype], FSP_ERR_NOT_ENABLED);
#endif

    /* Check state */
    FSP_ERROR_RETURN(RSIP_STATE_MAIN == p_instance_ctrl->state, FSP_ERR_INVALID_STATE);

    /* Call primitive (cast to match the argument type with the primitive function) */
    rsip_err_t rsip_ret =
        g_func.p_ecdsa_sign[p_wrapped_private_key->subtype]((const uint32_t *) p_wrapped_private_key->value,
                                                            (const uint32_t *) p_hash,
                                                            (uint32_t *) p_signature);

    /* Check error */
    fsp_err_t err = FSP_ERR_CRYPTO_RSIP_FATAL;
    switch (rsip_ret)
    {
        case RSIP_SUCCESS:
        {
            err = FSP_SUCCESS;
            break;
        }

        case RSIP_ERR_RESOURCE_CONFLICT:
        {
            err = FSP_ERR_CRYPTO_RSIP_RESOURCE_CONFLICT;
            break;
        }

        case RSIP_ERR_KEY_SET:
        {
            err = FSP_ERR_CRYPTO_RSIP_KEY_SET_FAIL;
            break;
        }

        case RSIP_ERR_FAIL:
        {
            err = FSP_ERR_CRYPTO_RSIP_FAIL;
            break;
        }

        default:
        {
            err = FSP_ERR_CRYPTO_RSIP_FATAL;
        }
    }

    return err;
}

/*******************************************************************************************************************//**
 * Verifies a hashed message. The message hash should be generated in advance.<br>
 * Implements @ref rsip_api_t::ecdsaVerify.
 *
 * \<Usage Precautions>
 * @arg The key type of p_wrapped_public_key must be RSIP_KEY_TYPE_ECC_xxx_PUBLIC.
 * @arg This version supports the following key types:
 * <table>
 * <caption id="ecdsaVeriLen"> </caption>
 * <tr><th>Key Length<th>Key Type</tr>
 * <tr><td>256 bit<td>RSIP_KEY_TYPE_ECC_SECP256R1_PUBLIC</td>
 * <tr><td>256 bit<td>RSIP_KEY_TYPE_ECC_BRAINPOOLP256R1_PUBLIC</td>
 * <tr><td>256 bit<td>RSIP_KEY_TYPE_ECC_SECP256K1_PUBLIC</td>
 * </table>
 * @arg The hash value must be computed and passed to the argument p_hash before executing this API.<br>
 * In the case of hash length is less than the key length, padding is required to make it the same as the key length.
 *
 * \<Operational State><br>
 * This API can only be executed in the STATE_MAIN, and there are no state transitions.
 *
 * @retval FSP_SUCCESS                           Normal termination.
 * @retval FSP_ERR_ASSERTION                     A required parameter is NULL.
 * @retval FSP_ERR_NOT_OPEN                      Module is not open.
 * @retval FSP_ERR_INVALID_STATE                 Internal state is illegal.
 * @retval FSP_ERR_NOT_ENABLED                   Input key type is disabled in this function by configuration.
 * @retval FSP_ERR_INVALID_ARGUMENT              Input key type or mode is illegal.
 * @retval FSP_ERR_CRYPTO_RSIP_KEY_SET_FAIL      Input key value is illegal.
 * @retval FSP_ERR_CRYPTO_RSIP_FAIL              @arg Input parameter is illegal.
 *                                               @arg Signature verification is failed.
 *
 * @retval FSP_ERR_CRYPTO_RSIP_RESOURCE_CONFLICT A resource conflict occurred because a hardware resource required
 *                                               by the processing is in use by other processing.
 * @retval FSP_ERR_CRYPTO_RSIP_FATAL             Software corruption is detected.
 **********************************************************************************************************************/
fsp_err_t R_RSIP_ECDSA_Verify(rsip_ctrl_t * const                p_ctrl,
                                rsip_wrapped_key_t const * const p_wrapped_public_key,
                                uint8_t const * const            p_hash,
                                uint8_t const * const            p_signature)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;

#if RSIP_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_wrapped_public_key);
    FSP_ASSERT(p_hash);
    FSP_ASSERT(p_signature);
    FSP_ERROR_RETURN(RSIP_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    /* Check key type */
    FSP_ERROR_RETURN(RSIP_ALG_ECC_PUBLIC == p_wrapped_public_key->alg, FSP_ERR_CRYPTO_RSIP_KEY_SET_FAIL);

    /* Check configuration */
    FSP_ERROR_RETURN(g_func.p_ecdsa_verify[p_wrapped_public_key->subtype], FSP_ERR_NOT_ENABLED);
#endif

    /* Check state */
    FSP_ERROR_RETURN(RSIP_STATE_MAIN == p_instance_ctrl->state, FSP_ERR_INVALID_STATE);

    /* Call primitive (cast to match the argument type with the primitive function) */
    rsip_err_t rsip_ret =
        g_func.p_ecdsa_verify[p_wrapped_public_key->subtype]((const uint32_t *) p_wrapped_public_key->value,
                                                            (const uint32_t *) p_hash,
                                                            (const uint32_t *) p_signature);

    /* Check error */
    fsp_err_t err = FSP_ERR_CRYPTO_RSIP_FATAL;
    switch (rsip_ret)
    {
        case RSIP_SUCCESS:
        {
            err = FSP_SUCCESS;
            break;
        }

        case RSIP_ERR_RESOURCE_CONFLICT:
        {
            err = FSP_ERR_CRYPTO_RSIP_RESOURCE_CONFLICT;
            break;
        }

        case RSIP_ERR_KEY_SET:
        {
            err = FSP_ERR_CRYPTO_RSIP_KEY_SET_FAIL;
            break;
        }

        case RSIP_ERR_FAIL:
        {
            err = FSP_ERR_CRYPTO_RSIP_FAIL;
            break;
        }

        default:
        {
            err = FSP_ERR_CRYPTO_RSIP_FATAL;
        }
    }

    return err;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup RSIP_PROTECTED)
 **********************************************************************************************************************/
