/***********************************************************************************************************************
* Copyright [2020-2024] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
*
* This software and documentation are supplied by Renesas Electronics America Inc. and may only be used with products
* of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.  Renesas products are
* sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for the selection and use
* of Renesas products and Renesas assumes no liability.  No license, express or implied, to any intellectual property
* right is granted by Renesas. This software is protected under all applicable laws, including copyright laws. Renesas
* reserves the right to change or discontinue this software and/or this documentation. THE SOFTWARE AND DOCUMENTATION
* IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND TO THE FULLEST EXTENT
* PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY, INCLUDING WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE SOFTWARE OR
* DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.  TO THE MAXIMUM
* EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR DOCUMENTATION
* (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER, INCLUDING,
* WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY LOST PROFITS,
* OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY
* OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
**********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_rrh46410_if.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/
fsp_err_t rm_rrh46410_delay_ms(rm_zmod4xxx_ctrl_t * const p_ctrl, uint32_t const delay_ms);
fsp_err_t rm_rrh46410_irq_open(rm_zmod4xxx_ctrl_t * const p_api_ctrl);
fsp_err_t rm_rrh46410_irq_close(rm_zmod4xxx_ctrl_t * const p_api_ctrl);
void      rm_rrh46410_irq_callback(external_irq_callback_args_t * p_args);

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Delay some milliseconds.
 *
 * @retval FSP_SUCCESS              Successfully delayed.
 **********************************************************************************************************************/
fsp_err_t rm_rrh46410_delay_ms (rm_zmod4xxx_ctrl_t * const p_ctrl, uint32_t const delay_ms)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);

    /* Software delay */
    R_BSP_SoftwareDelay(delay_ms, BSP_DELAY_MILLISECS);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Open IRQ driver and set callback function.
 *
 * @retval FSP_SUCCESS              Successfully configured.
 **********************************************************************************************************************/
fsp_err_t rm_rrh46410_irq_open (rm_zmod4xxx_ctrl_t * const p_api_ctrl)
{
#if RM_RRH46410_CFG_IRQ_ENABLE
    fsp_err_t err = FSP_SUCCESS;
    rm_rrh46410_instance_ctrl_t * p_ctrl         = (rm_rrh46410_instance_ctrl_t *) p_api_ctrl;
    external_irq_instance_t     * p_irq_instance = (external_irq_instance_t *) p_ctrl->p_irq_instance;
    irq_err_t ret;

    /* Open IRQ driver */
    ret = R_IRQ_Open(p_irq_instance->number, 
                     p_irq_instance->trigger, 
                     p_irq_instance->priority, 
                     p_irq_instance->p_handle, 
                     p_irq_instance->p_callback);

    switch (ret)
    {
        case IRQ_SUCCESS :
         err = FSP_SUCCESS;
         break;
        case IRQ_ERR_BAD_NUM :
         err = FSP_ERR_INVALID_CHANNEL;
         break;
        case IRQ_ERR_NOT_CLOSED :
         err = FSP_ERR_IN_USE;
         break;
        case IRQ_ERR_INVALID_PTR :
         err = FSP_ERR_INVALID_POINTER;
         break;
        case IRQ_ERR_INVALID_ARG :
         err = FSP_ERR_INVALID_ARGUMENT;
         break;
        case IRQ_ERR_LOCK :
         err = FSP_ERR_HW_LOCKED;
         break;
        default :
         err = FSP_ERR_ASSERTION;
         break;
    }

    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif
    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Close IRQ driver.
 *
 * @retval FSP_SUCCESS              Successfully close.
 **********************************************************************************************************************/
fsp_err_t rm_rrh46410_irq_close (rm_zmod4xxx_ctrl_t * const p_api_ctrl)
{
#if RM_RRH46410_CFG_IRQ_ENABLE
    rm_rrh46410_instance_ctrl_t * p_ctrl         = (rm_rrh46410_instance_ctrl_t *) p_api_ctrl;
    external_irq_instance_t     * p_irq_instance = (external_irq_instance_t *) p_ctrl->p_irq_instance;

    /* Close IRQ driver */
    R_IRQ_Close(*p_irq_instance->p_handle);
#endif
    return FSP_SUCCESS;
}

void rm_rrh46410_irq_callback (external_irq_callback_args_t * p_args)
{
    rm_rrh46410_instance_ctrl_t * p_ctrl = (rm_rrh46410_instance_ctrl_t *) p_args->p_context;
    rm_zmod4xxx_callback_args_t   rrh46410_callback_args;

    /* Set context */
    rrh46410_callback_args.p_context = p_ctrl->p_context;

    /* Set event */
    rrh46410_callback_args.event = RM_ZMOD4XXX_EVENT_MEASUREMENT_COMPLETE;
    p_ctrl->event                = RM_ZMOD4XXX_EVENT_MEASUREMENT_COMPLETE;

    if (NULL != p_ctrl->p_irq_callback)
    {
        /* Call callback function */
        p_ctrl->p_irq_callback(&rrh46410_callback_args);
    }
}
