/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
 * No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
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
 * Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * File Name    : r_ri3c_rx.c
 * Description  : Renesas RX FIT RI3C driver.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 15.08.2022 1.00    First release
 *                              Supported for RX26T.
 *         : 13.12.2023 1.11    Added WAIT_LOOP comments.
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "platform.h"
#include "r_ri3c_rx_if.h"
#include "r_ri3c_rx_data.h"
#include "r_ri3c_rx_config.h"
#include <string.h>

/***********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/
/* Initiate Extended configuration for RI3C instance prototype */
static ri3c_extended_cfg_t ri3c_extended_cfg_init ();

/* Bitrate calculation prototypes */
static double ri3c_calculateHighSetting (double pclka_frequency, 
    double t_high, 
    double desired_freq, 
    uint16_t register_max, 
    uint8_t dsbrpo);

static double ri3c_calculateLowSetting (double pclka_frequency, 
    double high_setting, 
    double desired_freq, 
    double t_cr, 
    double t_cf, 
    uint16_t register_max, 
    uint8_t dsbrpo);

static void    ri3c_calculateBitRate (void);
static long    ceil_function (long a, long b);
static uint8_t ri3c_address_parity (uint8_t address);

/* ISR function prototypes. */
static void ri3c_eei_isr(void);

/* bus setting prototypes. */
void ri3c_bus_idle_detection_time(void);

#if RI3C_CFG_CONTROLLER_SUPPORT
static uint32_t ri3c_xfer_command_calculate(uint32_t dev_index, bool rnw, uint32_t bitrate_setting, bool restart);
#endif

static void ri3c_fifo_read(ri3c_instance_ctrl_t *p_ctrl, uint32_t bytes);
static void ri3c_fifo_write(ri3c_instance_ctrl_t *p_ctrl);
static void ri3c_read_buffer_store(ri3c_instance_ctrl_t *const p_ctrl,
                                   ri3c_read_buffer_descriptor_t *p_buffer_descriptor,
                                   uint32_t data_word,
                                   uint32_t num_bytes,
                                   ri3c_event_t buffer_full_event);
static uint32_t ri3c_read_bytes_remaining_calculate(ri3c_instance_ctrl_t *p_ctrl, uint32_t data_length);
static uint32_t ri3c_next_data_word_calculate(ri3c_write_buffer_descriptor_t *p_buffer_descriptor);

#if RI3C_CFG_CONTROLLER_SUPPORT
static void ri3c_controller_error_recovery(ri3c_instance_ctrl_t *p_ctrl, bool error_recovery_case_2);
#endif

#if RI3C_CFG_TARGET_SUPPORT
static void ri3c_target_error_recovery(ri3c_instance_ctrl_t *p_ctrl, ri3c_target_error_recovery_type_t recovery_type);
#endif

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
ri3c_instance_ctrl_t g_ri3c0_ctrl;
ri3c_api_t           g_ri3c0_on_ri3c; // Filled in Interface API structure for this Instance.

/* Extended configuration for this instance of RI3C. */
ri3c_extended_cfg_t g_ri3c0_cfg_extend;

/* RI3C implementation of RI3C API. */
const ri3c_api_t g_ri3c_on_ri3c =
{
    .open = R_RI3C_Open,
    .enable = R_RI3C_Enable,
    .deviceCfgSet = R_RI3C_DeviceCfgSet,
    .controllerDeviceTableSet = R_RI3C_ControllerDeviceTableSet,
    .targetStatusSet = R_RI3C_TargetStatusSet,
    .deviceSelect = R_RI3C_DeviceSelect,
    .dynamicAddressAssignmentStart = R_RI3C_DynamicAddressAssignmentStart,
    .commandSend = R_RI3C_CommandSend,
    .write = R_RI3C_Write,
    .read = R_RI3C_Read,
    .ibiWrite = R_RI3C_IbiWrite,
    .ibiRead = R_RI3C_IbiRead,
    .close = R_RI3C_Close,
};

/* Bitrate Settings Variables */
/** Standard Bitrate Settings */
static uint32_t std_od_high_setting;
static uint32_t std_od_low_setting;
static uint32_t std_pp_high_setting;
static uint32_t std_pp_low_setting;

/** Extended Bitrate Settings */
static uint32_t ext_od_high_setting;
static uint32_t ext_od_low_setting;
static uint32_t ext_pp_high_setting;
static uint32_t ext_pp_low_setting;

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_RI3C_Open
 ******************************************************************************************************************//**
 * @brief Configure an RI3C instance.
 * @param[in] p_api_ctrl Pointer to RI3C control block.
 * @param[in] p_cfg Pointer to User configuration structure.
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was invalid.
 * @retval FSP_ERR_ALREADY_OPEN           Open has already been called for this instance.
 * @retval FSP_ERR_UNSUPPORTED            A selected feature is not supported with the current configuration.
 * @details Configure an RI3C instance.
 * @note None.
 */
fsp_err_t R_RI3C_Open(ri3c_ctrl_t *const p_api_ctrl, ri3c_cfg_t const *const p_cfg)
{
    g_ri3c0_cfg_extend = ri3c_extended_cfg_init();   
    ri3c_instance_ctrl_t * p_ctrl = (ri3c_instance_ctrl_t *)p_api_ctrl; // Cast to functional data type

#if RI3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_callback);
    FSP_ASSERT(NULL != p_cfg->p_extend);
    FSP_ERROR_RETURN(RI3C_OPEN != p_ctrl->open, FSP_ERR_ALREADY_OPEN);
#if 0U == RI3C_CFG_CONTROLLER_SUPPORT
    FSP_ERROR_RETURN(RI3C_DEVICE_TYPE_PRIMARY_CONTROLLER != p_cfg->device_type, FSP_ERR_UNSUPPORTED);
#endif
#if 0U == RI3C_CFG_TARGET_SUPPORT
    FSP_ERROR_RETURN(RI3C_DEVICE_TYPE_TARGET != p_cfg->device_type, FSP_ERR_UNSUPPORTED);
#endif /* 0U == RI3C_CFG_TARGET_SUPPORT */
#endif

    /* Initialize the internal state of the driver. */
    memset(p_ctrl, 0, sizeof(ri3c_instance_ctrl_t));

    p_ctrl->open  = RI3C_OPEN;
    p_ctrl->p_cfg = p_cfg;

    bsp_int_ctrl_t int_ctrl;
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Set IPL to the maximum value to disable all interrupts,
     * so the scheduler can not be scheduled in critical region.
     */
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);

    /* Clear the RI3C Module Stop bit.
     * MSPTD5 bit of Module Stop Register D (MSTPCRD) is RI3C Bus Interface 0 Module Stop
     * 0: Release from module-stop state
     * 1: Transition to the module-stop state is made
     */
    MSTP(RI3C0) = 0;

    /* Restore the IPL. */
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Get a pointer to the RI3C registers for this channel. */
    p_ctrl->p_reg = &RI3C0;

    /*
     * Reset the RI3C Peripheral so that it is in a known state during initialization 
     * (See Figure 35.70 I3C Communication Flow in the RX26T manual).
     */
    p_ctrl->p_reg->ICCR.BIT.ICE   = 0;
    p_ctrl->p_reg->ICRCR.BIT.MRST = 1U;

    /* The field will be cleared automatically upon reset completion (See section 35.2.4 in the Rx26T manual). */
    /* WAIT_LOOP */
    while (p_ctrl->p_reg->ICRCR.BIT.MRST != 0U)
    {
        /* Wait. */
    }

    /* Set RI3C mode. */
    p_ctrl->p_reg->ICMR.BIT.OMS = 0U;

    return FSP_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_RI3C_Open
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_RI3C_Enable
 *******************************************************************************************************************//**
 * @brief Enable the RI3C device.
 * @param[in] p_api_ctrl Pointer to RI3C control block.
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_INVALID_MODE           This instance is already enabled.
 * @details Enable the RI3C device.
 * @note None.
 */
fsp_err_t R_RI3C_Enable(ri3c_ctrl_t *const p_api_ctrl)
{
    ri3c_instance_ctrl_t * p_ctrl = (ri3c_instance_ctrl_t *)p_api_ctrl; // Cast to functional data type
    bsp_int_ctrl_t         int_ctrl;

#if RI3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);
    FSP_ERROR_RETURN(RI3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(RI3C_INTERNAL_STATE_DISABLED == p_ctrl->internal_state, FSP_ERR_INVALID_MODE);
#endif

    ri3c_extended_cfg_t * p_extend = (ri3c_extended_cfg_t *)p_ctrl->p_cfg->p_extend; // Cast to functional data type

#if RI3C_CFG_CONTROLLER_SUPPORT
    /* Write the standard and extended bitrate settings. */
    p_ctrl->p_reg->ICSBR.LONG = p_extend->bitrate_settings.icsbr;
    p_ctrl->p_reg->ICEBR.LONG = p_extend->bitrate_settings.icebr;
#endif

    /*
     * Write all remaining configuration settings 
     * (See Figure 35.70 RI3C Communication Flow in the RX26T manual).
     *
     * Configure the Normal IBI Data Segment Size used for receiving IBI data.
     */
    uint32_t icqbtcr = 6U << R_RI3C0_ICQBTCR_IDSS_Pos;

    /*
     * Enable Transfer Interrupts.
     * - Read Buffer Full Interrupt.
     * - Receive Status Queue Full Interrupt
     * - Response Queue Full Interrupt
     */
    uint32_t iccsier = R_RI3C0_ICCSIER_RIE_Msk | R_RI3C0_ICCSIER_RQFIE_Msk | R_RI3C0_ICCSIER_SQFIE_Msk;

#if RI3C_CFG_CONTROLLER_SUPPORT
    if (RI3C_DEVICE_TYPE_PRIMARY_CONTROLLER == p_ctrl->p_cfg->device_type)
    {
        p_ctrl->internal_state = RI3C_INTERNAL_STATE_CONTROLLER_IDLE;

        /* Enable the Queue Empty/Full Interrupt. */
        iccsier |= R_RI3C0_ICCSIER_IQEFIE_Msk;
    }
    else
#endif
    {
#if RI3C_CFG_TARGET_SUPPORT
        p_ctrl->internal_state = RI3C_INTERNAL_STATE_TARGET_IDLE;

        /*
         * Configure IBI queue threshold so that an IBI IRQ is generated when
         * there is one empty entry in the IBI transmit FIFO.
         */
        icqbtcr |= 1U << R_RI3C0_ICQBTCR_IQTH_Pos;
#endif
    }

    /*
     * Write settings to the Normal Queue Threshold Control Register.
     * - Normal Response Queue Threshold interrupt after one response is received.
     * - Normal IBI Queue Threshold interrupt after one IBI status is received.
     * - Normal IBI Data Segment Size set to 6 words.
     */
    p_ctrl->p_reg->ICQBTCR.LONG = icqbtcr;

    /* Set the Normal Transfer Data Buffer threshold for transmit and receive (The only supported configuration
     * is two entries). */
    p_ctrl->p_reg->ICDBTCR.LONG = 0;

    /* Set the Receive Status Queue Threshold to zero (Interrupt when there is one entry in the queue). */
    p_ctrl->p_reg->ICSQTCR.LONG = 0;

    /* Enable the Internal Error Status Flag. */
    p_ctrl->p_reg->ICISER.LONG = R_RI3C0_ICISER_BERDE_Msk;

    /* Enable all Bus Status Flags. */
    p_ctrl->p_reg->ICSER.LONG = (R_RI3C0_ICSER_STDE_Msk |
                                 R_RI3C0_ICSER_SPDE_Msk |
                                 R_RI3C0_ICSER_HDRXDE_Msk |
                                 R_RI3C0_ICSER_TMOE_Msk);

    /* Enable all Transfer Status Flags. */
    p_ctrl->p_reg->ICCSER.LONG = (R_RI3C0_ICCSER_TDE_Msk |
                                  R_RI3C0_ICCSER_RDE_Msk |
                                  R_RI3C0_ICCSER_IQEFDE_Msk |
                                  R_RI3C0_ICCSER_CQEDE_Msk |
                                  R_RI3C0_ICCSER_RQFDE_Msk |
                                  R_RI3C0_ICCSER_DTADE_Msk |
                                  R_RI3C0_ICCSER_DTEDE_Msk |
                                  R_RI3C0_ICCSER_SQFDE_Msk);

    /* Enable the Internal Error Interrupt Requests. */
    p_ctrl->p_reg->ICISIER.LONG = R_RI3C0_ICISIER_BERIE_Msk;

    /* Enable HDR exit pattern Interrupt Requests. */
    p_ctrl->p_reg->ICSIER.LONG = R_RI3C0_ICSIER_HDRXIE_Msk | R_RI3C0_ICSIER_TMOIE_Msk;

    /* Write settings to the Normal Transfer Interrupt Enable Register. */
    p_ctrl->p_reg->ICCSIER.LONG = iccsier;

    /* Write the Hot-Join Ackowlege settings. */
    p_ctrl->p_reg->ICCR.BIT.HJC = !p_extend->ibi_control.hot_join_acknowledge;

    /* Calculate the value of the IBI Notify Control Register. */
    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    uint32_t icincr =
        (uint32_t)(p_extend->ibi_control.notify_rejected_hot_join_requests << R_RI3C0_ICINCR_RHJRN_Pos);

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    icincr |= (uint32_t)(p_extend->ibi_control.notify_rejected_controllerrole_requests << R_RI3C0_ICINCR_RCRRN_Pos);

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    icincr |= (uint32_t)(p_extend->ibi_control.notify_rejected_interrupt_requests << R_RI3C0_ICINCR_RTIRN_Pos);

    /* Write settings to the IBI Notify Control Register. */
    p_ctrl->p_reg->ICINCR.LONG = icincr;

    /* Calculate the value of the SCL Clock Stalling Control Register. */
    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    uint32_t icstcr = 
        (uint32_t)(p_extend->bitrate_settings.clock_stalling.assigned_address_phase_enable << R_RI3C0_ICSTCR_AASE_Pos);

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    icstcr |= (uint32_t)(p_extend->bitrate_settings.clock_stalling.parity_phase_enable << R_RI3C0_ICSTCR_PBSE_Pos);

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    icstcr |= (uint32_t)(p_extend->bitrate_settings.clock_stalling.ack_phase_enable << R_RI3C0_ICSTCR_APSE_Pos);

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    icstcr |=
        (uint32_t)(p_extend->bitrate_settings.clock_stalling.clock_stalling_time << R_RI3C0_ICSTCR_STT_Pos);

    /* Write clock stalling settings to the SCL CLock Stalling Control Register. */
    p_ctrl->p_reg->ICSTCR.LONG = icstcr;

    /* Write Bus Condition Detection Registers. */
    p_ctrl->p_reg->ICBFTR = p_extend->bus_free_detection_time;
    p_ctrl->p_reg->ICBATR = p_extend->bus_available_detection_time;
    p_ctrl->p_reg->ICBITR = p_extend->bus_idle_detection_time;

    if (!p_extend->timeout_detection_enable)
    {
        /* Disable Timeout Detection. */
        p_ctrl->p_reg->ICTOR.LONG = 0U;
    }

#if RI3C_CFG_TARGET_SUPPORT

    /* Calculate the value of the CCC Target Events Command Register. */
    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    uint32_t ictevr =
        (uint32_t)(p_extend->target_command_response_info.inband_interrupt_enable << R_RI3C0_ICTEVR_ENINT_Pos);

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    ictevr |= 
        (uint32_t)(p_extend->target_command_response_info.controllerrole_request_enable << R_RI3C0_ICTEVR_ENCR_Pos);

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    ictevr |= (uint32_t)(p_extend->target_command_response_info.hotjoin_request_enable << R_RI3C0_ICTEVR_ENHJ_Pos);

    /* Calculate the value of the CCC Max Data Speed Read Register. */
    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    uint32_t icmrsr = (uint32_t)p_extend->target_command_response_info.read_data_rate << R_RI3C0_ICMRSR_MSRDR_Pos;

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    icmrsr |= (uint32_t)p_extend->target_command_response_info.clock_data_turnaround << R_RI3C0_ICMRSR_TSCO_Pos;

    /* Calculate the value of the CCC Max Data Speed Rurnaround Register. */
    uint32_t icmttr = p_extend->target_command_response_info.read_turnaround_time << R_RI3C0_ICMTTR_MRTT_Pos;

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    icmttr |= ((uint32_t)p_extend->target_command_response_info.read_turnaround_time_enable) <<
            R_RI3C0_ICMTTR_MRTTE_Pos;

    /* Calculate the value of the CCC Exchange Timing Support Information Register. */
    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    uint32_t ictsir = (uint32_t)(p_extend->target_command_response_info.oscillator_frequency <<
        R_RI3C0_ICTSIR_FREQ_Pos) & R_RI3C0_ICTSIR_FREQ_Msk;

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    ictsir |= (uint32_t)(p_extend->target_command_response_info.oscillator_inaccuracy <<
            R_RI3C0_ICTSIR_INAC_Pos) & R_RI3C0_ICTSIR_INAC_Msk;

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    uint32_t icmrlr = (uint32_t)p_extend->target_command_response_info.read_length;

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    icmrlr |= (uint32_t)p_extend->target_command_response_info.ibi_payload_length << R_RI3C0_ICMRLR_IBIPL_Pos;

    /* Write Target Command Response Info. */
    p_ctrl->p_reg->ICTEVR.LONG = ictevr;
    p_ctrl->p_reg->ICASR.LONG  = (1U << p_extend->target_command_response_info.activity_state);

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    p_ctrl->p_reg->ICMWLR.LONG = (uint32_t)p_extend->target_command_response_info.write_length;
    p_ctrl->p_reg->ICMRLR.LONG = icmrlr;

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    p_ctrl->p_reg->ICDSR.LONG  = (uint32_t)(p_extend->target_command_response_info.activity_state << \
                                R_RI3C0_ICDSR_CAS_Pos);

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    p_ctrl->p_reg->ICMWSR.LONG = (uint32_t)p_extend->target_command_response_info.write_data_rate;
    p_ctrl->p_reg->ICMRSR.LONG = icmrsr;
    p_ctrl->p_reg->ICMTTR.LONG = icmttr;
    p_ctrl->p_reg->ICTSIR.LONG = ictsir;
#endif /* RI3C_CFG_TARGET_SUPPORT */

    /* Enable the RI3C Bus. */
    p_ctrl->p_reg->ICCR.LONG |= R_RI3C0_ICCR_ICE_Msk;

    /* Enable the command response status buffer full IRQ. */
    R_BSP_InterruptRequestEnable(VECT(RI3C0, RESPI));
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);

    IEN(RI3C0, RESPI) = 1; // ICU.IER[IER_RI3C0_RESPI].BIT.IEN0 = 1;

    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);

    IPR(RI3C0, RESPI) = p_extend->ipl; // ICU.IPR[IPR_RI3C0_RESPI].BIT.IPR = p_extend->ipl;

    /* Enable the receive data buffer full IRQ. */
    R_BSP_InterruptRequestEnable(VECT(RI3C0, RXI));
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);

    IEN(RI3C0, RXI) = 1; // ICU.IER[IER_RI3C0_RXI].BIT.IEN6 = 1;

    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);

    IPR(RI3C0, RXI) = p_extend->ipl; // ICU.IPR[IPR_RI3C0_RXI].BIT.IPR = p_extend->ipl;

    /* Enable the transmit data buffer empty IRQ. */
    R_BSP_InterruptRequestEnable(VECT(RI3C0, TXI));
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);

    IEN(RI3C0, TXI) = 1; // ICU.IER[IER_RI3C0_TXI].BIT.IEN7 = 1;

    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);

    IPR(RI3C0, TXI) = p_extend->ipl; // ICU.IPR[IPR_RI3C0_TXI].BIT.IPR = p_extend->ipl;

#if RI3C_CFG_TARGET_SUPPORT

    /* Enable the receive status buffer full IRQ. */
    R_BSP_InterruptRequestEnable(VECT(RI3C0, RCVI));
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);

    IEN(RI3C0, RCVI) = 1; // ICU.IER[IER_RI3C0_RCVI].BIT.IEN3 = 1;

    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);

    IPR(RI3C0, RCVI) = p_extend->ipl; // ICU.IPR[IPR_RI3C0_RCVI].BIT.IPR = p_extend->ipl;

#endif

    /* Enable the IBI IRQ. */
    R_BSP_InterruptRequestEnable(VECT(RI3C0, IBII));
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);

    IEN(RI3C0, IBII) = 1; // ICU.IER[IER_RI3C0_IBII].BIT.IEN2 = 1;

    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);

    IPR(RI3C0, IBII) = p_extend->ipl; // ICU.IPR[IPR_RI3C0_IBII].BIT.IPR = p_extend->ipl;

    /* Enable the Error and Event IRQ. */
    R_BSP_InterruptRequestEnable(VECT(ICU, GROUPAL1));
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);

    EN(RI3C0, EEI) = 1; // ICU.GEN[GEN_RI3C0_EEI].BIT.EN13 = 1;

    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);

    IPR(ICU, GROUPAL1) = p_extend->eei_ipl; // ICU.IPR[IPR_ICU_GROUPAL1] = p_extend->ipl;

    /** EEI belongs to Group Interrupts GROUPAL1 - refer Table 14.6 Group Interrupt Requests */
    R_BSP_InterruptWrite(BSP_INT_SRC_AL1_RI3C0_EEI, (bsp_int_cb_t)ri3c_eei_isr);

    return FSP_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_RI3C_Enable
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_RI3C_DeviceCfgSet
 *******************************************************************************************************************//**
 * @brief Set the configuration for this device.
 * @param[in] p_api_ctrl Pointer to RI3C control block.
 * @param[in] p_device_cfg Pointer to Structure for configuring a target address when the driver is in target mode.
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_UNSUPPORTED            The device cannot be a secondary controller if controller support is disabled.
 * @details Set the configuration for this device.
 * @note None.
 */
fsp_err_t R_RI3C_DeviceCfgSet(ri3c_ctrl_t *const p_api_ctrl, ri3c_device_cfg_t const *const p_device_cfg)
{
    ri3c_instance_ctrl_t * p_ctrl = (ri3c_instance_ctrl_t *)p_api_ctrl; // Cast to functional data type

#if RI3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);
    FSP_ERROR_RETURN(RI3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(NULL != p_device_cfg);
#endif

#if RI3C_CFG_CONTROLLER_SUPPORT
    if (RI3C_DEVICE_TYPE_PRIMARY_CONTROLLER == p_ctrl->p_cfg->device_type)
    {
        /*
         * Configure the controller dynamic address and set it to valid (See Figure 35.70 RI3C Communication Flow
         * in the RX26T manual).
         */
        uint32_t iccar            = (uint32_t)p_device_cfg->dynamic_address << R_RI3C0_ICCAR_DADR_Pos;
        iccar                    |= R_RI3C0_ICCAR_DAV_Msk;
        p_ctrl->p_reg->ICCAR.LONG = iccar;
    }
    else
#endif
    {
#if RI3C_CFG_TARGET_SUPPORT

        /* Secondary controller device role is not supported. */
        FSP_ERROR_RETURN(1U != p_device_cfg->target_info.bcr_b.device_role, FSP_ERR_UNSUPPORTED);

        /*
         * Configure target device address table, Device Characteristics, Bus Characteristics, and
         * Provisional ID (See Figure 35.70 RI3C Communication Flow in the RX26T manual).
         *
         * Configure the device static address. */
        /* Cast to 32 bit unsigned integer to match 32 bit register. */
        uint32_t icdar0 = (uint32_t)p_device_cfg->static_address & R_RI3C0_ICDAR0_SADR_Msk;

        /** Configure the device dynamic address. */
        /* Cast to 32 bit unsigned integer to match 32 bit register. */
        icdar0 |= (uint32_t)(ri3c_address_parity(p_device_cfg->dynamic_address) << R_RI3C0_ICDAR0_DADR_Pos) & \
            R_RI3C0_ICDAR0_DADR_Msk;

        /** Configure the IBI payload setting based on the BCR register. */
        /* Cast to 32 bit unsigned integer to match 32 bit register. */
        icdar0 |= (uint32_t)((p_device_cfg->target_info.bcr_b.ibi_payload) << R_RI3C0_ICDAR0_IBIPL_Pos) & \
            R_RI3C0_ICDAR0_IBIPL_Msk;

        /** Write settings to the Target Device Address Table Register. */
        /* Cast to 32 bit unsigned integer to match 32 bit register. */
        p_ctrl->p_reg->ICDAR0.LONG = icdar0;

        /** Write the BCR and DCR settings to the Target Device Characteristic Table Register. */
        /* Cast to 32 bit unsigned integer to match 32 bit register. */
        uint32_t icdctr = (uint32_t)p_device_cfg->target_info.dcr;

        /* Cast to 32 bit unsigned integer to match 32 bit register. */
        icdctr                    |= (uint32_t)p_device_cfg->target_info.bcr << R_RI3C0_ICDCTR_LIMIT_Pos;
        p_ctrl->p_reg->ICDCTR.LONG = icdctr;

        /** Write the PID setting to the Target Device Characteristic Table PID Register. */
        /* Cast to 8 bit unsigned integer to match 8 bit longs PID . */
        uint8_t * pid = (uint8_t *)p_device_cfg->target_info.pid;

        /* Cast to 32 bit unsigned integer to match 32 bit register. */
        p_ctrl->p_reg->ICPIDLR = (uint32_t)((pid[4] << 8U) | (pid[5]));

        /* Cast to 32 bit unsigned integer to match 32 bit register. */
        p_ctrl->p_reg->ICPIDHR = (uint32_t)((pid[0] << 24U) |
                                            (pid[1] << 16U) |
                                            (pid[2] << 8U)  |
                                            (pid[3] << 0U));

        /* Set the target address to valid. */
        p_ctrl->p_reg->ICTCR.BIT.TA0DE = 1;
#endif
    }

    return FSP_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_RI3C_DeviceCfgSet
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_RI3C_ControllerDeviceTableSet
 ******************************************************************************************************************//**
 * @brief Configure an entry in the controller device table.
 * @param[in] p_api_ctrl Pointer to RI3C control block.
 * @param[in] device_index Index into the device table.
 * @param[in] p_device_table_cfg Pointer to the table settings for the entry in the master device table.
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_UNSUPPORTED            Controller Role requests must be rejected if target support is disabled.
 * @details Configure an entry in the controller device table.
 * @note This function is called in controller mode in order to configure the devices on the RI3C bus.
 * It may also be called in target mode when the target receives the DEFSVLS command.
 */
fsp_err_t R_RI3C_ControllerDeviceTableSet(ri3c_ctrl_t *const p_api_ctrl,
                                          uint32_t device_index,
                                          ri3c_device_table_cfg_t const *const p_device_table_cfg)
{
#if RI3C_CFG_CONTROLLER_SUPPORT
    ri3c_instance_ctrl_t * p_ctrl = (ri3c_instance_ctrl_t *)p_api_ctrl; // Cast to functional data type

#if RI3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT (NULL != p_api_ctrl);
    FSP_ERROR_RETURN (RI3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT (NULL != p_device_table_cfg);
#endif

    /* The driver does not support accepting controller role requests. */
    FSP_ERROR_RETURN(!p_device_table_cfg->controllerrole_request_accept, FSP_ERR_UNSUPPORTED);

#if((__CCRX__) || (__GNUC__))
    R_BSP_VOLATILE_EVENACCESS uint32_t *p_ictdatrn_reg = NULL;
#elif(__ICCRX__)
    uint32_t R_BSP_VOLATILE_SFR * p_ictdatrn_reg = NULL;
#endif
    
    uint32_t ictdatrn = 0;

    if (RI3C_DEVICE_INDEX_EXTENDED_DEVICE == device_index)
    {
#if(__CCRX__) || (__GNUC__)
        p_ictdatrn_reg = &p_ctrl->p_reg->ICEDATR.LONG;
#elif(__ICCRX__)
        p_ictdatrn_reg = (uint32_t R_BSP_VOLATILE_SFR *)&p_ctrl->p_reg->ICEDATR.LONG;
#endif
    }
    else
    {
        /* Compute the address of the ICTDATRn register. */
        uint32_t address_offset = (uint32_t)(&p_ctrl->p_reg->ICTDATR1.LONG - &p_ctrl->p_reg->ICTDATR0.LONG);
#if(__CCRX__) || (__GNUC__)
        /* Pointer to address of ICTDATRN in CCRX and GGC compilers. */
        p_ictdatrn_reg = (uint32_t *)(&p_ctrl->p_reg->ICTDATR0 + device_index * address_offset);
#elif(__ICCRX__)
        /* Pointer to address of ICTDATRN in IAR compiler. */
        p_ictdatrn_reg = (uint32_t R_BSP_VOLATILE_SFR *)(&p_ctrl->p_reg->ICTDATR0.LONG + device_index * address_offset);
#endif
        /* Configure the IBI settings for this device. */
        /* Cast to 32 bit unsigned integer to match 32 bit register. */
        ictdatrn |= (uint32_t)(p_device_table_cfg->ibi_payload << R_RI3C0_ICTDATR0_IBIPL_Pos) &
                    R_RI3C0_ICTDATR0_IBIPL_Msk;

        /* Cast to 32 bit unsigned integer to match 32 bit register. */
        ictdatrn |= (uint32_t)(!p_device_table_cfg->ibi_accept << R_RI3C0_ICTDATR0_TIRRJ_Pos) &
                    R_RI3C0_ICTDATR0_TIRRJ_Msk;

        /* Cast to 32 bit unsigned integer to match 32 bit register. */
        ictdatrn |= (uint32_t)(!p_device_table_cfg->controllerrole_request_accept << R_RI3C0_ICTDATR0_CRRRJ_Pos) &
                    R_RI3C0_ICTDATR0_CRRRJ_Msk;
    }

    /** Configure the device static address. */
    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    ictdatrn |= (uint32_t)(p_device_table_cfg->static_address << R_RI3C0_ICTDATR0_SADR_Pos) & R_RI3C0_ICTDATR0_SADR_Msk;

    /** Configure the device dynamic address. */
    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    ictdatrn |= (uint32_t)(ri3c_address_parity(p_device_table_cfg->dynamic_address) << R_RI3C0_ICTDATR0_DADR_Pos) &
                R_RI3C0_ICTDATR0_DADR_Msk;

    if (RI3C_DEVICE_PROTOCOL_I2C == p_device_table_cfg->device_protocol)
    {
        /* Set the device type so that transfers with this device use the legacy I2C protocol. */
        ictdatrn |= R_RI3C0_ICTDATR0_TYPE_Msk;
    }

    /** Write to the ICTDATRn register. */
    *p_ictdatrn_reg = ictdatrn;

    return FSP_SUCCESS;
#else
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(device_index);
    FSP_PARAMETER_NOT_USED(p_device_table_cfg);

    return FSP_ERR_UNSUPPORTED;
#endif
}
/**********************************************************************************************************************
 * End of function R_RI3C_ControllerDeviceTableSet
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_RI3C_TargetStatusSet
 *******************************************************************************************************************//**
 * @brief Set the status returned to the controller in response to a GETSTATUS command.
 * @param[in] p_api_ctrl Pointer to RI3C control block.
 * @param[in] status New status settings for responding to the GETSTATUS command code.
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_INVALID_MODE           The instance is not in target mode.
 * @retval FSP_ERR_UNSUPPORTED            Target support is disabled.
 * @details Set the status returned to the controller in response to a GETSTATUS command.
 * @note None.
 */
fsp_err_t R_RI3C_TargetStatusSet(ri3c_ctrl_t *const p_api_ctrl, ri3c_device_status_t status)
{
#if RI3C_CFG_TARGET_SUPPORT
    ri3c_instance_ctrl_t * p_ctrl = (ri3c_instance_ctrl_t *)p_api_ctrl; // Cast to functional data type

#if RI3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);
    FSP_ERROR_RETURN(RI3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);

    /** This function is only called in target mode. */
    FSP_ERROR_RETURN(
        RI3C_INTERNAL_STATE_TARGET_IDLE == p_ctrl->internal_state ||
        RI3C_INTERNAL_STATE_TARGET_IBI == p_ctrl->internal_state,
        FSP_ERR_INVALID_MODE);
#endif

    /** Clear the Pending Interrupt and Vendor Reserved fields. */
    uint32_t icdsr = p_ctrl->p_reg->ICDSR.LONG;
    icdsr &= ~(R_RI3C0_ICDSR_PNDINT_Msk | R_RI3C0_ICDSR_VRSV_Msk);

    /** Write the new Pending Interrupt and Vendor Reserved fields. */
    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    icdsr |= (uint32_t)status.pending_interrupt & R_RI3C0_ICDSR_PNDINT_Msk;

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    icdsr |= (uint32_t)(status.vendor_status << R_RI3C0_ICDSR_VRSV_Pos) & R_RI3C0_ICDSR_VRSV_Msk;

    p_ctrl->p_reg->ICDSR.LONG = icdsr;

    return FSP_SUCCESS;
#else
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(status);

    return FSP_ERR_UNSUPPORTED;
#endif
}
/**********************************************************************************************************************
 * End of function R_RI3C_TargetStatusSet
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_RI3C_DeviceSelect
 *******************************************************************************************************************//**
 * @brief In controller mode, select the device for the next transfer.
 * @param[in] p_api_ctrl Pointer to RI3C control block.
 * @param[in] device_index Index into the device table.
 * @param[in] bitrate_mode The bitrate settings for the selected device.
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_INVALID_MODE           This operation is prohibited in target mode.
 * @retval FSP_ERR_UNSUPPORTED            Controller support is disabled.
 * @details In controller mode, select the device for the next transfer.
 * @note This function is not used in target mode.
 */
fsp_err_t R_RI3C_DeviceSelect(ri3c_ctrl_t *const p_api_ctrl, uint32_t device_index, uint32_t bitrate_mode)
{
#if RI3C_CFG_CONTROLLER_SUPPORT
    ri3c_instance_ctrl_t * p_ctrl = (ri3c_instance_ctrl_t *)p_api_ctrl; // Cast to functional data type

#if RI3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);
    FSP_ERROR_RETURN(RI3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(RI3C_BITRATE_MODE_RI3C_SDR4_ICEBR_X4 >= bitrate_mode);
    FSP_ERROR_RETURN(
        RI3C_INTERNAL_STATE_TARGET_IDLE != p_ctrl->internal_state && RI3C_INTERNAL_STATE_DISABLED != p_ctrl->internal_state,
        FSP_ERR_INVALID_MODE);
    FSP_ASSERT(BSP_FEATURE_RI3C_MAX_DEV_COUNT > device_index ||
               RI3C_DEVICE_INDEX_EXTENDED_DEVICE == device_index);
#endif

    p_ctrl->device_index        = device_index;
    p_ctrl->device_bitrate_mode = (ri3c_bitrate_mode_t)bitrate_mode; // Cast to bit rate data type

    return FSP_SUCCESS;
#else
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(device_index);
    FSP_PARAMETER_NOT_USED(bitrate_mode);

    return FSP_ERR_UNSUPPORTED;
#endif
}
/**********************************************************************************************************************
 * End of function R_RI3C_DeviceSelect
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_RI3C_DynamicAddressAssignmentStart
 *******************************************************************************************************************//**
 * @brief Start the Dynamic Address Assignment Process.
 * @param[in] p_api_ctrl Pointer to RI3C control block.
 * @param[in] address_assignment_mode The command to use for Dynamic Address Assignment.
 * @param[in] starting_device_index The device index that will be used to assign the first device during
 *            Dynamic Address Assignment.
 * @param[in] device_count The number of devices to assign (Only used with I3C_ADDRESS_ASSIGNMENT_MODE_ENTDAA).
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL or invalid.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_INVALID_MODE           This operation is prohibited in target mode.
 * @retval FSP_ERR_IN_USE                 The operation could not be completed because the driver is busy.
 * @retval FSP_ERR_UNSUPPORTED            Controller support is disabled.
 * @details Start the Dynamic Address Assignment Process.
 * @note None.
 */
fsp_err_t R_RI3C_DynamicAddressAssignmentStart(ri3c_ctrl_t *const p_api_ctrl,
                                               ri3c_address_assignment_mode_t address_assignment_mode,
                                               uint32_t starting_device_index,
                                               uint32_t device_count)
{
#if RI3C_CFG_CONTROLLER_SUPPORT
    ri3c_instance_ctrl_t * p_ctrl = (ri3c_instance_ctrl_t *)p_api_ctrl; // Cast to functional data type

#if RI3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);
    FSP_ERROR_RETURN(RI3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(
        RI3C_INTERNAL_STATE_TARGET_IDLE != p_ctrl->internal_state && \
        RI3C_INTERNAL_STATE_DISABLED != p_ctrl->internal_state,
        FSP_ERR_INVALID_MODE);

    FSP_ASSERT(BSP_FEATURE_RI3C_MAX_DEV_COUNT > starting_device_index ||
               RI3C_DEVICE_INDEX_EXTENDED_DEVICE == starting_device_index);

    if (RI3C_ADDRESS_ASSIGNMENT_MODE_ENTDAA == address_assignment_mode)
    {
        if (RI3C_DEVICE_INDEX_EXTENDED_DEVICE == starting_device_index)
        {
            /** When the extended device index is selected, the device count must be 1. */
            FSP_ASSERT(1U == device_count);
        }
        else
        {
            /** The device count must be less than the remaining devices in the device table. */
            FSP_ASSERT(BSP_FEATURE_RI3C_MAX_DEV_COUNT > device_count + starting_device_index);
        }
    }
    else
    {
        /** If the dynamic address is configured with SETDASA then the device count is not used and should be set to 0. */
        FSP_ASSERT(0U == device_count);
    }
#endif

    /** Ensure that driver is in the idle state. */
    FSP_ERROR_RETURN(RI3C_INTERNAL_STATE_CONTROLLER_IDLE == p_ctrl->internal_state, FSP_ERR_IN_USE);

    p_ctrl->internal_state = RI3C_INTERNAL_STATE_CONTROLLER_ENTDAA;

    /** Set the transfer_data pointer to read the PID, DCR, and BCR registers for each device. */
    p_ctrl->read_buffer_descriptor.p_buffer    = (uint8_t *)&p_ctrl->current_target_info;
    p_ctrl->read_buffer_descriptor.count       = 0;
    p_ctrl->read_buffer_descriptor.buffer_size = sizeof(ri3c_target_info_t);

    /** Setup the command descriptor to start the ENTDAA command starting at the selected device index. */
    uint32_t command_descriptor = 0;
    command_descriptor |= (RI3C_CMD_DESC_CMD_ATTR_ADDR_ASSGN_CMD << RI3C_CMD_DESC_CMD_ATTR_Pos);

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    command_descriptor |= (uint32_t)(RI3C_EVENT_ADDRESS_ASSIGNMENT_COMPLETE << RI3C_CMD_DESC_TID_Pos);

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    command_descriptor |= (uint32_t)(address_assignment_mode << RI3C_CMD_DESC_CMD_Pos);

    command_descriptor |= (starting_device_index << RI3C_CMD_DESC_DEV_INDEX_Pos);
    command_descriptor |= (device_count << RI3C_CMD_DESC_ADDR_ASSGN_DEV_COUNT_Pos);
    command_descriptor |= RI3C_CMD_DESC_ROC_Msk;
    command_descriptor |= RI3C_CMD_DESC_TOC_Msk;

    /*
     * Write to the descriptor to the command queue.
     * Note that the command descriptor is two words. The least significant word must be written first followed by
     * the most significant word (See Section 35.3.1.1 in the RX26T manual).
     */
    p_ctrl->p_reg->ICCQR = command_descriptor;
    p_ctrl->p_reg->ICCQR = 0U;

    /* Clear the command queue empty flag. */
    p_ctrl->p_reg->ICCSR.BIT.CQEF = 0;

    return FSP_SUCCESS;
#else
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(address_assignment_mode);
    FSP_PARAMETER_NOT_USED(starting_device_index);
    FSP_PARAMETER_NOT_USED(device_count);

    return FSP_ERR_UNSUPPORTED;
#endif
}
/**********************************************************************************************************************
 * End of function R_RI3C_DynamicAddressAssignmentStart
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_RI3C_CommandSend
 *******************************************************************************************************************//**
 * @brief Send a broadcast or direct command to target devices on the bus.
 * @param[in] p_api_ctrl Pointer to RI3C control block.
 * @param[in] p_command_descriptor A descriptor for executing the command.
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_IN_USE                 The operation could not be completed because the driver is busy.
 * @retval FSP_ERR_INVALID_MODE           This driver is not in controller mode.
 * @retval FSP_ERR_INVALID_ALIGNMENT      The buffer must be aligned to 4 bytes. If it is a read operation, the length
 *                                        also be a multiple of 4 bytes.
 * @retval FSP_ERR_UNSUPPORTED            Controller support must be enabled to call this function.
 *                                        Target support must be enabled when sending the GETACCMST command.
 * @details Send a broadcast or direct command to target devices on the bus.
 * @note None.
 */
fsp_err_t R_RI3C_CommandSend(ri3c_ctrl_t *const p_api_ctrl, ri3c_command_descriptor_t *p_command_descriptor)
{
#if RI3C_CFG_CONTROLLER_SUPPORT
    ri3c_instance_ctrl_t * p_ctrl = (ri3c_instance_ctrl_t *)p_api_ctrl; // Cast to functional data type

#if RI3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);
    FSP_ERROR_RETURN(RI3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(NULL != p_command_descriptor);
    if (p_command_descriptor->length > 0)
    {
        FSP_ASSERT(NULL != p_command_descriptor->p_buffer);
    }

    FSP_ERROR_RETURN(
        RI3C_INTERNAL_STATE_TARGET_IDLE != p_ctrl->internal_state && \
        RI3C_INTERNAL_STATE_DISABLED != p_ctrl->internal_state,
        FSP_ERR_INVALID_MODE);

#if !RI3C_CFG_UNALIGNED_BUFFER_SUPPORT

    /** Verify that the buffer is aligned to 4 bytes. */
    FSP_ERROR_RETURN(0U == ((uint32_t)p_command_descriptor->p_buffer & 0x03U), FSP_ERR_INVALID_ALIGNMENT);
#endif
#endif

    /** The driver does not currently support relinquishing controller role request to secondary controllers. */
    FSP_ERROR_RETURN(I3C_CCC_DIRECT_GETACCMST != p_command_descriptor->command_code, FSP_ERR_UNSUPPORTED);

    /** Ensure that driver is in the idle state. */
    FSP_ERROR_RETURN(RI3C_INTERNAL_STATE_CONTROLLER_IDLE == p_ctrl->internal_state, FSP_ERR_IN_USE);

    /** Save the current command code. */
    p_ctrl->current_command_code = p_command_descriptor->command_code;

    ri3c_read_buffer_descriptor_t *p_transfer_descriptor;

#if RI3C_CFG_TARGET_SUPPORT
    if (I3C_CCC_DIRECT_GETACCMST == p_command_descriptor->command_code)
    {
        /** Disable the IBI Write Buffer Empty/Full IRQ in case the driver transisitons to target mode. */
        p_ctrl->p_reg->ICCSIER.BIT.IQEFIE = 0;
    }
#endif

    if (p_command_descriptor->rnw)
    {
        /*z* If this is a read transfer, update the read buffer descriptor. */
        p_transfer_descriptor                      = &p_ctrl->read_buffer_descriptor;
        p_transfer_descriptor->read_request_issued = false;

        p_ctrl->internal_state = RI3C_INTERNAL_STATE_CONTROLLER_COMMAND_READ;
    }
    else
    {
        /** If this is a write transfer, update the write buffer descriptor. */
        p_transfer_descriptor = (ri3c_read_buffer_descriptor_t *)&p_ctrl->write_buffer_descriptor;

        p_ctrl->internal_state = RI3C_INTERNAL_STATE_CONTROLLER_COMMAND_WRITE;
    }

    /** Update the buffer descriptor with the buffer provided in the command descriptor. */
    p_transfer_descriptor->p_buffer    = p_command_descriptor->p_buffer;
    p_transfer_descriptor->count       = 0;
    p_transfer_descriptor->buffer_size = p_command_descriptor->length;

    /** Calculate the command descriptor. */
    uint32_t command_descriptor = 0;
    command_descriptor |= (p_ctrl->device_index << RI3C_CMD_DESC_DEV_INDEX_Pos) & RI3C_CMD_DESC_DEV_INDEX_Msk;
    command_descriptor |= (0 << RI3C_CMD_DESC_XFER_MODE_Pos) & RI3C_CMD_DESC_XFER_MODE_Msk;

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    command_descriptor |= (uint32_t)(p_command_descriptor->rnw << RI3C_CMD_DESC_XFER_RNW_Pos);
    command_descriptor |= RI3C_CMD_DESC_ROC_Msk;

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    command_descriptor |= (uint32_t)(!p_command_descriptor->restart << RI3C_CMD_DESC_TOC_Pos) & RI3C_CMD_DESC_TOC_Msk;

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    command_descriptor |= (uint32_t)(p_command_descriptor->command_code << RI3C_CMD_DESC_CMD_Pos);
    command_descriptor |= (1U << 15U);

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    command_descriptor |= (uint32_t)(RI3C_EVENT_COMMAND_COMPLETE << RI3C_CMD_DESC_TID_Pos);

    /*
     * Write the descriptor to the command queue.
     * Note that the command descriptor is two words. The least significant word must be written first followed by
     * the most significant word (See Section 35.3.1.1, point (1) in the RX26T manual).
     */
    p_ctrl->p_reg->ICCQR = command_descriptor;
    p_ctrl->p_reg->ICCQR = (p_command_descriptor->length << RI3C_CMD_DESC_XFER_LENGTH_Pos) &
                           RI3C_CMD_DESC_XFER_LENGTH_Msk;

    /** Clear the command queue empty flag. */
    p_ctrl->p_reg->ICCSR.BIT.CQEF = 0;

    if (!p_command_descriptor->rnw && (0 < p_command_descriptor->length))
    {
        /** Calculate the next data word that will be written to the FIFO. */
        p_ctrl->next_word = ri3c_next_data_word_calculate(&p_ctrl->write_buffer_descriptor);

        /** Write data to the FIFO. */
        ri3c_fifo_write(p_ctrl);

        /** If there is still data remaining in the transfer then it will be written in the Write Buffer Empty IRQ. */
        if ((BSP_FEATURE_RI3C_ICDR_DEPTH * sizeof(uint32_t)) < p_command_descriptor->length)
        {
            /** Enable the Write Buffer Empty IRQ. */
            p_ctrl->p_reg->ICCSIER.BIT.TIE = 1;
        }
    }

    return FSP_SUCCESS;
#else
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(p_command_descriptor);

    return FSP_ERR_UNSUPPORTED;
#endif
}
/**********************************************************************************************************************
 * End of function R_RI3C_CommandSend
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_RI3C_Write
 *******************************************************************************************************************//**
 * @brief Set the write buffer for the transfer. In controller mode, start the transfer.
 *  When the transfer is completed send a stop condition or a repeated-start.
 * @param[in] p_api_ctrl Pointer to RI3C control block.
 * @param[in] p_data Pointer to a buffer to write.
 * @param[in] length Number of bytes to transfer.
 * @param[in] restart If true, issue a repeated-start after the transfer is completed (Controller only).
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_IN_USE                 The operation could not be completed because the driver is busy.
 * @retval FSP_ERR_INVALID_MODE           This driver is disabled.
 * @retval FSP_ERR_INVALID_ALIGNMENT      The buffer must be aligned to 4 bytes.
 * @details Set the write buffer for the transfer. In controller mode, start the transfer.
 *  When the transfer is completed send a stop condition or a repeated-start.
 * @note None.
 */
fsp_err_t R_RI3C_Write(ri3c_ctrl_t *const p_api_ctrl, uint8_t const *const p_data, uint32_t length, bool restart)
{
    ri3c_instance_ctrl_t * p_ctrl = (ri3c_instance_ctrl_t *)p_api_ctrl; // Cast to functional data type

#if 0U == RI3C_CFG_CONTROLLER_SUPPORT
    FSP_PARAMETER_NOT_USED(restart);
#endif

#if RI3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);
    FSP_ERROR_RETURN(RI3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(NULL != p_data);
    FSP_ERROR_RETURN(RI3C_INTERNAL_STATE_DISABLED != p_ctrl->internal_state, FSP_ERR_INVALID_MODE);
#if !RI3C_CFG_UNALIGNED_BUFFER_SUPPORT

    /* Verify that the buffer is aligned to 4 bytes. */
    FSP_ERROR_RETURN(0U == ((uint32_t)p_data & 0x03U), FSP_ERR_INVALID_ALIGNMENT);
#endif
#endif

#if RI3C_CFG_CONTROLLER_SUPPORT
    /* In controller mode, ensure that driver is in the idle state. */
    FSP_ERROR_RETURN(RI3C_INTERNAL_STATE_CONTROLLER_IDLE == p_ctrl->internal_state, FSP_ERR_IN_USE);
#endif

    /* Set the internal transfer state for a write transfer. */
    p_ctrl->write_buffer_descriptor.count       = 0;
    p_ctrl->write_buffer_descriptor.buffer_size = length;
    p_ctrl->write_buffer_descriptor.p_buffer    = (uint8_t *)p_data;

    /* Calculate the next data word that will be written to the FIFO. */
    p_ctrl->next_word = ri3c_next_data_word_calculate(&p_ctrl->write_buffer_descriptor);

#if RI3C_CFG_TARGET_SUPPORT
    if (RI3C_INTERNAL_STATE_TARGET_IDLE == p_ctrl->internal_state)
    {
        if (p_ctrl->write_buffer_descriptor.p_buffer != NULL)
        {
            /*
             * If a write buffer has already been configured, then the FIFO needs to be reset before the new data
             * can be written.
             */
            p_ctrl->p_reg->ICRCR.LONG = R_RI3C0_ICRCR_TBRST_Msk;

            /* The field will be cleared automatically upon reset completion. */
            /* WAIT_LOOP */
            while (p_ctrl->p_reg->ICRCR.BIT.MRST != 0U)
            {
                /* Wait. */
            }
        }

        /* Configure the number of bytes that the target will write before ending the transfer via the T bit. */
        p_ctrl->p_reg->ICTDLR.LONG = (length << 16U);
    }
#endif

    /* Write data to the FIFO. */
    ri3c_fifo_write(p_ctrl);

    /* If there is still data remaining in the transfer then it will be written in the Write Buffer Empty IRQ. */
    if ((BSP_FEATURE_RI3C_ICDR_DEPTH * sizeof(uint32_t)) < length)
    {
        /* Enable the Write Buffer Empty IRQ. */
        p_ctrl->p_reg->ICCSIER.BIT.TIE = 1;
    }

#if RI3C_CFG_CONTROLLER_SUPPORT
    if (RI3C_INTERNAL_STATE_CONTROLLER_IDLE == p_ctrl->internal_state)
    {
        p_ctrl->internal_state = RI3C_INTERNAL_STATE_CONTROLLER_WRITE;

        /*
         * Write the descriptor to the command queue.
         * Note that the command descriptor is two words. The least significant word must be written first followed by
         * the most significant word (See Section 35.3.1.1 in the RX26T manual).
         */
        p_ctrl->p_reg->ICCQR = ri3c_xfer_command_calculate(
            p_ctrl->device_index, 
            false, 
            p_ctrl->device_bitrate_mode, 
            restart);
        p_ctrl->p_reg->ICCQR = (length << RI3C_CMD_DESC_XFER_LENGTH_Pos) & RI3C_CMD_DESC_XFER_LENGTH_Msk;

        /* Clear the command queue empty flag. */
        p_ctrl->p_reg->ICCSR.BIT.CQEF = 0;
    }
#endif

    return FSP_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_RI3C_Write
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_RI3C_Read
 *******************************************************************************************************************//**
 * @brief Set the read buffer for the transfer. In controller mode, start the transfer. When the transfer is completed
 *  send a stop condition or a repeated-start.
 * @param[in] p_api_ctrl Pointer to RI3C control block.
 * @param[in] p_data Pointer to a buffer to store the bytes read during the transfer.
 * @param[in] length Number of bytes to transfer.
 * @param[in] restart If true, issue a repeated-start after the transfer is completed (Controller only).
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_IN_USE                 The operation could not be completed because the driver is busy.
 * @retval FSP_ERR_INVALID_MODE           This driver is disabled.
 * @retval FSP_ERR_INVALID_ALIGNMENT      The buffer must be aligned to 4 bytes and the length must be a multiple of
 *                                        4 bytes.
 * @details Set the read buffer for the transfer. In controller mode, start the transfer. When the transfer is completed
 *  send a stop condition or a repeated-start.
 * @note None.
 */
fsp_err_t R_RI3C_Read(ri3c_ctrl_t *const p_api_ctrl, uint8_t *const p_data, uint32_t length, bool restart)
{
    ri3c_instance_ctrl_t * p_ctrl = (ri3c_instance_ctrl_t *)p_api_ctrl; // Cast to functional data type

#if 0U == RI3C_CFG_CONTROLLER_SUPPORT
    FSP_PARAMETER_NOT_USED(restart);
#endif

#if RI3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);
    FSP_ERROR_RETURN(RI3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(NULL != p_data);
    FSP_ERROR_RETURN(RI3C_INTERNAL_STATE_DISABLED != p_ctrl->internal_state, FSP_ERR_INVALID_MODE);
#if !RI3C_CFG_UNALIGNED_BUFFER_SUPPORT

    /* The buffer must be 4 byte aligned. */
    FSP_ERROR_RETURN(0U == ((uint32_t)p_data & 0x03U), FSP_ERR_INVALID_ALIGNMENT);

    if (RI3C_INTERNAL_STATE_TARGET_IDLE == p_ctrl->internal_state)
    {
        /* The buffer size must be a multiple of 4 bytes in target mode. */
        FSP_ERROR_RETURN(0U == ((uint32_t)length & 0x03U), FSP_ERR_INVALID_ALIGNMENT);
    }
#endif
#endif

    /* In Controller mode, ensure that driver is in the idle state. */
    FSP_ERROR_RETURN(RI3C_INTERNAL_STATE_TARGET_IDLE == p_ctrl->internal_state || 
        RI3C_INTERNAL_STATE_CONTROLLER_IDLE == p_ctrl->internal_state, FSP_ERR_IN_USE);

    bsp_int_ctrl_t int_ctrl;

    /* Disable the Receive Buffer Full IRQ in order to ensure that updating the read buffer state is not interrupted. */
    /** Disable the Interrupt request source */
    R_BSP_InterruptRequestDisable(VECT(RI3C0, RXI));
    
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);

    IEN(RI3C0, RXI) = 0; // ICU.IER[IER_RI3C0_RXI].BIT.IEN6 = 0;

    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);

    /* Set the internal transfer state for a read transfer. */
    p_ctrl->read_buffer_descriptor.count               = 0;
    p_ctrl->read_buffer_descriptor.buffer_size         = length;
    p_ctrl->read_buffer_descriptor.p_buffer            = p_data;
    p_ctrl->read_buffer_descriptor.read_request_issued = false;

    R_BSP_InterruptRequestEnable(VECT(RI3C0, RXI));
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);

    IEN(RI3C0, RXI) = 1; // ICU.IER[IER_RI3C0_RXI].BIT.IEN6 = 1;

    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);

#if RI3C_CFG_CONTROLLER_SUPPORT
    if (RI3C_INTERNAL_STATE_CONTROLLER_IDLE == p_ctrl->internal_state)
    {
        p_ctrl->internal_state = RI3C_INTERNAL_STATE_CONTROLLER_READ;

        /*
         * Write the descriptor to the command queue.
         * Note that the command descriptor is two words. The least significant word must be written first followed by
         * the most significant word (See Section 35.3.1.1 in the RX26T manual).
         */
        p_ctrl->p_reg->ICCQR = ri3c_xfer_command_calculate(p_ctrl->device_index,
                                                           true,
                                                           p_ctrl->device_bitrate_mode,
                                                           restart);
        p_ctrl->p_reg->ICCQR = (length << RI3C_CMD_DESC_XFER_LENGTH_Pos) & RI3C_CMD_DESC_XFER_LENGTH_Msk;

        /* Clear the command queue empty flag. */
        p_ctrl->p_reg->ICCSR.BIT.CQEF = 0;
    }
#endif

    return FSP_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_RI3C_Read
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_RI3C_IbiWrite
 *******************************************************************************************************************//**
 * @brief Initiate an IBI write operation.
 * @param[in] p_api_ctrl Pointer to RI3C control block.
 * @param[in] ibi_type Type of In-band Interrupt.
 * @param[in] p_data Pointer to a buffer to start the bytes read during the transfer.
 * @param[in] length Number of bytes to transfer.
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_IN_USE                 The operation could not be completed because the driver is busy.
 * @retval FSP_ERR_INVALID_MODE           This function is only called in target mode.
 * @retval FSP_ERR_INVALID_ALIGNMENT      The buffer must be aligned to 4 bytes.
 * @retval FSP_ERR_UNSUPPORTED            Target support is disabled.
 * @details Initiate an IBI write operation.
 * @note This function is only used in target mode.
 */
fsp_err_t R_RI3C_IbiWrite(ri3c_ctrl_t *const p_api_ctrl,
                          ri3c_ibi_type_t ibi_type,
                          uint8_t const *const p_data,
                          uint32_t length)
{
#if RI3C_CFG_TARGET_SUPPORT
    ri3c_instance_ctrl_t * p_ctrl = (ri3c_instance_ctrl_t *)p_api_ctrl; // Cast to functional data type
#if RI3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);
    FSP_ERROR_RETURN(RI3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);

    /* If the device has an IBI payload, then a buffer is required. */
    if ((RI3C_IBI_TYPE_INTERRUPT == ibi_type) && (0 != (R_RI3C0_ICDAR0_IBIPL_Msk & p_ctrl->p_reg->ICDAR0.LONG)))
    {
        FSP_ASSERT(NULL != p_data);
        FSP_ASSERT(0 != length);
    }

    /* This function is only called in target mode. */
    FSP_ERROR_RETURN(RI3C_INTERNAL_STATE_TARGET_IDLE == p_ctrl->internal_state, FSP_ERR_INVALID_MODE);

#if !RI3C_CFG_UNALIGNED_BUFFER_SUPPORT

    /* The buffer must be 4 byte aligned. */
    FSP_ERROR_RETURN(0U == ((uint32_t)p_data & 0x03U), FSP_ERR_INVALID_ALIGNMENT);
#endif
#endif

    /* An IBI can't be started if an IBI is already in progress. */
    FSP_ERROR_RETURN(RI3C_INTERNAL_STATE_TARGET_IBI != p_ctrl->internal_state, FSP_ERR_IN_USE);

    p_ctrl->internal_state = RI3C_INTERNAL_STATE_TARGET_IBI;

    /* Setup the buffer for writing the data portion of the IBI. */
    p_ctrl->ibi_buffer_descriptor.p_buffer    = (uint8_t *)p_data;
    p_ctrl->ibi_buffer_descriptor.count       = 0;
    p_ctrl->ibi_buffer_descriptor.buffer_size = length;

    /* Calculate the command descriptor for starting an IBI. */
    uint32_t command_descriptor = 0;
    command_descriptor |= RI3C_CMD_DESC_ROC_Msk;
    command_descriptor |= (RI3C_EVENT_IBI_WRITE_COMPLETE << RI3C_CMD_DESC_TID_Pos);

    if (RI3C_IBI_TYPE_HOT_JOIN == ibi_type)
    {
        command_descriptor |= RI3C_CMD_DESC_XFER_HJ_Msk;
    }
    else if (RI3C_IBI_TYPE_INTERRUPT == ibi_type)
    {
        command_descriptor |= RI3C_CMD_DESC_XFER_RNW_Msk;
    }
    else
    {
        /* Controller Role request. */
    }

    if (0 < length)
    {
        /* Write the data to the IBI queue. */
        p_ctrl->p_reg->ICIQR = ri3c_next_data_word_calculate(
            (ri3c_write_buffer_descriptor_t *)&p_ctrl->ibi_buffer_descriptor);

        if (sizeof(uint32_t) < length)
        {
            /* If the transfer length is greater than 4 bytes, then prepare the next word to be written to the FIFO. */
            p_ctrl->ibi_next_word = ri3c_next_data_word_calculate(
                (ri3c_write_buffer_descriptor_t *)&p_ctrl->ibi_buffer_descriptor);

            /* Enable the IBI Write Buffer Empty/Full IRQ. */
            p_ctrl->p_reg->ICCSIER.BIT.IQEFIE = 1;
        }

        /* Clear the IBI Queue Empty/Full Flag (This will generate an IBI IRQ). */
        p_ctrl->p_reg->ICCSR.BIT.IQEFF = 0;
    }

    /*
     * Write the descriptor to the command queue.
     * Note that the command descriptor is two words. The least significant word must be written first followed by
     * the most significant word (See Section 35.3.1.1 in the RX26T manual).
     */
    p_ctrl->p_reg->ICCQR = command_descriptor;
    p_ctrl->p_reg->ICCQR = (length << RI3C_CMD_DESC_XFER_LENGTH_Pos) & RI3C_CMD_DESC_XFER_LENGTH_Msk;

    /* Clear the command queue empty flag. */
    p_ctrl->p_reg->ICCSR.BIT.CQEF = 0;

    return FSP_SUCCESS;
#else
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(ibi_type);
    FSP_PARAMETER_NOT_USED(p_data);
    FSP_PARAMETER_NOT_USED(length);

    return FSP_ERR_UNSUPPORTED;
#endif
}
/**********************************************************************************************************************
 * End of function R_RI3C_IbiWrite
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_RI3C_IbiRead
 *******************************************************************************************************************//**
 * @brief Set the read buffer for storing received IBI data.
 * @param[in] p_api_ctrl Pointer to RI3C control block.
 * @param[in] p_data Pointer to a buffer to start the bytes read during the transfer.
 * @param[in] length Number of bytes to transfer.
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @retval FSP_ERR_INVALID_MODE           This function is only called in controller mode.
 * @retval FSP_ERR_INVALID_ALIGNMENT      The buffer must be aligned to 4 bytes and the length must be a multiple of
 *                                        4 bytes.
 * @retval FSP_ERR_UNSUPPORTED            Controller support is disabled.
 * @details Set the read buffer for storing received IBI data.
 * @note This function is only used in controller mode.
 */
fsp_err_t R_RI3C_IbiRead(ri3c_ctrl_t *const p_api_ctrl, uint8_t *const p_data, uint32_t length)
{
#if RI3C_CFG_CONTROLLER_SUPPORT
    ri3c_instance_ctrl_t * p_ctrl = (ri3c_instance_ctrl_t *)p_api_ctrl; // Cast to functional data type

#if RI3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);
    FSP_ERROR_RETURN(RI3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(NULL != p_data);
    FSP_ASSERT(0 != length);

    /* This function is not used in target mode. */
    FSP_ERROR_RETURN(RI3C_INTERNAL_STATE_TARGET_IDLE != p_ctrl->internal_state &&
                     RI3C_INTERNAL_STATE_TARGET_IBI != p_ctrl->internal_state &&
                     RI3C_INTERNAL_STATE_DISABLED != p_ctrl->internal_state,
                     FSP_ERR_INVALID_MODE);

#if !RI3C_CFG_UNALIGNED_BUFFER_SUPPORT

    /* The buffer must be 4 byte aligned. */
    FSP_ERROR_RETURN(0U == ((uint32_t)p_data & 0x03U), FSP_ERR_INVALID_ALIGNMENT);

    /* The buffer size must be a multiple of 4 bytes. */
    FSP_ERROR_RETURN(0U == ((uint32_t)length & 0x03U), FSP_ERR_INVALID_ALIGNMENT);
#endif
#endif
    
    ri3c_extended_cfg_t * p_extend = (ri3c_extended_cfg_t *)p_ctrl->p_cfg->p_extend; // Cast to functional data type
    
    bsp_int_ctrl_t int_ctrl;

    /* Disable the IBI Buffer Full IRQ in order to ensure that updating the IBI buffer state is not interrupted. */
    /** Disable the Interrupt request source */
    R_BSP_InterruptRequestDisable(VECT(RI3C0, IBII));

    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);

    IEN(RI3C0, IBII) = 0; // ICU.IER[IER_RI3C0_IBII].BIT.IEN2 = 0;

    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);

    p_ctrl->ibi_buffer_descriptor.p_buffer            = p_data;
    p_ctrl->ibi_buffer_descriptor.count               = 0;
    p_ctrl->ibi_buffer_descriptor.buffer_size         = length;
    p_ctrl->ibi_buffer_descriptor.read_request_issued = false;

    R_BSP_InterruptRequestEnable(VECT(RI3C0, IBII));
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);

    IEN(RI3C0, IBII) = 1; // ICU.IER[IER_RI3C0_IBII].BIT.IEN2 = 1;

    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);

    IPR(RI3C0, IBII) = p_extend->ipl; // ICU.IPR[IPR_RI3C0_IBII].BIT.IPR = p_extend->ipl;

    return FSP_SUCCESS;
#else
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(p_data);
    FSP_PARAMETER_NOT_USED(length);

    return FSP_ERR_UNSUPPORTED;
#endif
}
/**********************************************************************************************************************
 * End of function R_RI3C_IbiRead
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_RI3C_Close
 *******************************************************************************************************************//**
 * @brief Close the RI3C instance.
 * @param[in] p_api_ctrl Pointer to RI3C control block.
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              An argument was NULL.
 * @retval FSP_ERR_NOT_OPEN               This instance has not been opened yet.
 * @details Close the RI3C instance.
 * @note None.
 */
fsp_err_t R_RI3C_Close(ri3c_ctrl_t *const p_api_ctrl)
{
    ri3c_instance_ctrl_t * p_ctrl = (ri3c_instance_ctrl_t *)p_api_ctrl; // Cast to functional data type

#if RI3C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_api_ctrl);
    FSP_ERROR_RETURN(RI3C_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    bsp_int_ctrl_t int_ctrl;

    /* Disable RI3C IRQs. */
    /** Disable RI3C RESPI */
    R_BSP_InterruptRequestDisable(VECT(RI3C0, RESPI));
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);

    IEN(RI3C0, RESPI) = 0; // ICU.IER[IER_RI3C0_RESPI].BIT.IEN0 = 0;

    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
    
    /** Disable RI3C RXI */
    R_BSP_InterruptRequestDisable(VECT(RI3C0, RXI));
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);

    IEN(RI3C0, RXI) = 0; // ICU.IER[IER_RI3C0_RXI].BIT.IEN6 = 0;

    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
    
    /** Disable RI3C TXI */
    R_BSP_InterruptRequestDisable(VECT(RI3C0, TXI));
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);

    IEN(RI3C0, TXI) = 0; // ICU.IER[IER_RI3C0_TXI].BIT.IEN7 = 0;

    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
    
    /** Disable RI3C RCVI */
    R_BSP_InterruptRequestDisable(VECT(RI3C0, RCVI));
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);

    IEN(RI3C0, RCVI) = 0; // ICU.IER[IER_RI3C0_RCVI].BIT.IEN3 = 0;

    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
    
    /** Disable RI3C IBII */
    R_BSP_InterruptRequestDisable(VECT(RI3C0, IBII));
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);

    IEN(RI3C0, IBII) = 0; // ICU.IER[IER_RI3C0_IBII].BIT.IEN2 = 0;

    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);

    /* Reset the RI3C Peripheral so that it is in a known state. */
    p_ctrl->p_reg->ICCR.BIT.ICE = 0;
    p_ctrl->p_reg->ICRCR.LONG   = 1U;

    /* The field will be cleared automatically upon reset completion (See section 35.2.4 in the RX26T manual). */
    /* WAIT_LOOP */
    while (0U != p_ctrl->p_reg->ICRCR.BIT.MRST)
    {
        /* Wait. */
    }

    /* Set the RI3C Module Stop bit. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Set IPL to the maximum value to disable all interrupts,
     * so the scheduler can not be scheduled in critical region.
     */
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);

    /* Clear the RI3C Module Stop bit.
     * MSPTD5 bit of Module Stop Register D (MSTPCRD) is RI3C Bus Interface 0 Module Stop
     * 0: Release from module-stop state
     * 1: Transition to the module-stop state is made
     */
    MSTP(RI3C0) = 1;

    /* Restore the IPL. */
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    p_ctrl->open = 0;

    return FSP_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_RI3C_Close
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * @brief Initiate Extended configuration for RI3C instance prototype
 **********************************************************************************************************************/
static ri3c_extended_cfg_t ri3c_extended_cfg_init(void)
{
    /* Calculate bitrate variables for g_ri3c0_cfg_extend */
    ri3c_calculateBitRate();

    /* Extended configuration for this instance of RI3C. */
    ri3c_extended_cfg_t ri3c0_cfg_extend =
    {
        .bitrate_settings =
        {
            /* Standard Bitrate */
            .icsbr = ((std_od_high_setting << R_RI3C0_ICSBR_ODHW_Pos) | (std_od_low_setting << R_RI3C0_ICSBR_ODLW_Pos))
                | ((std_pp_high_setting << R_RI3C0_ICSBR_PPHW_Pos) | (std_pp_low_setting << R_RI3C0_ICSBR_PPLW_Pos)),
            /* Extended Bitrate */
            .icebr = ((ext_od_high_setting << R_RI3C0_ICEBR_ODHW_Pos) | (ext_od_low_setting << R_RI3C0_ICEBR_ODLW_Pos))
                | ((ext_pp_high_setting << R_RI3C0_ICEBR_PPHW_Pos) | (ext_pp_low_setting << R_RI3C0_ICEBR_PPLW_Pos)),

            .clock_stalling =
            {
                .assigned_address_phase_enable = RI3C_CFG_ADDRESS_ASSIGNMENT_PHASE,
                .transition_phase_enable = RI3C_CFG_TRANSITION_PHASE,
                .parity_phase_enable = RI3C_CFG_PARITY_PHASE,
                .ack_phase_enable = RI3C_CFG_ACK_PHASE,
                .clock_stalling_time = RI3C_CFG_CLOCK_STALLING_TIME,
            },
        },

        .ibi_control.hot_join_acknowledge = RI3C_CFG_CONTROLLER_ACK_HOTJOIN_REQ,
        .ibi_control.notify_rejected_hot_join_requests = RI3C_CFG_CONTROLLER_NOTIFY_REJECTED_HOTJOIN_REQ,
        .ibi_control.notify_rejected_controllerrole_requests = RI3C_CFG_CONTROLLER_NOTIFY_REJECTED_CONTROLLERROLE_REQ,
        .ibi_control.notify_rejected_interrupt_requests = RI3C_CFG_CONTROLLER_NOTIFY_REJECTED_INTERRUPT_REQ,

        /* Cast to 32 bit unsigned integer to match 32 bit register. */
        .bus_free_detection_time = (uint32_t)RI3C_CFG_BUS_FREE_DETECT_TIME,
        .bus_available_detection_time = RI3C_CFG_BUS_AVAILABLE_CONDITION_DETECT_TIME,
        .bus_idle_detection_time = RI3C_CFG_BUS_IDLE_CONDITION_DETECT_TIME,

        .timeout_detection_enable = RI3C_CFG_TIMEOUT_DETECTION,
            
        .target_command_response_info =
        {
            .inband_interrupt_enable = RI3C_CFG_TARGET_IBI_REQ,
            .controllerrole_request_enable = RI3C_CFG_TARGET_CONTROLLERROLE_REQ,
            .hotjoin_request_enable = RI3C_CFG_TARGET_HOTJOIN_REQ,
            .activity_state = RI3C_CFG_TARGET_ENTER_ACTIVITY_STATE,
            .write_length = RI3C_CFG_TARGET_MAX_WRITE_LENGTH,
            .read_length = RI3C_CFG_TARGET_MAX_READ_LENGTH,
            .ibi_payload_length = RI3C_CFG_TARGET_MAX_IBI_PAYLOAD_LENGTH,
            .write_data_rate = RI3C_CFG_TARGET_WRITE_DATA_RATE,
            .read_data_rate = RI3C_CFG_TARGET_READ_DATA_RATE,
            .read_turnaround_time_enable = RI3C_CFG_TARGET_INCLUDE_MAX_READ_TURNAROUND_TIME,
            .read_turnaround_time = RI3C_CFG_TARGET_MAX_READ_TURNAROUND_TIME,
            .oscillator_frequency = RI3C_CFG_TARGET_FREQUENCY_BYTE,
            .oscillator_inaccuracy = RI3C_CFG_TARGET_INACCURACY_BYTE, 
        },

        .ipl = (RI3C_CFG_INTERRUPT_PRIORITY_LEVEL),
        .eei_ipl = (RI3C_CFG_INTERRUPT_PRIORITY_LEVEL),
    };
    return ri3c0_cfg_extend;
}

/***********************************************************************************************************************
 * @brief Compute the odd parity of an RI3C address and set the parity bit.
 * @param[in] address Number of loops to check bit (at least 1 us per loop).
 * @retval The address with the parity bit set.
 **********************************************************************************************************************/
static uint8_t ri3c_address_parity(uint8_t address)
{
    /* Compute the parity of the dynamic address. */
    uint32_t parity = address;

    /* XOR the upper 4 bits with the lower 4 bits. */
    parity ^= (parity >> 4);

    /* XOR the upper 2 bits of the result with the lower 2 bits. */
    parity ^= (parity >> 2);

    /* XOR the upper bit of the result with the lower bit. */
    parity ^= (parity >> 1);

    /* The result of XOR of all bits is stored in the LSB. */
    parity &= 1;

    /* The ODD parity will be the inverse of the even parity. */
    parity = !parity;

    /* Cast to match 8 bit longs address */
    return (uint8_t)(address | (parity << 7));
}

#if RI3C_CFG_CONTROLLER_SUPPORT

/***********************************************************************************************************************
 * @brief Setup the command descriptor for a read or write transfer to the selected device.
 * @param[in]  dev_index         The selected index for the transfer.
 * @param[in]  rnw               Read not write.
 * @param[in]  bitrate_setting   The bitrate setting (See @ref ri3c_bitrate_mode_t).
 * @param[in]  restart           Terminate the transfer with a repeated start condition.
 * @retval                       The calculated value for the command descriptor.
 **********************************************************************************************************************/
static uint32_t ri3c_xfer_command_calculate(uint32_t dev_index, bool rnw, uint32_t bitrate_setting, bool restart)
{
    uint32_t command_descriptor = 0;
    if (rnw)
    {
        command_descriptor |= (RI3C_EVENT_READ_COMPLETE << RI3C_CMD_DESC_TID_Pos);
    }
    else
    {
        command_descriptor |= (RI3C_EVENT_WRITE_COMPLETE << RI3C_CMD_DESC_TID_Pos);
    }

    command_descriptor |= (dev_index << RI3C_CMD_DESC_DEV_INDEX_Pos) & RI3C_CMD_DESC_DEV_INDEX_Msk;
    command_descriptor |= (bitrate_setting << RI3C_CMD_DESC_XFER_MODE_Pos) & RI3C_CMD_DESC_XFER_MODE_Msk;

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    command_descriptor |= (uint32_t)(rnw << RI3C_CMD_DESC_XFER_RNW_Pos);
    command_descriptor |= RI3C_CMD_DESC_ROC_Msk;

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    command_descriptor |= (uint32_t)(!restart << RI3C_CMD_DESC_TOC_Pos) & RI3C_CMD_DESC_TOC_Msk;

    return command_descriptor;
}

#endif

/***********************************************************************************************************************
 * @brief Read data from the Normal Transfer Data Buffer and write it into the user provided data buffer.
 * @param[in]  p_ctrl                Pointer to an instance's control structure.
 * @param[in]  bytes                 The number of bytes to read from the FIFO.
 **********************************************************************************************************************/
static inline void ri3c_fifo_read(ri3c_instance_ctrl_t *p_ctrl, uint32_t bytes)
{
    /* Cast is acceptable because it is the cast from uint32_t and stdint.h library is included and used */
    int bytes_remaining = (int)bytes;

    /* WAIT_LOOP */
    while (bytes_remaining > 0)
    {
        /* Each entry in the FIFO is 4 bytes. */
        uint32_t rx_data = p_ctrl->p_reg->ICDR.LONG;

        /* Clear the Read Buffer Full status flag. */
        p_ctrl->p_reg->ICCSR.BIT.RDRF = 0;

        /*
         * Store the next word of data into the read buffer. If there is not enough space, a RI3C_EVENT_READ_BUFFER_FULL
         * event will be called to notify the application that a new read buffer is required.
         */
        ri3c_read_buffer_store(p_ctrl,
                              &p_ctrl->read_buffer_descriptor,
                               rx_data,
                               (uint32_t)bytes_remaining,
                               RI3C_EVENT_READ_BUFFER_FULL);

        bytes_remaining -= 4;

        p_ctrl->read_transfer_count_final += 4;
    }
}

/***********************************************************************************************************************
 * @brief Write a word of data read from a FIFO into the read buffer.
 *
 * @param[in]  p_ctrl                Pointer to an instance's control structure.
 * @param[in]  p_buffer_descriptor   Pointer to a read buffer descriptor for storing the word of data.
 * @param[in]  data_word             Word of data read from a FIFO.
 * @param[in]  num_bytes             The number of valid data bytes in the data word.
 * @param[in]  buffer_full_event     The event to notify that application when their is no more space available in the
 *                                   buffer descriptor.
 **********************************************************************************************************************/
static inline void ri3c_read_buffer_store(ri3c_instance_ctrl_t *const p_ctrl,
                                          ri3c_read_buffer_descriptor_t *p_buffer_descriptor,
                                          uint32_t data_word,
                                          uint32_t num_bytes,
                                          ri3c_event_t buffer_full_event)
{
#if !RI3C_CFG_UNALIGNED_BUFFER_SUPPORT
    FSP_PARAMETER_NOT_USED(num_bytes);
#endif

#if RI3C_CFG_UNALIGNED_BUFFER_SUPPORT

    /* If unaligned buffers are supported, the word must be written one byte at a time. */
    /* WAIT_LOOP */
    for (uint32_t i = 0; i < sizeof(uint32_t) && i < num_bytes; i++)
    {
#endif

        /* Check if the buffer descriptor has space available. */
        if (p_buffer_descriptor->count >= p_buffer_descriptor->buffer_size)
        {
            /* If there is no more space available, and the driver already notified the application,
         * then discard the data. */
            if (p_buffer_descriptor->read_request_issued)
            {
                return;
            }

            ri3c_callback_args_t callback_args =
                {
                    .event = buffer_full_event,
                    .p_context = p_ctrl->p_cfg->p_context,
                };

            /* Update the buffer descriptor state so that it will not notify the application again until a new buffer
         * is provided. */
            p_buffer_descriptor->read_request_issued = true;

            /* Notify the application that the read buffer is full. */
            p_ctrl->p_cfg->p_callback(&callback_args);

            /* If the application didn't provide a new read buffer, then discard the data. */
            if (p_buffer_descriptor->read_request_issued)
            {
                return;
            }
        }

#if RI3C_CFG_UNALIGNED_BUFFER_SUPPORT

        /* Write the data to the read buffer one byte at a time. */
        p_buffer_descriptor->p_buffer[p_buffer_descriptor->count++] = (uint8_t)(data_word >> (i * 8U)) & UINT8_MAX;
    }
#endif

#if !RI3C_CFG_UNALIGNED_BUFFER_SUPPORT

    /* Write the data to the read buffer one word at a time. */
    *((uint32_t *)&p_buffer_descriptor->p_buffer[p_buffer_descriptor->count]) = data_word;
    p_buffer_descriptor->count += 4;
#endif
}

/***********************************************************************************************************************
 * @brief Calculate the number of bytes left in the FIFO at the end of a transfer. If the buffer is aligned, 
 * then it is safe to assume that the bytes remaining is a multiple of 4 bytes.
 *
 * @param[in]  p_ctrl                Pointer to an instance's control structure.
 * @param[in]  data_length           The total transfer length read from a status descriptor.
 **********************************************************************************************************************/
static inline uint32_t ri3c_read_bytes_remaining_calculate(ri3c_instance_ctrl_t *p_ctrl, uint32_t data_length)
{
    uint32_t bytes_remaining = 0;
    if (p_ctrl->p_reg->ICDBSR.BIT.RBFL > 0)
    {
        bytes_remaining = data_length - p_ctrl->read_transfer_count_final;
    }

    return bytes_remaining;
}

/***********************************************************************************************************************
 * @brief Fill the transmit FIFO with data from a write buffer descriptor. If there is no more data in the write buffer,
 * then disable the transmit IRQ, and return.
 *
 * @param[in]  p_ctrl                Pointer to an instance's control structure.
 **********************************************************************************************************************/
static inline void ri3c_fifo_write(ri3c_instance_ctrl_t *p_ctrl)
{
    bool transfer_complete = false;
    do
    {
        /* Write data to the transmit FIFO. */
        p_ctrl->p_reg->ICDR.LONG = p_ctrl->next_word;

        /* If there is no more data to write, disable the transmit IRQ and return. */
        if (p_ctrl->write_buffer_descriptor.count >= p_ctrl->write_buffer_descriptor.buffer_size)
        {
            p_ctrl->p_reg->ICCSIER.BIT.TIE = 0;
            IR(RI3C0, TXI)                 = 0;
            transfer_complete              = true;
        }
        else
        {
            /* Calculate the next word of data to write to the FIFO. */
            p_ctrl->next_word = ri3c_next_data_word_calculate(&p_ctrl->write_buffer_descriptor);
        }

        /* Continue writing data until the transmit FIFO is full. */
    } while ((p_ctrl->p_reg->ICDBSR.LONG & UINT8_MAX) && !transfer_complete); /* WAIT_LOOP */

    /* Clear the Transmit Buffer Empty status flag. */
    p_ctrl->p_reg->ICCSR.BIT.TDRE = 0;
}

/***********************************************************************************************************************
 * Compute the value of the next data word in a write transfer.
 *
 * @param[in]  p_buffer_descriptor   Pointer to a write buffer descriptor.
 **********************************************************************************************************************/
static inline uint32_t ri3c_next_data_word_calculate(ri3c_write_buffer_descriptor_t *p_buffer_descriptor)
{
    uint32_t data_word = 0;

    /* Read the current transfer count. */
    uint32_t count                 = p_buffer_descriptor->count;
    uint8_t const *p_transfer_data = p_buffer_descriptor->p_buffer;

#if RI3C_CFG_UNALIGNED_BUFFER_SUPPORT

    /* Compute the value of the next data word that will be written. */
    /* WAIT_LOOP */
    for (uint32_t i = 0; i < sizeof(uint32_t) && count < p_buffer_descriptor->buffer_size; i++)
    {
        /* If there is no more data to write, then exit. */
        data_word |= (uint32_t)(p_transfer_data[count++] << (i * 8));
    }

#else

    /* If the buffer is aligned, then read the data from the buffer using a word access. */
    data_word = *((uint32_t *)(&p_transfer_data[count]));
    count += 4;
#endif

    /* Update the transfer count. */
    p_buffer_descriptor->count = count;

    return data_word;
}

/***********************************************************************************************************************
 * ISR for handling the Command Response IRQ. The Command Response IRQ occurs on completion of a command and
 * provides status information.
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT (ri3c_resp_isr, VECT_RI3C0_RESPI)
R_BSP_ATTRIB_STATIC_INTERRUPT void ri3c_resp_isr(void)
{
    ri3c_instance_ctrl_t *p_ctrl = &g_ri3c0_ctrl;
    IR(RI3C0, RESPI) = 0;

    /* Get the response status from the Response Status Queue. */
    uint32_t response_status_descriptor = p_ctrl->p_reg->ICRQR;

    /* Clear the Response Queue Full status flag. */
    p_ctrl->p_reg->ICCSR.BIT.RQFF = 0;

    /* Read the DATA_LENGTH field in the response descriptor. */
    uint32_t data_length = (response_status_descriptor & RI3C_RESP_STATUS_DESC_DATA_LENGTH_Msk) >>
                           RI3C_RESP_STATUS_DESC_DATA_LENGTH_Pos;

    /* Read the ERR_STATUS field in the response descriptor. */
    uint32_t err_status = (response_status_descriptor & RI3C_RESP_STATUS_DESC_ERR_STATUS_Msk) >>
                          RI3C_RESP_STATUS_DESC_ERR_STATUS_Pos;

    bool error_recovery_case_2 = false;

    uint32_t internal_state = p_ctrl->internal_state;

    ri3c_callback_args_t callback_args;
    callback_args.event        = (ri3c_event_t)(internal_state & RI3C_INTERNAL_EVENT_MASK); // Cast to event data type
    callback_args.event_status = err_status;
    callback_args.p_context    = p_ctrl->p_cfg->p_context;

    p_ctrl->internal_state = RI3C_INTERNAL_STATE_CONTROLLER_IDLE;

    switch (internal_state)
    {
#if RI3C_CFG_CONTROLLER_SUPPORT
        case RI3C_INTERNAL_STATE_CONTROLLER_ENTDAA:
        {
            /* Reset the read buffer descriptor that was used to read the target info. */
            p_ctrl->read_buffer_descriptor = (ri3c_read_buffer_descriptor_t){0};

            break;
        }

        case RI3C_INTERNAL_STATE_CONTROLLER_WRITE:
        case RI3C_INTERNAL_STATE_CONTROLLER_COMMAND_WRITE:
        {
            /* Calculate the total transfer length. Note that for a write transfer, the DATA_LENGTH field in the response
             * descriptor provides the number of bytes remaining. */
            callback_args.transfer_size = p_ctrl->write_buffer_descriptor.buffer_size - data_length;

            /* Reset the write buffer descriptor that was used for the transfer. */
            p_ctrl->write_buffer_descriptor = (ri3c_write_buffer_descriptor_t){0};

            /* Disable the transmit IRQ if it hasn't been disabled already. */
            p_ctrl->p_reg->ICCSIER.BIT.TIE = 0;
            IR(RI3C0, TXI)                 = 0;
            break;
        }

        case RI3C_INTERNAL_STATE_CONTROLLER_READ:
        case RI3C_INTERNAL_STATE_CONTROLLER_COMMAND_READ:
        {
            uint32_t bytes_remaining = ri3c_read_bytes_remaining_calculate(p_ctrl, data_length);

            /* Read the remaining byte stored in the FIFO. */
            ri3c_fifo_read(p_ctrl, bytes_remaining);

            /* If the transfer length is less than expected, the driver must perform error recovery defined in
             * Figure 35.66 in the RX26T manual. */
            if (data_length != p_ctrl->read_buffer_descriptor.buffer_size)
            {
                error_recovery_case_2 = true;
            }

            /*
             * For a read transfer, the DATA_LENGTH field in the response descriptor provides the total number of bytes
             * read.
             */
            callback_args.transfer_size = data_length;

            /* Reset the read buffer descriptor that was used for the read transfer. */
            p_ctrl->read_buffer_descriptor = (ri3c_read_buffer_descriptor_t){
            0};
#if RI3C_CFG_TARGET_SUPPORT
            if (I3C_CCC_DIRECT_GETACCMST == p_ctrl->current_command_code)
            {
                if (0U == (p_ctrl->p_reg->ICMMR.LONG & R_RI3C0_ICMMR_ACF_Msk))
                {
                    /* If the GETACCMST command was successful, transition the driver to target mode. */
                    p_ctrl->internal_state = RI3C_INTERNAL_STATE_TARGET_IDLE;

                    /*
                     * Configure IBI queue threshold so that an IBI IRQ is generated 
                     * when there is one empty entry in the IBI transmit FIFO.
                     */
                    p_ctrl->p_reg->ICQBTCR.BIT.IQTH = 1;
                }
                else
                {
                    /* If the command was not successfull, re-enable the IBI Write Buffer Empty/Full IRQ. */
                    p_ctrl->p_reg->ICCSIER.BIT.IQEFIE = 1;
                }
            }
#endif /* RI3C_CFG_TARGET_SUPPORT */

            p_ctrl->current_command_code = 0;
            break;
        }
#endif /* RI3C_CFG_CONTROLLER_SUPPORT */

#if RI3C_CFG_TARGET_SUPPORT
        case RI3C_INTERNAL_STATE_TARGET_IBI:
        {
            /* Calculate the total transfer length. Note that for an IBI transfer, the DATA_LENGTH field in the response
             * descriptor provides the number of bytes remaining. */
            callback_args.transfer_size = p_ctrl->ibi_buffer_descriptor.buffer_size - data_length;

            /* Reset the buffer descriptor that was used for the IBI transfer. */
            p_ctrl->ibi_buffer_descriptor = (ri3c_read_buffer_descriptor_t){
            0};

            p_ctrl->internal_state = RI3C_INTERNAL_STATE_TARGET_IDLE;
            break;
        }
#endif
        default:
        {
            break;
        }
    }

    /* Reset the total bytes read. */
    p_ctrl->read_transfer_count_final = 0;

    uint32_t iccsr = p_ctrl->p_reg->ICCSR.LONG;

    /*
     * If the transfer was aborted, then the abort flag must be cleared before notifying the application
     * that a transfer has completed.
     */
    if (0 != (iccsr & R_RI3C0_ICCSR_DTAF_Msk))
    {
        p_ctrl->p_reg->ICCR.BIT.ABORT = 0;
    }

    /* If a transfer error occurs, follow the error recovery operation defined in
     * Figure 35.66 and 35.67 in the RX26T manual. */
    if ((0 != (iccsr & R_RI3C0_ICCSR_DTEF_Msk)) || error_recovery_case_2)
    {
#if RI3C_CFG_TARGET_SUPPORT
        if (RI3C_INTERNAL_STATE_TARGET_IDLE == p_ctrl->internal_state)
        {
            ri3c_target_error_recovery(p_ctrl, RI3C_TARGET_ERROR_RECOVERY_TYPE_IBI);
        }
        else
#endif
        {
#if RI3C_CFG_CONTROLLER_SUPPORT
            ri3c_controller_error_recovery(p_ctrl, error_recovery_case_2);
#endif
        }
    }

    /* Clear error status flags. */
    p_ctrl->p_reg->ICCSR.LONG &= ~(R_RI3C0_ICCSR_DTEF_Msk | R_RI3C0_ICCSR_DTAF_Msk);

    /* Notify the application of the event. */
    p_ctrl->p_cfg->p_callback(&callback_args);
}

/***********************************************************************************************************************
 * ISR for handling the Receive Buffer Full IRQ. The Receive Buffer Full IRQ is generated when the number of entries
 * in the Receive Data Buffer is greater than or equal to the configured threshold.
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT (ri3c_rx_isr, VECT_RI3C0_RXI)
R_BSP_ATTRIB_STATIC_INTERRUPT void ri3c_rx_isr(void)
{
    ri3c_instance_ctrl_t *p_ctrl = &g_ri3c0_ctrl;
    IR(RI3C0, RXI) = 0;

    /*
     * If the transfer is complete, the remainder of the Read Data FIFO must be read in the Receive Status Queue Full,
     * or Response Status Queue Full ISRs.
     *
     * This is because in order to read the remaining data, the driver must know exactly how many total bytes were read
     * during the transfer.
     */
    if (0 == (p_ctrl->p_reg->ICCSR.LONG & (R_RI3C0_ICCSR_SQFF_Msk | R_RI3C0_ICCSER_RQFDE_Msk)))
    {
        ri3c_fifo_read(p_ctrl, p_ctrl->p_reg->ICDBSR.BIT.RBFL * sizeof(uint32_t));
    }

#if RI3C_CFG_CONTROLLER_SUPPORT
    if (RI3C_INTERNAL_STATE_CONTROLLER_ENTDAA == p_ctrl->internal_state)
    {
        /* The address phase during ENTDAA occurs after 8 bytes (PID, BCR, and DCR) are read. */
        if (8U == p_ctrl->read_buffer_descriptor.count)
        {
            /* Reset the data pointer after reading each device PID, BCR, and DCR. */
            p_ctrl->read_buffer_descriptor.count = 0;

            /* Notify the applciation that the ENTDAA Address Phase has started. */
            ri3c_callback_args_t callback_args;
            callback_args.event         = RI3C_EVENT_ENTDAA_ADDRESS_PHASE;
            callback_args.p_target_info = &p_ctrl->current_target_info;
            callback_args.p_context     = p_ctrl->p_cfg->p_context;

            p_ctrl->p_cfg->p_callback(&callback_args);
        }
    }
#endif
}

/***********************************************************************************************************************
 * ISR for handling the Transmit Buffer Empty IRQ. The Transmit Buffer Empty IRQ is generated when the number of
 * empties in the Transmit Data Buffer is greater than or equal to the threshold.
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT (ri3c_tx_isr, VECT_RI3C0_TXI)
R_BSP_ATTRIB_STATIC_INTERRUPT void ri3c_tx_isr(void)
{
    ri3c_instance_ctrl_t *p_ctrl = &g_ri3c0_ctrl;
    IR(RI3C0, TXI)               = 0;

    /* Write data to the FIFO. */
    ri3c_fifo_write(p_ctrl);
}

/***********************************************************************************************************************
 * ISR for handling the Receive Status Buffer Full IRQ. The Receive Status Buffer Full IRQ is generated when the
 * number of Receive Status Queue entries is greater than the threshold.
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT (ri3c_rcv_isr, VECT_RI3C0_RCVI)
R_BSP_ATTRIB_STATIC_INTERRUPT void ri3c_rcv_isr(void)
{
#if RI3C_CFG_TARGET_SUPPORT
    ri3c_instance_ctrl_t *p_ctrl = &g_ri3c0_ctrl;
    IR(RI3C0, RCVI) = 0;

    /* Get the receive status descriptor for the Receive Status Queue. */
    uint32_t receive_status_descriptor = p_ctrl->p_reg->ICSQR;

    /* Clear the Receive Status Queue Full Flag. */
    p_ctrl->p_reg->ICCSR.BIT.SQFF = 0;

    /* Read the DATA_LENGTH field in the response descriptor. */
    uint32_t data_length = receive_status_descriptor & RI3C_RECV_STATUS_DESC_DATA_LENGTH_Msk;

    /* Read the ERR_STATUS field in the response descriptor. */
    uint32_t err_status = (receive_status_descriptor & RI3C_RECV_STATUS_DESC_ERR_STATUS_Msk) >>
                          RI3C_RECV_STATUS_DESC_ERR_STATUS_Pos;

    ri3c_callback_args_t callback_args;
    callback_args.p_context    = p_ctrl->p_cfg->p_context;
    callback_args.event_status = err_status;

    ri3c_target_error_recovery_type_t error_recovery_type = RI3C_TARGET_ERROR_RECOVERY_TYPE_OTHER;

    /* The transfer type is a normal SDR transfer. */
    if (0U == (receive_status_descriptor & RI3C_RECV_STATUS_DESC_TRANSFER_TYPE_Msk))
    {
        /* If the transfer is a read transfer. */
        if (0U == (receive_status_descriptor & RI3C_RECV_STATUS_DESC_SDR_R_W_TYPE_Msk))
        {
            /* Calculate the remaining number of bytes stored in the read FIFO. */
            uint32_t bytes_remaining = ri3c_read_bytes_remaining_calculate(p_ctrl, data_length);

            /* Read the remaining bytes stored in the FIFO. */
            ri3c_fifo_read(p_ctrl, bytes_remaining);

            /*
             * If the last transfer was a read, then the transfer length was stored in
             * the receive status descriptor.
             */
            callback_args.transfer_size = data_length;
            callback_args.event         = RI3C_EVENT_READ_COMPLETE;

            /* Reset ther state for read transfers. */
            p_ctrl->read_buffer_descriptor = (ri3c_read_buffer_descriptor_t){
                0};

            /* Set the error recovery procedure to use if there was a transfer error. */
            error_recovery_type = RI3C_TARGET_ERROR_RECOVERY_TYPE_READ;
        }
        else
        {
            /* Calculate the total transfer length. Note that for a write transfer, the DATA_LENGTH field in the receive
             * descriptor provides the number of bytes remaining. */
            callback_args.transfer_size = data_length;
            callback_args.event         = RI3C_EVENT_WRITE_COMPLETE;

            /* Reset the state for write transfers. */
            p_ctrl->write_buffer_descriptor = (ri3c_write_buffer_descriptor_t){
                0};

            /* Set the error recovery procedure to use if there was a transfer error. */
            error_recovery_type = RI3C_TARGET_ERROR_RECOVERY_TYPE_WRITE;

            /* Disable the transmit IRQ if it hasn't been disabled already. */
            p_ctrl->p_reg->ICCSIER.BIT.TIE = 0;
            IR(RI3C0, TXI)                 = 0;
        }
    }
    else
    {
        /*
         * The transfer type is a command.
         *
         * Get the command code from the response status descriptor.
         */
        uint8_t command_code =
            (uint8_t)((receive_status_descriptor & RI3C_RECV_STATUS_DESC_CMD_Msk) >> RI3C_RECV_STATUS_DESC_CMD_Pos);

        if ((I3C_CCC_BROADCAST_ENTDAA == command_code) ||
            (I3C_CCC_DIRECT_SETDASA == command_code))
        {
            uint32_t data_length_w = (data_length + sizeof(uint32_t) - 1) / sizeof(uint32_t);

            /* WAIT_LOOP */
            for (uint32_t i = 0; i < data_length_w; i++)
            {
                /*
                 * Perform dummy read.
                 * See 35.3.2.1 (2) RI3C Target Operation (b) Dynamic Address Assignment Procedure in the RX26T manual.
                 */
                p_ctrl->p_reg->ICDR.LONG;
            }

            /* Reset the state for read transfers. */
            p_ctrl->read_buffer_descriptor = (ri3c_read_buffer_descriptor_t){
                0};

            /* Notify the application that the ENTDAA command has completed. */
            callback_args.event = RI3C_EVENT_ADDRESS_ASSIGNMENT_COMPLETE;

            /* Verify that the target address is valid. */
            if (p_ctrl->p_reg->ICDAMR0.BIT.DAV)
            {
                /* Provide the assigned dynamic address in the callback. */
                uint32_t dynamic_address_w = (p_ctrl->p_reg->ICDAMR0.LONG & R_RI3C0_ICDAMR0_TADR_Msk);

                /* Cast to 8 bit to match 8 bit longs address */
                callback_args.dynamic_address = (uint8_t)(dynamic_address_w >> R_RI3C0_ICDAMR0_TADR_Pos) & UINT8_MAX;
            }
        }
        else
        {
            /* Calculate the remaining number of bytes stored in the read FIFO. */
            uint32_t bytes_remaining = ri3c_read_bytes_remaining_calculate(p_ctrl, data_length);

            /* Read the remaining byte stored in the FIFO. */
            ri3c_fifo_read(p_ctrl, bytes_remaining);

            if (I3C_CCC_DIRECT_GETACCMST == command_code)
            {
                if (0 != (p_ctrl->p_reg->ICMMR.LONG & R_RI3C0_ICMMR_ACF_Msk))
                {
                    /* On success of the GETACCMST command, the driver will transition to controller mode. */
                    p_ctrl->internal_state = RI3C_INTERNAL_STATE_CONTROLLER_IDLE;

                    /*
                     * Configure IBI queue threshold so that an IBI IRQ is generated when there is
                     *  1 IBI status descriptor in the FIFO.
                     */
                    p_ctrl->p_reg->ICQBTCR.BIT.IQTH = 0;

                    /*
                     * In target mode the IBI Queue Empty/Full flag indicates that the queue is empty.
                     * After transitioning to controller mode, this bit indicates if there is status information
                     * in the queue. If the IBI queue was empty before tranistioning to controller mode,
                     * then it will be set and the status should be cleared.
                     */
                    p_ctrl->p_reg->ICCSR.BIT.IQEFF = 0;
                }

                /* Re-enable the IBI Status Buffer Full IRQ. */
                p_ctrl->p_reg->ICCSIER.BIT.IQEFIE = 1;
            }

            if (p_ctrl->read_buffer_descriptor.count > 0)
            {
                /* Reset the state for read transfers if data was read. */
                p_ctrl->read_buffer_descriptor = (ri3c_read_buffer_descriptor_t){
                    0};
            }

            callback_args.event         = RI3C_EVENT_COMMAND_COMPLETE;
            callback_args.command_code  = command_code;
            callback_args.transfer_size = data_length;
        }
    }

    /* Reset the total bytes read. */
    p_ctrl->read_transfer_count_final = 0;

    uint32_t iccsr = p_ctrl->p_reg->ICCSR.LONG;

    /* If an error occurred during the transfer, perform the error recovery operation defined in
     * Figure 35.67 in the RX26T manual.
     */
    if ((0 != (iccsr & (R_RI3C0_ICCSR_DTEF_Msk | R_RI3C0_ICCSR_DTAF_Msk))) && (0U == p_ctrl->p_reg->ICSQSR.BIT.SQFL))
    {
        if (RI3C_INTERNAL_STATE_TARGET_IDLE == p_ctrl->internal_state)
        {
            ri3c_target_error_recovery(p_ctrl, error_recovery_type);
        }
    }

    /* Clear error status flags. */
    p_ctrl->p_reg->ICCSR.LONG &= ~(R_RI3C0_ICCSR_DTEF_Msk | R_RI3C0_ICCSR_DTAF_Msk);

    /* Notify the application of the event. */
    p_ctrl->p_cfg->p_callback(&callback_args);
#endif
}

/***********************************************************************************************************************
 * ISR for writing to the IBI Data Queue in target mode, and reading from the IBI Status Queue in controller mode.
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT (ri3c_ibi_isr, VECT_RI3C0_IBII)
R_BSP_ATTRIB_STATIC_INTERRUPT void ri3c_ibi_isr(void)
{
    ri3c_instance_ctrl_t *p_ctrl = &g_ri3c0_ctrl;
    IR(RI3C0, IBII)              = 0;

#if RI3C_CFG_TARGET_SUPPORT
    if (RI3C_INTERNAL_STATE_TARGET_IBI == p_ctrl->internal_state)
    {
        /* Write the data to the IBI queue. */
        p_ctrl->p_reg->ICIQR = p_ctrl->ibi_next_word;

        /* Clear the IBI Queue Empty/Full Flag. */
        p_ctrl->p_reg->ICCSR.BIT.IQEFF = 0;

        if (p_ctrl->ibi_buffer_descriptor.count >= p_ctrl->ibi_buffer_descriptor.buffer_size)
        {
            /* If there is no more data to write, then disable the IBI IRQ. */
            p_ctrl->p_reg->ICCSIER.BIT.IQEFIE = 0;
            IR(RI3C0, IBII)                   = 0;
        }
        else
        {
            /* Calcaulate the next word of data that will be written to the IBI FIFO. */
            p_ctrl->ibi_next_word = ri3c_next_data_word_calculate(
                (ri3c_write_buffer_descriptor_t *)&p_ctrl->ibi_buffer_descriptor);
        }
    }
    else
#endif
    {
#if RI3C_CFG_CONTROLLER_SUPPORT

        /* Read the IBI status descriptor. */
        uint32_t ibi_status = p_ctrl->p_reg->ICIQR;

        /* Get the number of bytes following the status descriptor. */
        uint32_t ibi_data_length = (ibi_status & RI3C_IBI_STATUS_DESC_LENGTH_Msk) >> RI3C_IBI_STATUS_DESC_LENGTH_Pos;

        /* Update the total number of bytes read during the IBI transfer. */
        p_ctrl->ibi_transfer_count_final += ibi_data_length;

        /* Calculate the number of words that need to be read from the IBI Queue. */
        uint32_t ibi_data_length_w = (ibi_data_length + sizeof(uint32_t) - 1) / sizeof(uint32_t);

        /* WAIT_LOOP */
        for (uint32_t i = 0; i < ibi_data_length_w; i++)
        {
            /* Get the next word of data from the IBI Queue. */
            uint32_t read_data = p_ctrl->p_reg->ICIQR;

            /*
             * Store the next word of data into the read buffer. If there is not enough space,
             * a RI3C_EVENT_IBI_READ_BUFFER_FULL event will be called to notify the application
             * that a new read buffer is required.
             */
            ri3c_read_buffer_store(p_ctrl,
                                   &p_ctrl->ibi_buffer_descriptor,
                                   read_data,
                                   ibi_data_length,
                                   RI3C_EVENT_IBI_READ_BUFFER_FULL);
            ibi_data_length -= 4;
        }

        /* Clear the IBI Queue Empty/Full Flag. */
        p_ctrl->p_reg->ICCSR.BIT.IQEFF = 0;

        /* If this is the last IBI status descriptor, then the IBI transfer is completed. */
        if (0U != (RI3C_IBI_STATUS_DESC_LAST_STATUS_Msk & ibi_status))
        {
            /* Get the address of the IBI transfer. */
            uint8_t ibi_address = (ibi_status & RI3C_IBI_STATUS_DESC_IBI_ID_Msk) >>
                                  (RI3C_IBI_STATUS_DESC_IBI_ID_Pos + 1);

            /* Get the RNW bit of the IBI transfer. */
            uint8_t ibi_rnw = (ibi_status >> RI3C_IBI_STATUS_DESC_IBI_ID_Pos) & 1U;

            uint32_t err_status =
                (ibi_status & (RI3C_IBI_STATUS_DESC_IBI_ST_Msk | RI3C_IBI_STATUS_DESC_ERR_STATUS_Msk)) >>
                RI3C_IBI_STATUS_DESC_ERR_STATUS_Pos;
            ri3c_callback_args_t callback_args =
                {
                    .event = RI3C_EVENT_IBI_READ_COMPLETE,
                    .event_status = err_status,
                    .ibi_type = RI3C_IBI_TYPE_INTERRUPT,
                    .ibi_address = ibi_address,
                    .transfer_size = p_ctrl->ibi_transfer_count_final,
                    .p_context = p_ctrl->p_cfg->p_context
                };

            if (RI3C_HOT_JOIN_ADDRESS == ibi_address)
            {
                callback_args.ibi_type = RI3C_IBI_TYPE_HOT_JOIN;
            }
            else if (0U == ibi_rnw)
            {
                callback_args.ibi_type = RI3C_IBI_TYPE_CONTROLLERROLE_REQUEST;
            }
            else
            {
                /* IBI type is RI3C_IBI_TYPE_INTERRUPT. */
            }

            p_ctrl->ibi_buffer_descriptor = (ri3c_read_buffer_descriptor_t){0};
            p_ctrl->ibi_transfer_count_final = 0;

            /* Notify the application of the event. */
            p_ctrl->p_cfg->p_callback(&callback_args);
        }
#endif
    }
}

/***********************************************************************************************************************
 * ISR for providing the following events to the application:
 * - RI3C_EVENT_INTERNAL_ERROR: An internal error can occur if too many transfers occur sequenctionally and overflow
 *                              the Receive Status Queue.
 * - RI3C_EVENT_TIMEOUT_DETECTED
 * - RI3C_EVENT_HDR_EXIT_PATTERN_DETECTED
 **********************************************************************************************************************/
void ri3c_eei_isr(void)
{
    ri3c_instance_ctrl_t *p_ctrl = &g_ri3c0_ctrl;
    IR(ICU, GROUPAL1) = 0;

    ri3c_callback_args_t callback_args;
    callback_args.p_context = p_ctrl->p_cfg->p_context;

    /* Read the Internal Status Register. */
    uint32_t icisr            = p_ctrl->p_reg->ICISR.LONG;
    uint32_t icisr_clear_mask = 0;

    /* Read the Bus Status Register. */
    uint32_t icsr2            = p_ctrl->p_reg->ICSR2.LONG;
    uint32_t icsr2_clear_mask = 0;

    if (0U != (icisr & R_RI3C0_ICISR_BERF_Msk))
    {
        /* Notify the application of the internal error. */
        callback_args.event = RI3C_EVENT_INTERNAL_ERROR;
        p_ctrl->p_cfg->p_callback(&callback_args);

        icisr_clear_mask |= R_RI3C0_ICISR_BERF_Msk;

        /* Resume RI3C operation. */
        p_ctrl->p_reg->ICCR.BIT.RESUME = 1;
    }

    /* Check if a timeout occurred. */
    if (0U != (icsr2 & R_RI3C0_ICSR2_TMOF_Msk))
    {
        /* Notify that application that the timeout was detected. */
        callback_args.event = RI3C_EVENT_TIMEOUT_DETECTED;
        p_ctrl->p_cfg->p_callback(&callback_args);

        icsr2_clear_mask |= R_RI3C0_ICSR2_TMOF_Msk;
    }

    /* Check if the HDR Exit Pattern was detected. */
    if (0U != (icsr2 & R_RI3C0_ICSR2_HDRXDF_Msk))
    {
        /* Notify that application that the HDR Exit pattern was detected. */
        callback_args.event = RI3C_EVENT_HDR_EXIT_PATTERN_DETECTED;
        p_ctrl->p_cfg->p_callback(&callback_args);

        icsr2_clear_mask |= R_RI3C0_ICSR2_HDRXDF_Msk;
    }

    /* Clear the status flags that have been handled. */
    p_ctrl->p_reg->ICISR.LONG &= ~icisr_clear_mask;
    p_ctrl->p_reg->ICSR2.LONG &= ~icsr2_clear_mask;
}

#if RI3C_CFG_CONTROLLER_SUPPORT

/***********************************************************************************************************************
 * @brief Perform error recovery.
 **********************************************************************************************************************/
void ri3c_controller_error_recovery(ri3c_instance_ctrl_t *p_ctrl, bool error_recovery_case_2)
{
#if !I3C_ECO_VERSION

    /* Flush the Command, Rx and Tx Buffers. */
    p_ctrl->p_reg->ICRCR.LONG = RI3C_ICRCR_FIFO_FLUSH_Msk;

    /* The field will be cleared automatically upon reset completion (See section 35.2.4 in the RX26T manual). */
    /* WAIT_LOOP */
    while (p_ctrl->p_reg->ICRCR.LONG & RI3C_ICRCR_FIFO_FLUSH_Msk != 0U)
    {
        /* Wait. */
    }

    /* Wait for the bus available condition. */
    /* WAIT_LOOP */
    while (1)
    {
        /* If SDA is pulled low, then a target device started an IBI during error recovery. */
        if (0U == p_ctrl->p_reg->ICIMR.BIT.SCLI)
        {
            break;
        }

        /* Check the bus available condition. */
        if (1 == p_ctrl->p_reg->ICBSR.BIT.BAVL)
        {
            break;
        }
    }

    if (error_recovery_case_2)
    {
        /* Disable the IBI Status Buffer Full IRQ. */
        p_ctrl->p_reg->ICCSIER.BIT.IQEFIE = 0;

        /* When recovering from a read operation where the transfer length is less than expected, perform internal
         * software reset. */
        p_ctrl->p_reg->ICRCR.LONG = R_RI3C0_ICRCR_ISRST_Msk;
        p_ctrl->p_reg->ICRCR.LONG = 0;

        /* Restore the current controller setting. */
        p_ctrl->p_reg->ICMMR.LONG = (uint32_t)(R_RI3C0_ICMMR_ACF_Msk | R_RI3C0_ICMMR_WP_Msk);

        /* After an internal reset, the ACF bit is cleared which causes the IBI Queue Empty/Full Flag to be set
         * indicating the queue is empty. Since the driver is in controller mode,
         * this status should be discarded and the flag should be cleared.
         */
        p_ctrl->p_reg->ICCSR.BIT.IQEFF = 0;

        /* Enable the IBI Status Buffer Full IRQ. */
        p_ctrl->p_reg->ICCSIER.BIT.IQEFIE = 1;
    }
    else
    {
        /* Resume RI3C operation. */
        p_ctrl->p_reg->ICCR.BIT.RESUME = 1;
    }

    /* If a target device started an IBI during error recovery,
     * then it must be NACK'd or SDA will be held low indefinitely.
     */
    if (0U == p_ctrl->p_reg->ICIMR.BIT.SCLI)
    {
        /* Calculate the frequency of PCLKA. */
        uint32_t pclka_frequency = BSP_PCLKA_HZ;

        ri3c_extended_cfg_t * p_extend = (ri3c_extended_cfg_t *)p_ctrl->p_cfg->p_extend; // Cast to functional data type

        /* Get the low and high period in PCLKA ticks. */
        uint32_t pclka_low_period = (p_extend->bitrate_settings.icsbr & R_RI3C0_ICSBR_ODLW_Msk)
            >> R_RI3C0_ICSBR_ODLW_Pos;
        uint32_t pclka_high_period = (p_extend->bitrate_settings.icsbr & R_RI3C0_ICSBR_ODHW_Msk)
            >> R_RI3C0_ICSBR_ODHW_Pos;

        /* Calculate the high and low period for SCL. */
        uint32_t high_frequency = pclka_frequency / pclka_high_period;
        uint32_t low_frequency  = pclka_frequency / pclka_low_period;
        uint32_t high_delay_us  = (1000000U + high_frequency - 1) / high_frequency; // NOLINT(readability-magic-numbers)
        uint32_t low_delay_us   = (1000000U + low_frequency - 1) / low_frequency;   // NOLINT(readability-magic-numbers)

        /* Check if ICBCR is working correctly. */
        bool icbcr_zero = true;

        /* WAIT_LOOP */
        for (uint32_t i = 0; i < 4; i++)
        {
            R_BSP_SoftwareDelay(high_delay_us + low_delay_us, BSP_DELAY_MICROSECS);
            if (0 != p_ctrl->p_reg->ICBCR.BIT.BC)
            {
                icbcr_zero = false;
            }
        }

        /* If ICBCR is not incrementing, then the controller is not aware of the IBI. */
        if (icbcr_zero)
        {
            /* Write SCL low in order to complete the start condition. */
            p_ctrl->p_reg->ICOCR.LONG = R_RI3C0_ICOCR_SDAO_Msk | R_RI3C0_ICOCR_SOWP_Msk;

            R_BSP_SoftwareDelay(low_delay_us, BSP_DELAY_MICROSECS);

            /* Complete 9 SCL clock cycles while holding SDA high in order to NACK the IBI. */
            /* WAIT_LOOP */
            for (uint32_t i = 0; i < 9; i++)
            {
                /* Write SCL high. */
                p_ctrl->p_reg->ICOCR.LONG = R_RI3C0_ICOCR_SDAO_Msk | R_RI3C0_ICOCR_SCLO_Msk | R_RI3C0_ICOCR_SOWP_Msk;

                R_BSP_SoftwareDelay(high_delay_us, BSP_DELAY_MICROSECS);

                /* Write SCL low. */
                p_ctrl->p_reg->ICOCR.LONG = R_RI3C0_ICOCR_SDAO_Msk | R_RI3C0_ICOCR_SOWP_Msk;

                R_BSP_SoftwareDelay(low_delay_us, BSP_DELAY_MICROSECS);
            }

            /* Write SCL and SDA low. */
            p_ctrl->p_reg->ICOCR.LONG = R_RI3C0_ICOCR_SOWP_Msk;

            R_BSP_SoftwareDelay(low_delay_us, BSP_DELAY_MICROSECS);

            /* Write SCL high. */
            p_ctrl->p_reg->ICOCR.LONG = R_RI3C0_ICOCR_SCLO_Msk | R_RI3C0_ICOCR_SOWP_Msk;

            R_BSP_SoftwareDelay(high_delay_us, BSP_DELAY_MICROSECS);

            /* Write SDA anb SCL high to complete the stop condition. */
            p_ctrl->p_reg->ICOCR.LONG = R_RI3C0_ICOCR_SDAO_Msk | R_RI3C0_ICOCR_SCLO_Msk | R_RI3C0_ICOCR_SOWP_Msk;
        }
    }
#endif
}

#endif /* RI3C_CFG_CONTROLLER_SUPPORT */

#if RI3C_CFG_TARGET_SUPPORT

/***********************************************************************************************************************
 * @brief Perform error recovery according to Figure 35.67 in the RX26T manual
 **********************************************************************************************************************/
void ri3c_target_error_recovery(ri3c_instance_ctrl_t *p_ctrl, ri3c_target_error_recovery_type_t recovery_type)
{
    switch (recovery_type)
    {
        case RI3C_TARGET_ERROR_RECOVERY_TYPE_WRITE:
        {
            /* Flush the Command and Tx Data FIFO. */
            p_ctrl->p_reg->ICRCR.LONG = R_RI3C0_ICRCR_TBRST_Msk;
            break;
        }

        case RI3C_TARGET_ERROR_RECOVERY_TYPE_READ:
        {
            /* Flush the Command and Rx Data FIFO. */
            p_ctrl->p_reg->ICRCR.LONG = R_RI3C0_ICRCR_RBRST_Msk;
            break;
        }

        case RI3C_TARGET_ERROR_RECOVERY_TYPE_IBI:
        {
            /* Flush the Command and Rx Data FIFO. */
            p_ctrl->p_reg->ICRCR.LONG = R_RI3C0_ICRCR_CQRST_Msk;
            break;
        }

        default:
        {
            break;
        }
    }

    /* The field will be cleared automatically upon reset completion (See section 35.2.4 in the RX26T manual). */
    /* WAIT_LOOP */
    while (p_ctrl->p_reg->ICRCR.BIT.MRST != 0)
    {
        /* Wait. */
    }

    /* Wait for Bus Available Condition (See Figure 35.67 in the RX26T manual). */
    /* WAIT_LOOP */
    while (p_ctrl->p_reg->ICBSR.LONG & R_RI3C0_ICBSR_BAVL_Msk != R_RI3C0_ICBSR_BAVL_Msk)
    {
        /* Wait. */
    }

    /* Wait for start condition to be cleared (See Figure 35.67 in the RX26T manual). */
    /* WAIT_LOOP */
    while (p_ctrl->p_reg->ICSR2.LONG & R_RI3C0_ICSR2_START_Msk != 0U)
    {
        /* Wait. */
    }

    /* Read the current value of DADR. */
    uint32_t icdar0 = p_ctrl->p_reg->ICDAR0.LONG;

    /* Perform internal software reset. */
    p_ctrl->p_reg->ICRCR.LONG = R_RI3C0_ICRCR_ISRST_Msk;
    p_ctrl->p_reg->ICRCR.LONG = 0;

    /* Get the PCLKA frequency. */
    uint32_t pclka_frequency = BSP_PCLKA_HZ;

    /* Wait for the expected amount of time for the Bus Available Condition. */
    uint32_t expected_bus_available_time = 
        (1000000U * p_ctrl->p_reg->ICBATR + pclka_frequency - 1) / pclka_frequency; // NOLINT(readability-magic-numbers)
    R_BSP_SoftwareDelay(expected_bus_available_time, BSP_DELAY_MICROSECS);

    /* If the Bus is already available then error recovery is complete. */
    if (0 == (p_ctrl->p_reg->ICBSR.LONG & R_RI3C0_ICBSR_BAVL_Msk))
    {

        /* WAIT_LOOP */
        while (0 == (p_ctrl->p_reg->ICSR2.LONG & R_RI3C0_ICSR2_START_Msk))
        {
            /* Perform internal software reset. */
            p_ctrl->p_reg->ICRCR.LONG = R_RI3C0_ICRCR_ISRST_Msk;

            /* Wait for Bus Available Condition (See Figure 35.67 in the RX26T manual). */
            /* WAIT_LOOP */
            while (p_ctrl->p_reg->ICBSR.LONG & R_RI3C0_ICBSR_BAVL_Msk != R_RI3C0_ICBSR_BAVL_Msk)
            {
                /* Wait. */
            }

            p_ctrl->p_reg->ICRCR.BIT.MRST = 0;
        }
    }

    /* Write back value of ICDAR0.DADR */
    p_ctrl->p_reg->ICDAR0.LONG = icdar0;
}

#endif /* RI3C_CFG_TARGET_SUPPORT */

/***********************************************************************************************************************
 * @brief Calculate RI3C Bit rate
 **********************************************************************************************************************/
static void ri3c_calculateBitRate(void)
{
    double   t_high;
    double   desired_freq;
    double   t_cr;
    double   t_cf;
    uint16_t register_max;
    uint8_t  dsbrpo;

    uint32_t pclka_frequency = BSP_PCLKA_HZ;
    
    /** Calculate Standard Open Drain Bit Rate */
    t_high       = (double)RI3C_CFG_STANDARD_OPEN_DRAIN_LOGIC_HIGH_PERIOD; // Cast to double to avoid incorrect rounding
    desired_freq = (double)RI3C_CFG_STANDARD_OPEN_DRAIN_FREQUENCY; // Cast to double to avoid incorrect rounding
    t_cr         = (double)RI3C_CFG_OPEN_DRAIN_RISING_TIME; // Cast to double to avoid incorrect rounding
    t_cf         = (double)RI3C_CFG_OPEN_DRAIN_FALLING_TIME; // Cast to double to avoid incorrect rounding
    register_max = 511;
    dsbrpo       = 0;

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    std_od_high_setting = (uint32_t)ri3c_calculateHighSetting(pclka_frequency,
            t_high,
            desired_freq,
            register_max, 
            dsbrpo);

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    std_od_low_setting = (uint32_t)ri3c_calculateLowSetting(pclka_frequency, 
            std_od_high_setting, 
            desired_freq, 
            t_cr, 
            t_cf, 
            register_max, 
            dsbrpo);
    
    if ((std_od_high_setting >= 256) || (std_od_low_setting >= 256))
    {
        register_max        = 255;
        dsbrpo              = 1;

        /* Cast to 32 bit unsigned integer to match 32 bit register. */
        std_od_high_setting = (uint32_t)ri3c_calculateHighSetting(pclka_frequency,
                t_high,
                desired_freq,
                register_max,
                dsbrpo);

        /* Cast to 32 bit unsigned integer to match 32 bit register. */
        std_od_low_setting  = (uint32_t)ri3c_calculateLowSetting(pclka_frequency,
                std_od_high_setting,
                desired_freq,
                t_cr,
                t_cf,
                register_max,
                dsbrpo);
    }

    /** Calculate Standard Push Pull Bit Rate */
    t_high       = (double)RI3C_CFG_STANDARD_PUSH_PULL_LOGIC_HIGH_PERIOD; // Cast to double to avoid incorrect rounding
    desired_freq = (double)RI3C_CFG_STANDARD_PUSH_PULL_FREQUENCY; // Cast to double to avoid incorrect rounding
    t_cr         = (double)RI3C_CFG_PUSH_PULL_RISING_TIME; // Cast to double to avoid incorrect rounding
    t_cf         = (double)RI3C_CFG_PUSH_PULL_FALLING_TIME; // Cast to double to avoid incorrect rounding
    register_max = 63;
    dsbrpo       = 0;

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    std_pp_high_setting = (uint32_t)ri3c_calculateHighSetting(pclka_frequency, 
            t_high, 
            desired_freq, 
            register_max, 
            dsbrpo);

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    std_pp_low_setting  = (uint32_t)ri3c_calculateLowSetting(pclka_frequency,
            std_pp_high_setting,
            desired_freq,
            t_cr,
            t_cf,
            register_max,
            dsbrpo);

    /* Calculate Extended Open Drain Bit Rate */
    t_high       = (double)RI3C_CFG_EXTENDED_OPEN_DRAIN_LOGIC_HIGH_PERIOD; // Cast to double to avoid incorrect rounding
    desired_freq = (double)RI3C_CFG_EXTENDED_OPEN_DRAIN_FREQUENCY; // Cast to double to avoid incorrect rounding
    t_cr         = (double)RI3C_CFG_OPEN_DRAIN_RISING_TIME; // Cast to double to avoid incorrect rounding
    t_cf         = (double)RI3C_CFG_OPEN_DRAIN_FALLING_TIME; // Cast to double to avoid incorrect rounding
    register_max = 511;
    dsbrpo       = 0;

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    ext_od_high_setting = (uint32_t)ri3c_calculateHighSetting(pclka_frequency,
            t_high,
            desired_freq,
            register_max,
            dsbrpo);

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    ext_od_low_setting = (uint32_t)ri3c_calculateLowSetting(pclka_frequency, 
            ext_od_high_setting,
            desired_freq,
            t_cr,
            t_cf,
            register_max,
            dsbrpo);

    /** Calculate Extended Push Pull Bit Rate */
    t_high       = (double)RI3C_CFG_EXTENDED_PUSH_PULL_LOGIC_HIGH_PERIOD; // Cast to double to avoid incorrect rounding
    desired_freq = (double)RI3C_CFG_EXTENDED_PUSH_PULL_FREQUENCY; // Cast to double to avoid incorrect rounding
    t_cr         = (double)RI3C_CFG_PUSH_PULL_RISING_TIME; // Cast to double to avoid incorrect rounding
    t_cf         = (double)RI3C_CFG_PUSH_PULL_FALLING_TIME; // Cast to double to avoid incorrect rounding
    register_max = 511;
    dsbrpo       = 0;

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    ext_od_high_setting = (uint32_t)ri3c_calculateHighSetting(pclka_frequency,
            t_high,
            desired_freq,
            register_max,
            dsbrpo);

    /* Cast to 32 bit unsigned integer to match 32 bit register. */
    ext_od_low_setting = (uint32_t)ri3c_calculateLowSetting(pclka_frequency,
            ext_od_high_setting,
            desired_freq, t_cr,
            t_cf,
            register_max,
            dsbrpo);
}
/**********************************************************************************************************************
 * End of function ri3c_calculateBitRate
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * @brief Calculate High Setting
 **********************************************************************************************************************/
static inline double ri3c_calculateHighSetting(double pclka_frequency, 
    double t_high,
    double desired_freq,
    uint16_t register_max,
    uint8_t dsbrpo)
{
    uint8_t divider;
    double  high_setting;

    divider = dsbrpo + 1;

    /* Utilize the MIN, MAX API functions of BSP Module */
    high_setting = R_BSP_MAX(1, R_BSP_MIN(register_max, (pclka_frequency * t_high / (1000000000 * divider))));

    return high_setting;
}
/**********************************************************************************************************************
 * End of function ri3c_calculateHighSetting
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * @brief Calculate Low Setting
 **********************************************************************************************************************/
static inline double ri3c_calculateLowSetting(double pclka_frequency,
    double high_setting,
    double desired_freq,
    double t_cr,
    double t_cf,
    uint16_t register_max,
    uint8_t dsbrpo)
{
    double  actual_t_high;
    double  t_low;
    double  low_setting;
    uint8_t divider;

    divider       = dsbrpo + 1;
    actual_t_high = high_setting * divider / pclka_frequency * 1000000000;
    t_low         = ((1000000000 / desired_freq) - (t_cr + t_cf + actual_t_high));

    /* Utilize the MIN, MAX API functions of BSP Module */
    low_setting = R_BSP_MAX(1, R_BSP_MIN(register_max, 
        ceil_function((long)(t_low * pclka_frequency / 1000000000), divider)));

    return low_setting;
}
/**********************************************************************************************************************
 * End of function ri3c_calculateLowSetting
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * @brief Ceil function
 * @retval Ceil value of the division result.
 **********************************************************************************************************************/
static long ceil_function(long a, long b)
{
    long ceil_value = (a / b) + ((a % b) != 0);

    return ceil_value;
}
/**********************************************************************************************************************
 * End of function ceil_function
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: g_ri3c0_callback
 **********************************************************************************************************************
 * @brief Callback function for ISR.
 * @param[in] p_args Pointer to the callback arguments.
 * @details Callback function for ISR.
 * @note None.
 *********************************************************************************************************************/
void g_ri3c0_callback(ri3c_callback_args_t const *const p_args)
{
    /* User code here */
}
/**********************************************************************************************************************
 * End of function g_ri3c0_callback
 *********************************************************************************************************************/
