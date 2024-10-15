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
* File Name    : r_rsip_sha.c
* Description  : Interface definition for the r_rsip_sha RSIP module.
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

static rsip_err_t sha_update (rsip_ctrl_t * const p_ctrl, const uint8_t * p_message, uint32_t message_length);
static rsip_err_t sha_finish (rsip_ctrl_t * const p_ctrl, uint8_t * p_digest);
static rsip_err_t hmac_update (rsip_ctrl_t * const p_ctrl, const uint8_t * p_message, uint32_t message_length);
static rsip_err_t hmac_sign_finish (rsip_ctrl_t * const p_ctrl, uint8_t * p_mac);
static rsip_err_t hmac_verify_finish (rsip_ctrl_t * const p_ctrl, const uint8_t * p_mac, const uint32_t mac_length);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

static const uint32_t gs_hmac_length[] =
{
    [RSIP_KEY_HMAC_SHA224] = RSIP_PRV_BYTE_SIZE_DIGEST_SHA224,
    [RSIP_KEY_HMAC_SHA256] = RSIP_PRV_BYTE_SIZE_DIGEST_SHA256,
};

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
 * Prepares a SHA generation.<br>
 * Implements @ref rsip_api_t::shaInit.
 *
 * \<Usage Precautions>
 * @arg Hash type must follow the list below.
 * <table>
 * <caption id="shaMessHash2"> </caption>
 * <tr><th>Hash Type<th>Corresponding Parameter</tr>
 * <tr><td>SHA224<td>RSIP_HASH_TYPE_SHA224</td>
 * <tr><td>SHA256<td>RSIP_HASH_TYPE_SHA256</td>
 * </table>
 *
 * \<Operational State><br>
 * This API can only be executed in the STATE_MAIN,
 * and the state after execution changes as follows depending on the return value.
 * <table>
 * <caption id="RetVal13"> </caption>
 * <tr><th>Return Value<th>State after execution</tr>
 * <tr><td>FSP_SUCCESS<td>STATE_SHA</td>
 * <tr><td>Others<td>No change</td>
 * </table>
 *
 * @retval FSP_SUCCESS                           Normal termination.
 * @retval FSP_ERR_ASSERTION                     A required parameter is NULL.
 * @retval FSP_ERR_NOT_OPEN                      Module is not open.
 * @retval FSP_ERR_INVALID_STATE                 Internal state is illegal.
 * @retval FSP_ERR_INVALID_ARGUMENT              Input key type or mode is illegal.
 * @retval FSP_ERR_NOT_ENABLED                   Input key type is disabled in this function by configuration.
**********************************************************************************************************************/
fsp_err_t R_RSIP_SHA_Init(rsip_ctrl_t * const p_ctrl, rsip_hash_type_t const hash_type)
{
#if RSIP_CFG_PARAM_CHECKING_ENABLE
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;

    FSP_ASSERT(p_instance_ctrl);
    FSP_ERROR_RETURN(RSIP_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    /* Check configuration */
    FSP_ERROR_RETURN(g_sha_enabled[hash_type], FSP_ERR_NOT_ENABLED);
#endif

    return r_rsip_sha_init(p_ctrl, hash_type);
}

/*******************************************************************************************************************//**
 * Inputs message.<br>
 * Implements @ref rsip_api_t::shaUpdate.
 *
 * \<Usage Precautions>
 * @arg Requires prerequisite that R_RSIP_SHA_Init, R_RSIP_SHA_Update results in FSP_SUCCESS.
 *
 * \<Operational State><br>
 * This API can only be executed in the STATE_SHA,
 * and the state after execution changes as follows depending on the return value.
 * <table>
 * <caption id="RetVal14"> </caption>
 * <tr><th>Return Value<th>State after execution</tr>
 * <tr><td>Any<td>No change</td>
 * </table>
 *
 * @retval FSP_SUCCESS                           Normal termination.
 * @retval FSP_ERR_ASSERTION                     A required parameter is NULL.
 * @retval FSP_ERR_NOT_OPEN                      Module is not open.
 * @retval FSP_ERR_INVALID_STATE                 Internal state is illegal.
 *
 * @retval FSP_ERR_CRYPTO_RSIP_RESOURCE_CONFLICT A resource conflict occurred because a hardware resource required
 *                                               by the processing is in use by other processing.
 * @retval FSP_ERR_CRYPTO_RSIP_FATAL             Software corruption is detected.
 **********************************************************************************************************************/
fsp_err_t R_RSIP_SHA_Update(rsip_ctrl_t * const p_ctrl, uint8_t const * const p_message, uint32_t const message_length)
{
#if RSIP_CFG_PARAM_CHECKING_ENABLE
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;

    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_message || (0 == message_length));
    FSP_ERROR_RETURN(RSIP_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    return r_rsip_sha_update(p_ctrl, p_message, message_length);
}

/*******************************************************************************************************************//**
 * Finalizes a SHA generation.<br>
 * Implements @ref rsip_api_t::shaFinish.
 *
 * \<Usage Precautions>
 * @arg Requires prerequisite that R_RSIP_SHA_Init, R_RSIP_SHA_Update results in FSP_SUCCESS.
 * @arg The length of message digest is the list below.
 * <table>
 * <caption id="shaMessLen2"> </caption>
 * <tr><th>Hash Type<th>digest length</tr>
 * <tr><td>SHA224<td>28</td>
 * <tr><td>SHA256<td>32</td>
 * </table>
 *
 * \<Operational State><br>
 * This API can only be executed in the STATE_SHA,
 * and the state after execution changes as follows depending on the return value.
 * <table>
 * <caption id="RetVal15"> </caption>
 * <tr><th>Return Value<th>State after execution</tr>
 * <tr><td>FSP_ERR_ASSERTION<br>FSP_ERR_NOT_OPEN<td>No change</td>
 * <tr><td>Others<td>STATE_MAIN</td>
 * </table>
 *
 * @retval FSP_SUCCESS                           Normal termination.
 * @retval FSP_ERR_ASSERTION                     A required parameter is NULL.
 * @retval FSP_ERR_NOT_OPEN                      Module is not open.
 * @retval FSP_ERR_INVALID_STATE                 Internal state is illegal.
 *
 * @retval FSP_ERR_CRYPTO_RSIP_RESOURCE_CONFLICT A resource conflict occurred because a hardware resource required
 *                                               by the processing is in use by other processing.
 * @retval FSP_ERR_CRYPTO_RSIP_FATAL             Software corruption is detected.
 **********************************************************************************************************************/
fsp_err_t R_RSIP_SHA_Finish(rsip_ctrl_t * const p_ctrl, uint8_t * const p_digest)
{
#if RSIP_CFG_PARAM_CHECKING_ENABLE
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;

    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_digest);
    FSP_ERROR_RETURN(RSIP_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    return r_rsip_sha_finish(p_ctrl, p_digest);
}

/*******************************************************************************************************************//**
 * Suspends SHA operation.
 *
 * This API releases RSIP resource and outputs intermediate results. Therefore, it can be used in the following cases:
 * - Execute another cryptographic operations during inputting successive chunks of the message.
 * - Reuse intermediate results.
 *
 * Implements @ref rsip_api_t::shaSuspend.
 *
 * @par State transition
 * @parblock
 * This API can only be executed in **STATE_SHA**, and causes state transition.
 *
 * |Return value|Next state|
 * |------------|----------|
 * |FSP_SUCCESS |STATE_MAIN|
 * |Others      |No change |
 * @endparblock
 *
 * @retval FSP_SUCCESS                           Normal termination.
 * @retval FSP_ERR_ASSERTION                     A required parameter is NULL.
 * @retval FSP_ERR_NOT_OPEN                      Module is not open.
 * @retval FSP_ERR_INVALID_STATE                 Internal state is illegal.
 * @retval FSP_ERR_CRYPTO_RSIP_FATAL             Software corruption is detected.
 **********************************************************************************************************************/
fsp_err_t R_RSIP_SHA_Suspend(rsip_ctrl_t * const p_ctrl, rsip_sha_handle_t * const p_handle)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;
    fsp_err_t              err             = FSP_ERR_CRYPTO_RSIP_FATAL;

#if RSIP_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_handle);
    FSP_ERROR_RETURN(RSIP_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Check state */
    FSP_ERROR_RETURN(RSIP_STATE_SHA == p_instance_ctrl->state, FSP_ERR_INVALID_STATE);

    rsip_err_t rsip_ret;
    switch (p_instance_ctrl->handle.sha.handle_state)
    {
        /* If the state is update, call suspension function */
        case RSIP_USER_HANDLE_STATE_UPDATE:
        {
            /* Handle state transition */
            p_instance_ctrl->handle.sha.handle_state = RSIP_USER_HANDLE_STATE_RESUME;

            rsip_ret = RSIP_SUCCESS;
            break;
        }

        default:
        {
            rsip_ret = RSIP_SUCCESS;
        }
    }

    /* Check error */
    switch (rsip_ret)
    {
        case RSIP_SUCCESS:
        {
            /* Copy handle */
            *p_handle = p_instance_ctrl->handle.sha;

            err = FSP_SUCCESS;
            break;
        }

        default:
        {
            err = FSP_ERR_CRYPTO_RSIP_FATAL;
        }
    }

    /* State transition */
    p_instance_ctrl->state = RSIP_STATE_MAIN;

    return err;
}

/*******************************************************************************************************************//**
 * Resumes SHA operation suspended by R_RSIP_SHA_Suspend().
 *
 * Implements @ref rsip_api_t::shaResume.
 *
 * @par State transition
 * @parblock
 * This API can only be executed in **STATE_MAIN**, and causes state transition.
 *
 * |Return value|Next state|
 * |------------|----------|
 * |FSP_SUCCESS |STATE_SHA |
 * |Others      |No change |
 * @endparblock
 *
 * @retval FSP_SUCCESS                           Normal termination.
 * @retval FSP_ERR_ASSERTION                     A required parameter is NULL.
 * @retval FSP_ERR_NOT_OPEN                      Module is not open.
 * @retval FSP_ERR_INVALID_STATE                 Internal state is illegal.
 **********************************************************************************************************************/
fsp_err_t R_RSIP_SHA_Resume(rsip_ctrl_t * const p_ctrl, rsip_sha_handle_t const * const p_handle)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;

#if RSIP_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_handle);
    FSP_ERROR_RETURN(RSIP_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Check state */
    FSP_ERROR_RETURN(RSIP_STATE_MAIN == p_instance_ctrl->state, FSP_ERR_INVALID_STATE);

    /* Copy handle */
    p_instance_ctrl->handle.sha = *p_handle;

    /* State transition */
    p_instance_ctrl->state = RSIP_STATE_SHA;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Prepares a HMAC generation.<br>
 * Implements @ref rsip_api_t::hmacInit.
 *
 * \<Usage Precautions>
 * @arg The key type of p_wrapped_key must be RSIP_KEY_TYPE_HMAC_xxx.
 *
 * \<Operational State><br>
 * This API can only be executed in the STATE_MAIN,
 * and the state after execution changes as follows depending on the return value.
 * <table>
 * <caption id="RetVal18"> </caption>
 * <tr><th>Return Value<th>State after execution</tr>
 * <tr><td>FSP_SUCCESS<td>STATE_HMAC</td>
 * <tr><td>Others<td>No change</td>
 * </table>
 *
 * @retval FSP_SUCCESS                           Normal termination.
 * @retval FSP_ERR_ASSERTION                     A required parameter is NULL.
 * @retval FSP_ERR_NOT_OPEN                      Module is not open.
 * @retval FSP_ERR_INVALID_STATE                 Internal state is illegal.
 * @retval FSP_ERR_NOT_ENABLED                   Input key type is disabled in this function by configuration.
 * @retval FSP_ERR_CRYPTO_RSIP_KEY_SET_FAIL      Input key value is illegal.
 * @retval FSP_ERR_CRYPTO_RSIP_RESOURCE_CONFLICT A resource conflict occurred because a hardware resource required
 *                                               by the processing is in use by other processing.
 * @retval FSP_ERR_CRYPTO_RSIP_FATAL             Software corruption is detected.
 **********************************************************************************************************************/
fsp_err_t R_RSIP_HMAC_Init(rsip_ctrl_t * const p_ctrl, rsip_wrapped_key_t const * const p_wrapped_key)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;

#if RSIP_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_wrapped_key);
    FSP_ERROR_RETURN(RSIP_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    /* Check key type */
    FSP_ERROR_RETURN(RSIP_ALG_HMAC == p_wrapped_key->alg, FSP_ERR_CRYPTO_RSIP_KEY_SET_FAIL);

    /* Check configuration */
    FSP_ERROR_RETURN(g_hmac_enabled[p_wrapped_key->subtype], FSP_ERR_NOT_ENABLED);
#endif

    /* Check state */
    FSP_ERROR_RETURN(RSIP_STATE_MAIN == p_instance_ctrl->state, FSP_ERR_INVALID_STATE);

    rsip_hmac_handle_t * p_handle = &p_instance_ctrl->handle.hmac;

    /* Reset handle */
    p_handle->buffered_length = 0;
    p_handle->total_length    = 0;

    /* Copy wrapped key */
    memcpy(p_handle->wrapped_key, p_wrapped_key, RSIP_BYTE_SIZE_WRAPPED_KEY_HMAC_SHA256);

    /* Set block size */
    p_handle->block_size = RSIP_PRV_BYTE_SIZE_HASH_BLOCK_SHA1_SHA224_SHA256;

    /* State transition */
    p_instance_ctrl->state = RSIP_STATE_HMAC;
    p_handle->handle_state = RSIP_USER_HANDLE_STATE_INIT;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Inputs message.<br>
 * Implements @ref rsip_api_t::hmacUpdate.
 *
 * \<Usage Precautions>
 * @arg Requires prerequisite that R_RSIP_HMAC_Init, R_RSIP_HMAC_Update results in FSP_SUCCESS.
 *
 * \<Operational State><br>
 * This API can only be executed in the STATE_HMAC,
 * and the state after execution changes as follows depending on the return value.
 * <table>
 * <caption id="RetVal19"> </caption>
 * <tr><th>Return Value<th>State after execution</tr>
 * <tr><td>Any<td>No change</td>
 * </table>
 *
 * @retval FSP_SUCCESS                           Normal termination.
 * @retval FSP_ERR_ASSERTION                     A required parameter is NULL.
 * @retval FSP_ERR_NOT_OPEN                      Module is not open.
 * @retval FSP_ERR_INVALID_STATE                 Internal state is illegal.
 * @retval FSP_ERR_NOT_ENABLED                   Input key type is disabled in this function by configuration.
 * @retval FSP_ERR_INVALID_ARGUMENT              Input key type or mode is illegal.
 * @retval FSP_ERR_CRYPTO_RSIP_KEY_SET_FAIL      Input key value is illegal.
 * @retval FSP_ERR_CRYPTO_RSIP_RESOURCE_CONFLICT A resource conflict occurred because a hardware resource required
 *                                               by the processing is in use by other processing.
 * @retval FSP_ERR_CRYPTO_RSIP_FATAL             Software corruption is detected.
 **********************************************************************************************************************/
fsp_err_t R_RSIP_HMAC_Update(rsip_ctrl_t * const p_ctrl,
                            uint8_t const * const p_message,
                            uint32_t const message_length)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;
    fsp_err_t              err             = FSP_ERR_CRYPTO_RSIP_FATAL;

#if RSIP_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_message || (0 == message_length));
    FSP_ERROR_RETURN(RSIP_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Check state */
    FSP_ERROR_RETURN(RSIP_STATE_HMAC == p_instance_ctrl->state, FSP_ERR_INVALID_STATE);

    rsip_hmac_handle_t * p_handle = &p_instance_ctrl->handle.hmac;

    if (0 == message_length)
    {
        err = FSP_SUCCESS;
    }
    else
    {
        rsip_err_t      rsip_ret      = RSIP_SUCCESS;
        const uint8_t * p_msg_pos     = p_message;
        uint32_t        processed_len = 0;

        /* (1) Remaining message in buffer and head of new input message */
        if ((0 != p_handle->buffered_length) &&
            (p_handle->block_size < (p_handle->buffered_length + message_length)))
        {
            uint32_t len = p_handle->block_size - p_handle->buffered_length;

            /* Copy head of new message to buffer */
            memcpy(p_handle->buffer + p_handle->buffered_length, p_msg_pos, len);

            /* Call function */
            rsip_ret = hmac_update(p_ctrl, p_handle->buffer, p_handle->block_size);

            p_handle->buffered_length = 0;
            p_handle->total_length   += p_handle->block_size;
            processed_len            += len;
        }

        /* (2) New input message except last block */
        if ((p_handle->block_size < message_length) && (RSIP_SUCCESS == rsip_ret))
        {
            uint32_t len = ((message_length - processed_len - 1) / p_handle->block_size) *
                            p_handle->block_size;

            /* Call function */
            rsip_ret = hmac_update(p_ctrl, p_message + processed_len, len);

            p_handle->total_length += len;
            processed_len          += len;
        }

        /* Check error */
        switch (rsip_ret)
        {
            case RSIP_SUCCESS:
            {
                /* (3) Last block */
                memcpy(p_handle->buffer + p_handle->buffered_length,
                        p_message + processed_len,
                        message_length - processed_len);
                p_handle->buffered_length += message_length - processed_len;

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

            /* RSIP_ERR_FAIL is not used in this function */
            case RSIP_ERR_FAIL:
            default:
            {
                err = FSP_ERR_CRYPTO_RSIP_FATAL;
            }
        }
    }

    return err;
}

/*******************************************************************************************************************//**
 * Finalizes a HMAC generation.<br>
 * Implements @ref rsip_api_t::hmacSignFinish.
 *
 * \<Usage Precautions>
 * @arg Requires prerequisite that R_RSIP_HMAC_Init, R_RSIP_HMAC_Update results in FSP_SUCCESS.
 * @arg The length of MAC is the list below.
 * <table>
 * <caption id="MacType3"> </caption>
 * <tr><th>HMAC Type<th>MAC length</tr>
 * <tr><td>HMAC-SHA224<td>28</td>
 * <tr><td>HMAC-SHA256<td>32</td>
 * </table>
 *
 * \<Operational State><br>
 * This API can only be executed in the STATE_HMAC,
 * and the state after execution changes as follows depending on the return value.
 * <table>
 * <caption id="RetVal20"> </caption>
 * <tr><th>Return Value<th>State after execution</tr>
 * <tr><td>FSP_ERR_ASSERTION<br>FSP_ERR_NOT_OPEN<br>FSP_ERR_INVALID_STATE<td>No change</td>
 * <tr><td>Others<td>STATE_MAIN</td>
 * </table>
 *
 * @retval FSP_SUCCESS                           Normal termination.
 * @retval FSP_ERR_ASSERTION                     A required parameter is NULL.
 * @retval FSP_ERR_NOT_OPEN                      Module is not open.
 * @retval FSP_ERR_INVALID_STATE                 Internal state is illegal.
 * @retval FSP_ERR_NOT_ENABLED                   Input key type is disabled in this function by configuration.
 * @retval FSP_ERR_INVALID_ARGUMENT              Input key type or mode is illegal.
 * @retval FSP_ERR_CRYPTO_RSIP_KEY_SET_FAIL      Input key value is illegal.
 * @retval FSP_ERR_CRYPTO_RSIP_RESOURCE_CONFLICT A resource conflict occurred because a hardware resource required
 *                                               by the processing is in use by other processing.
 * @retval FSP_ERR_CRYPTO_RSIP_FATAL             Software corruption is detected.
 **********************************************************************************************************************/
fsp_err_t R_RSIP_HMAC_SignFinish(rsip_ctrl_t * const p_ctrl, uint8_t * const p_mac)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;
    fsp_err_t              err             = FSP_ERR_CRYPTO_RSIP_FATAL;

#if RSIP_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_mac);
    FSP_ERROR_RETURN(RSIP_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Check state */
    FSP_ERROR_RETURN(RSIP_STATE_HMAC == p_instance_ctrl->state, FSP_ERR_INVALID_STATE);

    /* Call function */
    rsip_err_t rsip_ret = hmac_sign_finish(p_ctrl, p_mac);

    /* Check error */
    switch (rsip_ret)
    {
        case RSIP_SUCCESS:
        {
            /* State transition */
            p_instance_ctrl->state = RSIP_STATE_MAIN;

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

        /* RSIP_ERR_FAIL is not used in this function */
        case RSIP_ERR_FAIL:
        default:
        {
            err = FSP_ERR_CRYPTO_RSIP_FATAL;
        }
    }

    return err;
}

/*******************************************************************************************************************//**
 * Finalizes a HMAC verification.<br>
 * Implements @ref rsip_api_t::hmacVerifyFinish.
 *
 * \<Usage Precautions>
 * @arg Requires prerequisite that R_RSIP_HMAC_Init, R_RSIP_HMAC_Update results in FSP_SUCCESS.
 * @arg The length of MAC must follow the list below.
 * <table>
 * <caption id="MacType4"> </caption>
 * <tr><th>HMAC Type<th>MAC length</tr>
 * <tr><td>HMAC-SHA224<td>3 to 28</td>
 * <tr><td>HMAC-SHA256<td>4 to 32</td>
 * </table>
 *
 * \<Operational State><br>
 * This API can only be executed in the STATE_HMAC,
 * and the state after execution changes as follows depending on the return value.
 * <table>
 * <caption id="RetVal21"> </caption>
 * <tr><th>Return Value<th>State after execution</tr>
 * <tr><td>FSP_ERR_ASSERTION<br>FSP_ERR_NOT_OPEN<br>FSP_ERR_INVALID_STATE<br>FSP_ERR_INVALID_SIZE<td>No change</td>
 * <tr><td>Others<td>STATE_MAIN</td>
 * </table>
 *
 * @retval FSP_SUCCESS                           Normal termination.
 * @retval FSP_ERR_ASSERTION                     A required parameter is NULL.
 * @retval FSP_ERR_NOT_OPEN                      Module is not open.
 * @retval FSP_ERR_INVALID_STATE                 Internal state is illegal.
 * @retval FSP_ERR_INVALID_SIZE                  mac_length is illegal.
 * @retval FSP_ERR_CRYPTO_RSIP_KEY_SET_FAIL      Input key value is illegal.
 * @retval FSP_ERR_NOT_ENABLED                   Input key type is disabled in this function by configuration.
 * @retval FSP_ERR_INVALID_ARGUMENT              Input key type or mode is illegal.
 * @retval FSP_ERR_CRYPTO_RSIP_FAIL              MAC verification is failed.
 * @retval FSP_ERR_CRYPTO_RSIP_RESOURCE_CONFLICT A resource conflict occurred because a hardware resource required
 *                                               by the processing is in use by other processing.
 * @retval FSP_ERR_CRYPTO_RSIP_FATAL             Software corruption is detected.
 **********************************************************************************************************************/
fsp_err_t R_RSIP_HMAC_VerifyFinish(rsip_ctrl_t * const p_ctrl, uint8_t const * const p_mac, uint32_t const mac_length)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;
    fsp_err_t              err             = FSP_ERR_CRYPTO_RSIP_FATAL;

#if RSIP_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_mac);
    FSP_ERROR_RETURN(RSIP_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Check state */
    FSP_ERROR_RETURN(RSIP_STATE_HMAC == p_instance_ctrl->state, FSP_ERR_INVALID_STATE);

    /* mac_length must be 4 or greater (common) */
    FSP_ERROR_RETURN(4 <= mac_length, FSP_ERR_INVALID_SIZE);

    rsip_hmac_handle_t * p_handle = &p_instance_ctrl->handle.hmac;

    /* mac_length must be MAC size or less */
    FSP_ERROR_RETURN(mac_length <= gs_hmac_length[((rsip_wrapped_key_t *) &(p_handle->wrapped_key))->subtype],
                    FSP_ERR_INVALID_SIZE);

    /* Call function (cast to match the argument type with the function) */
    rsip_err_t rsip_ret = hmac_verify_finish(p_ctrl, p_mac, mac_length);

    /* Check error */
    switch (rsip_ret)
    {
        case RSIP_SUCCESS:
        {
            /* State transition */
            p_instance_ctrl->state = RSIP_STATE_MAIN;

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

/**********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Prepares a SHA generation.
 *
 * @param[in,out] p_ctrl    Pointer to control block.
 * @param[in]     hash_type Generating hash type.
 *
 * @retval FSP_SUCCESS                           Normal termination.
 * @retval FSP_ERR_INVALID_STATE                 Internal state is illegal.
 **********************************************************************************************************************/
fsp_err_t r_rsip_sha_init(rsip_ctrl_t * const p_ctrl, rsip_hash_type_t const hash_type)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;

    /* Check state */
    FSP_ERROR_RETURN(RSIP_STATE_MAIN == p_instance_ctrl->state, FSP_ERR_INVALID_STATE);

    rsip_sha_handle_t * p_handle = &p_instance_ctrl->handle.sha;

    /* Reset handle */
    p_handle->buffered_length = 0;
    p_handle->total_length    = 0;

    /* Set hash type */
    p_handle->type = hash_type;

    /* Set block size */
    switch (hash_type)
    {
        /* SHA-224 */
        case RSIP_HASH_TYPE_SHA224:
        {
            r_rsip_sha224initprivate(p_handle);
            p_handle->block_size = RSIP_PRV_BYTE_SIZE_HASH_BLOCK_SHA1_SHA224_SHA256;
            break;
        }

        /* SHA-256 */
        case RSIP_HASH_TYPE_SHA256:
        {
            r_rsip_sha256initprivate(p_handle);
            p_handle->block_size = RSIP_PRV_BYTE_SIZE_HASH_BLOCK_SHA1_SHA224_SHA256;
            break;
        }

        default:
        {
            break;
        }
    }

    /* State transition */
    p_instance_ctrl->state = RSIP_STATE_SHA;
    p_handle->handle_state = RSIP_USER_HANDLE_STATE_INIT;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Inputs message.
 *
 * @param[in,out] p_ctrl         Pointer to control block.
 * @param[in]     p_message      Pointer to message. The length is message_length.
 * @param[in]     message_length Byte length of message (0 or more bytes).
 *
 * @retval FSP_SUCCESS                           Normal termination.
 * @retval FSP_ERR_INVALID_STATE                 Internal state is illegal.
 *
 * @retval FSP_ERR_CRYPTO_RSIP_RESOURCE_CONFLICT A resource conflict occurred because a hardware resource required
 *                                               by the processing is in use by other processing.
 * @retval FSP_ERR_CRYPTO_RSIP_FATAL             Software corruption is detected.
 **********************************************************************************************************************/
fsp_err_t r_rsip_sha_update(rsip_ctrl_t * const p_ctrl, uint8_t const * const p_message, uint32_t const message_length)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;
    fsp_err_t              err             = FSP_ERR_CRYPTO_RSIP_FATAL;

    /* Check state */
    FSP_ERROR_RETURN(RSIP_STATE_SHA == p_instance_ctrl->state, FSP_ERR_INVALID_STATE);

    rsip_sha_handle_t * p_handle = &p_instance_ctrl->handle.sha;

    if (0 == message_length)
    {
        err = FSP_SUCCESS;
    }
    else
    {
        rsip_err_t      rsip_ret      = RSIP_SUCCESS;
        const uint8_t * p_msg_pos     = p_message;
        uint32_t        processed_len = 0;

        /* (1) Remaining message in buffer and head of new input message */
        if ((0 != p_handle->buffered_length) &&
            (p_handle->block_size < (p_handle->buffered_length + message_length)))
        {
            uint32_t len = p_handle->block_size - p_handle->buffered_length;

            /* Copy head of new message to buffer */
            memcpy(p_handle->buffer + p_handle->buffered_length, p_msg_pos, len);

            /* Call function */
            rsip_ret = sha_update(p_ctrl, p_handle->buffer, p_handle->block_size);

            p_handle->buffered_length = 0;
            p_handle->total_length   += p_handle->block_size;
            processed_len            += len;
        }

        /* (2) New input message except last block */
        if ((p_handle->block_size < message_length) && (RSIP_SUCCESS == rsip_ret))
        {
            uint32_t len = ((message_length - processed_len - 1) / p_handle->block_size) *
                            p_handle->block_size;

            /* Call function */
            rsip_ret = sha_update(p_ctrl, p_message + processed_len, len);

            p_handle->total_length += len;
            processed_len          += len;
        }

        /* Check error */
        switch (rsip_ret)
        {
            case RSIP_SUCCESS:
            {
                /* (3) Last block */
                memcpy(p_handle->buffer + p_handle->buffered_length,
                        p_message + processed_len,
                        message_length - processed_len);
                p_handle->buffered_length += message_length - processed_len;

                err = FSP_SUCCESS;
                break;
            }

            case RSIP_ERR_RESOURCE_CONFLICT:
            {
                err = FSP_ERR_CRYPTO_RSIP_RESOURCE_CONFLICT;
                break;
            }

            default:
            {
                err = FSP_ERR_CRYPTO_RSIP_FATAL;
            }
        }
    }

    return err;
}

/*******************************************************************************************************************//**
 * Finalizes a SHA generation.
 *
 * @param[in,out] p_ctrl   Pointer to control block.
 * @param[out]    p_digest Pointer to destination of message digest. The length depends on hash type.
 *
 * @retval FSP_SUCCESS                           Normal termination.
 * @retval FSP_ERR_INVALID_STATE                 Internal state is illegal.
 *
 * @retval FSP_ERR_CRYPTO_RSIP_RESOURCE_CONFLICT A resource conflict occurred because a hardware resource required
 *                                               by the processing is in use by other processing.
 * @retval FSP_ERR_CRYPTO_RSIP_FATAL             Software corruption is detected.
 **********************************************************************************************************************/
fsp_err_t r_rsip_sha_finish(rsip_ctrl_t * const p_ctrl, uint8_t * const p_digest)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;
    fsp_err_t              err             = FSP_ERR_CRYPTO_RSIP_FATAL;

    /* Check state */
    FSP_ERROR_RETURN(RSIP_STATE_SHA == p_instance_ctrl->state, FSP_ERR_INVALID_STATE);

    /* Call function */
    rsip_err_t rsip_ret = sha_finish(p_ctrl, p_digest);

    /* Check error */
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

        default:
        {
            err = FSP_ERR_CRYPTO_RSIP_FATAL;
        }
    }

    return err;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

static rsip_err_t sha_update(rsip_ctrl_t * const p_ctrl, const uint8_t * p_message, uint32_t message_length)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;
    rsip_sha_handle_t    * p_handle        = &p_instance_ctrl->handle.sha;

    rsip_err_t ret;
    switch (p_handle->handle_state)
    {
        case RSIP_USER_HANDLE_STATE_INIT:
        {
            ret = r_rsip_sha1sha2_init_update(p_handle->current_hash,
                                            (uint8_t *) p_message, message_length,
                                            p_handle->internal_state);
            break;
        }

        case RSIP_USER_HANDLE_STATE_RESUME:
        case RSIP_USER_HANDLE_STATE_UPDATE:
        default:
        {
            ret = r_rsip_sha1sha2_update(p_handle->current_hash,
                                        (uint8_t *) p_message, message_length,
                                        p_handle->internal_state);
        }
    }

    /* Check error */
    switch (ret)
    {
        case RSIP_SUCCESS:
        {
            /* State transition */
            p_handle->handle_state = RSIP_USER_HANDLE_STATE_UPDATE;
            break;
        }

        default:
        {
            /* State transition (abort) */
            p_instance_ctrl->state = RSIP_STATE_MAIN;
        }
    }

    return ret;
}

static rsip_err_t sha_finish(rsip_ctrl_t * const p_ctrl, uint8_t * p_digest)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;
    rsip_sha_handle_t    * p_handle        = &p_instance_ctrl->handle.sha;
    rsip_err_t             ret;
    switch (p_handle->handle_state)
    {
        case RSIP_USER_HANDLE_STATE_INIT:
        {
            ret = r_rsip_sha1sha2_init_final(p_handle->current_hash, p_handle->buffer,
                                            p_handle->buffered_length, p_digest);
            memcpy(p_handle->current_hash, p_digest, RSIP_PRV_SHA224256_HASH_LENGTH_BYTE_SIZE);
            break;
        }

        case RSIP_USER_HANDLE_STATE_RESUME:
        case RSIP_USER_HANDLE_STATE_UPDATE:
        default:
        {
            ret = r_rsip_sha1sha2_final(p_handle->current_hash,
                                        p_handle->buffer,
                                        p_handle->buffered_length,
                                        p_handle->total_length + p_handle->buffered_length,
                                        p_digest,
                                        p_handle->internal_state);
            memcpy(p_handle->current_hash, p_digest, RSIP_PRV_SHA224256_HASH_LENGTH_BYTE_SIZE);
        }
    }

    /* State transition */
    p_instance_ctrl->state = RSIP_STATE_MAIN;

    return ret;
}

static rsip_err_t hmac_update(rsip_ctrl_t * const p_ctrl, const uint8_t * p_message, uint32_t message_length)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;
    rsip_hmac_handle_t   * p_handle        = &p_instance_ctrl->handle.hmac;

    rsip_err_t ret;
    switch (p_handle->handle_state)
    {
        case RSIP_USER_HANDLE_STATE_INIT:
        {
            ret = r_rsip_hmac_init_update((rsip_wrapped_key_t *) p_handle->wrapped_key,
                                            (uint8_t *) p_message,
                                            message_length,
                                            p_handle->internal_state);
            break;
        }

        case RSIP_USER_HANDLE_STATE_UPDATE:
        default:
        {
            ret = r_rsip_hmac_update((rsip_wrapped_key_t *) p_handle->wrapped_key,
                                        (uint8_t *) p_message,
                                        message_length,
                                        p_handle->internal_state);
        }
    }

    /* Check error */
    switch (ret)
    {
        case RSIP_SUCCESS:
        {
            /* State transition */
            p_handle->handle_state = RSIP_USER_HANDLE_STATE_UPDATE;
            break;
        }

        default:
        {
            /* State transition (abort) */
            p_instance_ctrl->state = RSIP_STATE_MAIN;
        }
    }

    return ret;
}

static rsip_err_t hmac_sign_finish(rsip_ctrl_t * const p_ctrl, uint8_t * p_mac)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;
    rsip_hmac_handle_t   * p_handle        = &p_instance_ctrl->handle.hmac;
    rsip_err_t             ret;
    switch (p_handle->handle_state)
    {
        case RSIP_USER_HANDLE_STATE_INIT:
        {
            ret = r_rsip_hmac_init_final((rsip_wrapped_key_t*) p_handle->wrapped_key,
                                        p_handle->buffer,
                                        p_handle->buffered_length,
                                        p_mac);
            break;
        }

        case RSIP_USER_HANDLE_STATE_UPDATE:
        default:
        {
            ret = r_rsip_hmac_final((rsip_wrapped_key_t *) p_handle->wrapped_key,
                                    p_handle->buffer,
                                    p_handle->buffered_length,
                                    p_handle->total_length + p_handle->buffered_length,
                                    p_mac,
                                    p_handle->internal_state);
        }
    }

    /* State transition */
    p_instance_ctrl->state = RSIP_STATE_MAIN;

    return ret;
}

static rsip_err_t hmac_verify_finish(rsip_ctrl_t * const p_ctrl, const uint8_t * p_mac, const uint32_t mac_length)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;
    rsip_hmac_handle_t   * p_handle        = &p_instance_ctrl->handle.hmac;
    rsip_err_t             ret;
    switch (p_handle->handle_state)
    {
        case RSIP_USER_HANDLE_STATE_INIT:
        {
            ret = r_rsip_hmac_init_verify((rsip_wrapped_key_t *) p_handle->wrapped_key,
                                        p_handle->buffer,
                                        p_handle->buffered_length,
                                        p_mac,
                                        mac_length);
            break;
        }

        case RSIP_USER_HANDLE_STATE_UPDATE:
        default:
        {
            ret = r_rsip_hmac_verify((rsip_wrapped_key_t *) p_handle->wrapped_key,
                                    p_handle->buffer,
                                    p_handle->buffered_length,
                                    p_handle->total_length + p_handle->buffered_length,
                                    p_mac,
                                    mac_length,
                                    p_handle->internal_state);
        }
    }

    /* State transition */
    p_instance_ctrl->state = RSIP_STATE_MAIN;

    return ret;
}
