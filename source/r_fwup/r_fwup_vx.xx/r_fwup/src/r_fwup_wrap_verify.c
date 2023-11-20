/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_fwup_wrap_verify.c
 * Description  : Functions for the Firmware update module.
 **********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 20.07.2023 2.00    First Release
 *         : 29.09.2023 2.01    Fixed log messages.
 *                              Add parameter checking.
 *                              Added arguments to R_FWUP_WriteImageProgram API.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_fwup_if.h"
#include "r_fwup_wrap_verify.h"

/**** Start user code ****/
/**** End user code   ****/

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/**** Start user code ****/
/**** End user code   ****/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
/**** Start user code ****/
/**** End user code   ****/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
/**** Start user code ****/
/**** End user code   ****/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
/**** Start user code ****/
/**** End user code   ****/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_get_crypt_context
 * Description  : wrapper function for get to the crypt library's context.
 * Arguments    : none
 * Return Value : library's static pointer
 *********************************************************************************************************************/
void * r_fwup_wrap_get_crypt_context(void)
{
    /* library's context. that need to be a static value. */
    /**** Start user code ****/
    static uint32_t s_ctx; /* Dummy */
    return ((void *)&s_ctx);
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_get_crypt_context
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_sha256_init
 * Description  : wrapper function for sha256.
 * Arguments    : vp_ctx : context
 * Return Value : library processing result
 *********************************************************************************************************************/
int32_t r_fwup_wrap_sha256_init(void * vp_ctx)
{
    /**** Start user code ****/
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_sha256_init
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_sha256_update
 * Description  : wrapper function for sha256.
 * Arguments    : vp_ctx   : context
 *                p_data   : message data
 *                data_len : data len
 * Return Value : library processing result
 *********************************************************************************************************************/
int32_t r_fwup_wrap_sha256_update(void * vp_ctx, C_U8_FAR *p_data, uint32_t datalen)
{
    /**** Start user code ****/
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_sha256_update
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_sha256_final
 * Description  : wrapper function for sha256.
 * Arguments    : p_hash : hash value storage destination pointer
 *                vp_ctx : context
 * Return Value : library processing result
 *********************************************************************************************************************/
int32_t r_fwup_wrap_sha256_final(uint8_t *p_hash, void * vp_ctx)
{
    /**** Start user code ****/
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_sha256_final
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_verify_ecdsa
 * Description  : wrapper function for ECDSA verification.
 * Arguments    : p_hash     : hash data
 *                p_sig_type : signature type
 *                p_sig      : signature
 *                sig_size   : signature size
 * Return Value : 0          : verify OK
 *                other      : verify NG
 *********************************************************************************************************************/
int32_t r_fwup_wrap_verify_ecdsa(uint8_t *p_hash, uint8_t *p_sig_type, uint8_t *p_sig, uint32_t sig_size)
{
    /**** Start user code ****/
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_verify_ecdsa
 *********************************************************************************************************************/

/**** Start user code ****/
/**** End user code   ****/

