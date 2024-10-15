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
#include "r_smc_entry.h"
#include "rm_rai_data_collector.h"
#include "rm_rai_data_shipper.h"
#if (RM_RAI_DATA_SHIPPER_CFG_DEVICE_USED == RM_COMMS_UART)
 #include "rm_comms_uart_if.h"
#elif (RM_RAI_DATA_SHIPPER_CFG_DEVICE_USED == RM_COMMS_USB_PCDC)
 #include "rm_comms_usb_pcdc_if.h"
#endif

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#define FUNC(x, y)      R_ ## x ## y
#define XSETCRC8(x)     FUNC(x, _SetCRC8)
#define XINPUTDATA(x)   FUNC(x, _Input_Data)
#define XGETRESULT(x)   FUNC(x, _Get_Result)

#define RM_RAI_DATA_SHIPPER_CFG_SETCRC8     XSETCRC8  (RM_RAI_DATA_SHIPPER_CFG_CRC_COMPONENT)
#define RM_RAI_DATA_SHIPPER_CFG_INPUTDATA   XINPUTDATA(RM_RAI_DATA_SHIPPER_CFG_CRC_COMPONENT)
#define RM_RAI_DATA_SHIPPER_CFG_GETRESULT   XGETRESULT(RM_RAI_DATA_SHIPPER_CFG_CRC_COMPONENT)


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
 * Global function prototypes
 **********************************************************************************************************************/
#if (RM_RAI_DATA_SHIPPER_CFG_DEVICE_USED == RM_COMMS_UART)
extern void rai_data_shipper_write_callback(rm_comms_uart_callback_args_t * p_args);
void rm_rai_data_shipper_callback0(rm_comms_uart_callback_args_t * p_args);
#elif (RM_RAI_DATA_SHIPPER_CFG_DEVICE_USED == RM_COMMS_USB_PCDC)
extern void rai_data_shipper_write_callback(rm_comms_usb_pcdc_callback_args_t * p_args);
void rm_rai_data_shipper_callback0(rm_comms_usb_pcdc_callback_args_t * p_args);
#endif
void RM_RAI_DATA_SHIPPER_CFG_CALLBACK(rai_data_shipper_callback_args_t * p_args);

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
/* Data Shipper0 */
#if RM_RAI_DATA_SHIPPER_CFG_CRC_ENABLE & RM_RAI_DATA_SHIPPER_CFG_CRC_ENABLE
/* CRC */
crc_api_t g_rai_data_shipper0_crc_driver_api =
{
    .setCrc8   = RM_RAI_DATA_SHIPPER_CFG_SETCRC8,
    .inputData = RM_RAI_DATA_SHIPPER_CFG_INPUTDATA,
    .getResult = RM_RAI_DATA_SHIPPER_CFG_GETRESULT,
};

const crc_instance_t g_crc0 =
{
    .p_api = (crc_api_t const *)&g_rai_data_shipper0_crc_driver_api,
};
#endif

rai_data_shipper_instance_ctrl_t g_rai_data_shipper0_ctrl;

const rai_data_shipper_cfg_t g_rai_data_shipper0_cfg =
{
    .divider    = RM_RAI_DATA_SHIPPER_CFG_FRAME_RATE_DIVIDER,
#if RM_RAI_DATA_SHIPPER_CFG_CRC_ENABLE & RM_RAI_DATA_SHIPPER_CFG_CRC_ENABLE
    .p_crc      = &g_crc0,
#else
    .p_crc      = NULL,
#endif
    .p_comms    = &RM_RAI_DATA_SHIPPER_CFG_DEVICE_COMMS_INSTANCE,
    .p_context  = NULL,
    .p_callback = RM_RAI_DATA_SHIPPER_CFG_CALLBACK,
};

const rai_data_shipper_instance_t g_rai_data_shipper0 =
{
    .p_ctrl   = &g_rai_data_shipper0_ctrl,
    .p_cfg    = &g_rai_data_shipper0_cfg,
    .p_api    = &g_ds_on_rai_data_shipper,
};

/* Callback function */
#if (RM_RAI_DATA_SHIPPER_CFG_DEVICE_USED == RM_COMMS_UART)
void rm_rai_data_shipper_callback0(rm_comms_uart_callback_args_t * p_args)
#elif (RM_RAI_DATA_SHIPPER_CFG_DEVICE_USED == RM_COMMS_USB_PCDC)
void rm_rai_data_shipper_callback0(rm_comms_usb_pcdc_callback_args_t * p_args)
#endif
{
    p_args->p_context = (void *)&g_rai_data_shipper0_ctrl;
    rai_data_shipper_write_callback(p_args);
}
