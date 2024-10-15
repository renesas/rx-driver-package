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
* File Name    : r_rsip_fwup.c
* Description  : Interface definition for the r_rsip_fwup RSIP module.
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
RSIP_SEC_P_SECURE_BOOT
/*******************************************************************************************************************//**
 * Start Firmware Update state.<br>
 * Implements @ref rsip_api_t::startupdatefirmware.
 *
 * \<Operational State><br>
 * This API can only be executed in the STATE_INITIAL,
 * and the state after execution changes as follows depending on the return value.
 * <table>
 * <caption id="RetVal1"> </caption>
 * <tr><th>Return Value<th>State after execution</tr>
 * <tr><td>FSP_SUCCESS<td>STATE_FWUP</td>
 * <tr><td>Others<td>No change</td>
 * </table>
 *
 * @retval FSP_SUCCESS                           Normal termination.
 * @retval FSP_ERR_ASSERTION                     A required parameter is NULL.
 * @retval FSP_ERR_NOT_OPEN                      Module is not open.
 * @retval FSP_ERR_INVALID_STATE                 Internal state is illegal.
 * @retval FSP_ERR_CRYPTO_RSIP_FAIL              Hardware initialization is failed.
 * @retval FSP_ERR_CRYPTO_RSIP_RESOURCE_CONFLICT A resource conflict occurred because a hardware resource required
 *                                               by the processing is in use by other processing.
 * @retval FSP_ERR_CRYPTO_RSIP_FATAL             Software corruption is detected.
 **********************************************************************************************************************/
fsp_err_t R_RSIP_FWUP_StartUpdateFirmware(rsip_ctrl_t * const p_ctrl)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;

#if RSIP_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl);
    FSP_ERROR_RETURN(RSIP_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Check state */
    FSP_ERROR_RETURN(RSIP_STATE_MAIN == p_instance_ctrl->state, FSP_ERR_INVALID_STATE);

    rsip_err_t rsip_ret = r_rsip_fwup_start();

    /* Check error */
    fsp_err_t err = FSP_ERR_CRYPTO_RSIP_FATAL;
    switch (rsip_ret)
    {
        case RSIP_SUCCESS:
        {
            /* State transition */
            p_instance_ctrl->state = RSIP_STATE_FWUP;
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

RSIP_SEC_P_SECURE_BOOT
/*******************************************************************************************************************//**
 * Prepares the FWUP MAC sign.<br>
 * Implements @ref rsip_api_t::firmwareMacSignInit.
 *
 * \<Usage Precautions>
 * @arg The p_encrypted_image_encryption_key is Encrypted Image Encryption Key.
 *
 * \<Operational State><br>
 * This API can only be executed in the RSIP_STATE_FWUP.
 *
 * @retval FSP_SUCCESS                           Normal termination.
 * @retval FSP_ERR_ASSERTION                     A required parameter is NULL.
 * @retval FSP_ERR_NOT_OPEN                      Module is not open.
 * @retval FSP_ERR_INVALID_STATE                 Internal state is illegal.
 * @retval FSP_ERR_CRYPTO_RSIP_RESOURCE_CONFLICT A resource conflict occurred because a hardware resource required
 *                                               by the processing is in use by other processing.
 * @retval FSP_ERR_CRYPTO_RSIP_KEY_SET_FAIL      Input key value is illegal.
 * @retval FSP_ERR_CRYPTO_RSIP_FATAL             Software corruption is detected.
 **********************************************************************************************************************/
fsp_err_t R_RSIP_FWUP_MAC_Sign_Init(rsip_ctrl_t * const      p_ctrl,
                                    rsip_wrapped_key_t const * const p_wrapped_key_encryption_key,
                                    uint8_t const * const    p_encrypted_image_encryption_key,
                                    uint8_t const * const    p_initial_vector,
                                    uint32_t const           firmware_size)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;

#if RSIP_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_wrapped_key_encryption_key);
    FSP_ASSERT(p_encrypted_image_encryption_key);
    FSP_ASSERT(p_initial_vector);
    FSP_ASSERT(firmware_size);
    FSP_ERROR_RETURN(RSIP_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    FSP_ERROR_RETURN(RSIP_STATE_FWUP == p_instance_ctrl->state, FSP_ERR_INVALID_STATE);

    /* Check configuration */
    FSP_ERROR_RETURN(g_func.p_fwup.p_init, FSP_ERR_NOT_ENABLED);
#endif

    /* firmware_size must be multiples of 16 */
    FSP_ERROR_RETURN((firmware_size % RSIP_PRV_BYTE_SIZE_FWUP_BLOCK == 0), FSP_ERR_INVALID_SIZE);


    /* Call primitive (cast to match the argument type with the primitive function) */
    rsip_err_t rsip_ret = g_func.p_fwup.p_init((const uint32_t*) p_wrapped_key_encryption_key->value,
                                                (const uint32_t*) p_encrypted_image_encryption_key,
                                                (const uint32_t*) p_initial_vector);

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

        default:
        {
            err = FSP_ERR_CRYPTO_RSIP_FATAL;
        }
    }


    return err;
}

RSIP_SEC_P_SECURE_BOOT
/*******************************************************************************************************************//**
 * Update the FWUP MAC sign.<br>
 * Implements @ref rsip_api_t::firmwareMacSignUpdate.
 *
 * \<Usage Precautions>
 * @arg The length must be in multiples of 16 bytes.
 *
 * \<Operational State><br>
 * This API can only be executed in the RSIP_STATE_FWUP.
 *
 * @retval FSP_SUCCESS                           Normal termination.
 * @retval FSP_ERR_ASSERTION                     A required parameter is NULL.
 * @retval FSP_ERR_NOT_OPEN                      Module is not open.
 * @retval FSP_ERR_INVALID_STATE                 Internal state is illegal.
 * @retval FSP_ERR_INVALID_SIZE                  Input length is illegal.
 * @retval FSP_ERR_CRYPTO_RSIP_FATAL             Software corruption is detected.
 **********************************************************************************************************************/
fsp_err_t R_RSIP_FWUP_MAC_Sign_Update(rsip_ctrl_t * const   p_ctrl,
                                        uint8_t const * const p_input,
                                        uint8_t * const       p_output,
                                        uint32_t const        length)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;

#if RSIP_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_input);
    FSP_ASSERT(p_output);
    FSP_ERROR_RETURN(length != 0, FSP_ERR_INVALID_SIZE);
    FSP_ERROR_RETURN(RSIP_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(RSIP_STATE_FWUP == p_instance_ctrl->state, FSP_ERR_INVALID_STATE);

    /* Check configuration */
    FSP_ERROR_RETURN(g_func.p_fwup.p_update, FSP_ERR_NOT_ENABLED);
#endif

    /* firmware_size must be multiples of 16 */
    FSP_ERROR_RETURN((length % RSIP_PRV_BYTE_SIZE_FWUP_BLOCK == 0), FSP_ERR_INVALID_SIZE);

    /* Call primitive (cast to match the argument type with the primitive function) */
    rsip_err_t rsip_ret = g_func.p_fwup.p_update((const uint32_t *) p_input, length / 4, (uint32_t *) p_output);

    /* Check error */
    fsp_err_t err = FSP_ERR_CRYPTO_RSIP_FATAL;
    switch (rsip_ret)
    {
        case RSIP_SUCCESS:
        {
            err = FSP_SUCCESS;
            break;
        }

        default:
        {
            err = FSP_ERR_CRYPTO_RSIP_FATAL;
        }
    }

    return err;    
}

RSIP_SEC_P_SECURE_BOOT
/*******************************************************************************************************************//**
 * Finalizes the FWUP MAC sign.<br>
 * Implements @ref rsip_api_t::firmwareMacSignFinish.
 *
 * \<Usage Precautions>
 * @arg The length must be in multiples of 16 bytes.
 *
 * \<Operational State><br>
 * This API can only be executed in the RSIP_STATE_FWUP.
 *
 * @retval FSP_SUCCESS                           Normal termination.
 * @retval FSP_ERR_ASSERTION                     A required parameter is NULL.
 * @retval FSP_ERR_NOT_OPEN                      Module is not open.
 * @retval FSP_ERR_INVALID_STATE                 Internal state is illegal.
 * @retval FSP_ERR_INVALID_SIZE                  Input length is illegal.
 * @retval FSP_ERR_CRYPTO_RSIP_FAIL              Internal error.
 * @retval FSP_ERR_CRYPTO_RSIP_FATAL             Software corruption is detected.
 **********************************************************************************************************************/
fsp_err_t R_RSIP_FWUP_MAC_Sign_Finish(rsip_ctrl_t * const p_ctrl,
                                        uint8_t const * const p_input,
                                        uint8_t const * const p_input_mac,
                                        uint32_t const        length,
                                        uint8_t * const       p_output,
                                        uint8_t * const p_output_mac)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;

#if RSIP_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_input);
    FSP_ASSERT(p_input_mac);
    FSP_ASSERT(p_output);
    FSP_ASSERT(p_output_mac);

    FSP_ERROR_RETURN(length != 0, FSP_ERR_INVALID_SIZE);
    FSP_ERROR_RETURN(RSIP_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    FSP_ERROR_RETURN(RSIP_STATE_FWUP == p_instance_ctrl->state, FSP_ERR_INVALID_STATE);

    /* Check configuration */
    FSP_ERROR_RETURN(g_func.p_fwup.p_final, FSP_ERR_NOT_ENABLED);
#endif


    /* firmware_size must be multiples of 16 */
    FSP_ERROR_RETURN((length % RSIP_PRV_BYTE_SIZE_FWUP_BLOCK == 0), FSP_ERR_INVALID_SIZE);

    /* Call primitive (cast to match the argument type with the primitive function) */
    rsip_err_t rsip_ret = g_func.p_fwup.p_final((const uint32_t *) p_input, 
                                                (const uint32_t *) p_input_mac, 
                                                length / 4,
                                                (uint32_t *) p_output, 
                                                (uint32_t *) p_output_mac);

    /* Check error */
    fsp_err_t err = FSP_ERR_CRYPTO_RSIP_FATAL;
    switch (rsip_ret)
    {
        case RSIP_SUCCESS:
        {
            err = FSP_SUCCESS;
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

RSIP_SEC_P_SECURE_BOOT
/*******************************************************************************************************************//**
 * Prepares the SecureBoot MAC verify.<br>
 * Implements @ref rsip_api_t::securebootMacVerifyInit.
 *
 *
 * \<Operational State><br>
 * This API can only be executed in the RSIP_STATE_FWUP.
 *
 * @retval FSP_SUCCESS                           Normal termination.
 **********************************************************************************************************************/
fsp_err_t R_RSIP_SB_MAC_Verify_Init(rsip_ctrl_t * const p_ctrl)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;

#if RSIP_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl);
    FSP_ERROR_RETURN(RSIP_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    FSP_ERROR_RETURN(RSIP_STATE_MAIN == p_instance_ctrl->state, FSP_ERR_INVALID_STATE);

    /* Check configuration */
    FSP_ERROR_RETURN(g_func.p_sb.p_init, FSP_ERR_NOT_ENABLED);
#endif


    rsip_err_t rsip_ret = g_func.p_sb.p_init();

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

        default:
        {
            err = FSP_ERR_CRYPTO_RSIP_FATAL;
        }
    }
    return err;
}

RSIP_SEC_P_SECURE_BOOT
/*******************************************************************************************************************//**
 * Update the SecureBoot MAC verify.<br>
 * Implements @ref rsip_api_t::securebootMacVerifyUpdate.
 *
 * \<Usage Precautions>
 * @arg The input_length must be in multiples of 16 bytes.
 *
 * \<Operational State><br>
 * This API can only be executed in the RSIP_STATE_FWUP.
 *
 * @retval FSP_SUCCESS                           Normal termination.
 * @retval FSP_ERR_ASSERTION                     A required parameter is NULL.
 * @retval FSP_ERR_NOT_OPEN                      Module is not open.
 * @retval FSP_ERR_INVALID_STATE                 Internal state is illegal.
 * @retval FSP_ERR_INVALID_SIZE                  Input length is illegal.
 * @retval FSP_ERR_CRYPTO_RSIP_FATAL             Software corruption is detected.
 **********************************************************************************************************************/
fsp_err_t R_RSIP_SB_MAC_Verify_Update(rsip_ctrl_t * const p_ctrl,
                                        uint8_t const * const p_input,
                                        uint32_t const input_length)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;

#if RSIP_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_input);
    FSP_ERROR_RETURN(input_length != 0, FSP_ERR_INVALID_SIZE);
    FSP_ERROR_RETURN(RSIP_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    FSP_ERROR_RETURN(RSIP_STATE_MAIN == p_instance_ctrl->state, FSP_ERR_INVALID_STATE);

    /* Check configuration */
    FSP_ERROR_RETURN(g_func.p_sb.p_update, FSP_ERR_NOT_ENABLED);
#endif

    /* input_length must be multiples of 16 */
    FSP_ERROR_RETURN((input_length % RSIP_PRV_BYTE_SIZE_FWUP_BLOCK == 0), FSP_ERR_INVALID_SIZE);

    /* Call primitive (cast to match the argument type with the primitive function) */
    rsip_err_t rsip_ret = g_func.p_sb.p_update((const uint32_t *) p_input, input_length / 4);

    /* Check error */
    fsp_err_t err = FSP_ERR_CRYPTO_RSIP_FATAL;
    switch (rsip_ret)
    {
        case RSIP_SUCCESS:
        {
            err = FSP_SUCCESS;
            break;
        }

        default:
        {
            err = FSP_ERR_CRYPTO_RSIP_FATAL;
        }
    }

    return err;  
}

RSIP_SEC_P_SECURE_BOOT
/*******************************************************************************************************************//**
 * Finalizes the SecureBoot MAC verify.<br>
 * Implements @ref rsip_api_t::securebootMacVerifyFinish.
 *
 * \<Usage Precautions>
 * @arg The input_length must be in multiples of 16 bytes.
 *
 * \<Operational State><br>
 * This API can only be executed in the RSIP_STATE_FWUP.
 *
 * @retval FSP_SUCCESS                           Normal termination.
 * @retval FSP_ERR_ASSERTION                     A required parameter is NULL.
 * @retval FSP_ERR_NOT_OPEN                      Module is not open.
 * @retval FSP_ERR_INVALID_STATE                 Internal state is illegal.
 * @retval FSP_ERR_INVALID_SIZE                  Input length is illegal.
 * @retval FSP_ERR_CRYPTO_RSIP_FAIL              Internal error.
 * @retval FSP_ERR_CRYPTO_RSIP_FATAL             Software corruption is detected.
 **********************************************************************************************************************/
fsp_err_t R_RSIP_SB_MAC_Verify_Finish(rsip_ctrl_t * const p_ctrl,
                                        uint8_t const * const p_input, 
                                        uint8_t const * const p_mac,
                                        uint32_t const input_length)
{
    rsip_instance_ctrl_t * p_instance_ctrl = (rsip_instance_ctrl_t *) p_ctrl;

#if RSIP_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_input);
    FSP_ASSERT(p_mac);
    FSP_ERROR_RETURN(input_length != 0, FSP_ERR_INVALID_SIZE);
    FSP_ERROR_RETURN(RSIP_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    FSP_ERROR_RETURN(RSIP_STATE_MAIN == p_instance_ctrl->state, FSP_ERR_INVALID_STATE);

    /* Check configuration */
    FSP_ERROR_RETURN(g_func.p_sb.p_final, FSP_ERR_NOT_ENABLED);
#endif

    /* input_length must be multiples of 16 */
    FSP_ERROR_RETURN((input_length % RSIP_PRV_BYTE_SIZE_FWUP_BLOCK == 0), FSP_ERR_INVALID_SIZE);

    /* Call primitive (cast to match the argument type with the primitive function) */
    rsip_err_t rsip_ret = g_func.p_sb.p_final((const uint32_t *) p_input, 
                                                (const uint32_t *) p_mac, 
                                                input_length / 4);

    /* Check error */
    fsp_err_t err = FSP_ERR_CRYPTO_RSIP_FATAL;
    switch (rsip_ret)
    {
        case RSIP_SUCCESS:
        {
            err = FSP_SUCCESS;
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
RSIP_SEC_DEFAULT
/*******************************************************************************************************************//**
 * @} (end addtogroup RSIP_PROTECTED)
 **********************************************************************************************************************/
