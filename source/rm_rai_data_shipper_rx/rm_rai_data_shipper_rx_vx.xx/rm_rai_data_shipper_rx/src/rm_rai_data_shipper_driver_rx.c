/***********************************************************************************************************************
 * Copyright [2023] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
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

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "rm_rai_data_shipper.h"

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

fsp_err_t rm_rai_data_shipper_crc_open       (rai_data_shipper_ctrl_t          * const p_api_ctrl);
fsp_err_t rm_rai_data_shipper_crc_close      (rai_data_shipper_ctrl_t          * const p_api_ctrl);
fsp_err_t rm_rai_data_shipper_crc_calculate  (rai_data_shipper_ctrl_t          * const p_api_ctrl,
                                              crc_input_t                      * const p_crc_input, 
                                              uint32_t                         *       calculatedValue);
fsp_err_t rm_rai_data_shipper_crc_initialize (rai_data_shipper_ctrl_t          * const p_api_ctrl);

/*******************************************************************************************************************//**
 * Open CRC module.
 *
 * @retval FSP_SUCCESS                  Successfully configured.
 * @retval FSP_ERR_UNSUPPORTED          CRC is unsupported.
 ***********************************************************************************************************************/
fsp_err_t rm_rai_data_shipper_crc_open(rai_data_shipper_ctrl_t * const p_api_ctrl)
{
#if RM_RAI_DATA_SHIPPER_CFG_CRC_ENABLE
    return FSP_SUCCESS;
#else
    return FSP_ERR_UNSUPPORTED;
#endif
}

/*******************************************************************************************************************//**
 * Close CRC module.
 *
 * @retval FSP_SUCCESS                  Successfully configured.
 * @retval FSP_ERR_UNSUPPORTED          CRC is unsupported.
 ***********************************************************************************************************************/
fsp_err_t rm_rai_data_shipper_crc_close(rai_data_shipper_ctrl_t * const p_api_ctrl)
{
#if RM_RAI_DATA_SHIPPER_CFG_CRC_ENABLE
    return FSP_SUCCESS;
#else
    return FSP_ERR_UNSUPPORTED;
#endif
}

/*******************************************************************************************************************//**
 * Calculation for CRC.
 *
 * @retval FSP_SUCCESS                  Successfully configured.
 * @retval FSP_ERR_UNSUPPORTED          CRC is unsupported.
 ***********************************************************************************************************************/
fsp_err_t rm_rai_data_shipper_crc_calculate  (rai_data_shipper_ctrl_t          * const p_api_ctrl,
                                              crc_input_t                      * const p_crc_input, 
                                              uint32_t                         *       calculatedValue)
{
#if RM_RAI_DATA_SHIPPER_CFG_CRC_ENABLE
    int16_t     i;
    uint8_t    *p;
    rai_data_shipper_instance_ctrl_t * p_ctrl = (rai_data_shipper_instance_ctrl_t *) p_api_ctrl;

    /* Input values */
    p = (uint8_t *)p_crc_input->p_input_buffer;
    for(i = 0; i < p_crc_input->num_bytes; ++i)
    {
        p_ctrl->p_cfg->p_crc->p_api->inputData(*p);
        ++p;
    }

    /* Get calcualted value */
    p_ctrl->p_cfg->p_crc->p_api->getResult(calculatedValue);

    return FSP_SUCCESS;
#else
    return FSP_ERR_UNSUPPORTED;
#endif
}

/*******************************************************************************************************************//**
 * Initialize for CRC.
 *
 * @retval FSP_SUCCESS                  Successfully configured.
 * @retval FSP_ERR_UNSUPPORTED          CRC is unsupported.
 ***********************************************************************************************************************/
fsp_err_t rm_rai_data_shipper_crc_initialize (rai_data_shipper_ctrl_t          * const p_api_ctrl)
{
#if RM_RAI_DATA_SHIPPER_CFG_CRC_ENABLE
    rai_data_shipper_instance_ctrl_t * p_ctrl = (rai_data_shipper_instance_ctrl_t *) p_api_ctrl;

    /* Initialize */
    p_ctrl->p_cfg->p_crc->p_api->setCrc8();

    return FSP_SUCCESS;
#else
    return FSP_ERR_UNSUPPORTED;
#endif
}
