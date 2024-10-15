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
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_canfd_rx.c
* Description  : CANFD FIT driver.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 22.11.2021 1.00    Initial Release
*         : 06.01.2023 1.20    Fixed TXRF flag not cleared in the function canfd_channel_tx_isr()
*         : 13.12.2023 1.31    Added WAIT_LOOP comments.
*         : 28.06.2024 1.40    Added support for RX261.
*                              Fixed to comply with GSCE Coding Standards Rev.6.5.0.
***********************************************************************************************************************/
/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/* Interface file for this package. */
#include "r_canfd_rx_if.h"
#include <string.h>
#include "platform.h"
#include "r_canfd_rx_config.h"
/* Private header file for this package. */
#include "r_canfd_rx_private.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define CANFD_OPEN                        (0x52434644U) /* "RCFD" in ASCII */

#define CANFD_BAUD_RATE_PRESCALER_MIN     (1U)
#define CANFD_BAUD_RATE_PRESCALER_MAX     (1024U)

#define CANFD_PRV_CTR_MODE_MASK           (R_CANFD_GCR_SLPRQ_Msk + R_CANFD_GCR_GMDC_Msk)
#define CANFD_PRV_CTR_RESET_BIT           (1U)
#define CANFD_PRV_RXMB_MAX                (32U)
#define CANFD_PRV_TXMB_CHANNEL_OFFSET     (64U)

#define CANFD_PRV_RX_FIFO_MAX            (2U)
#define CANFD_PRV_RX_FIFO_STATUS_MASK    (R_CANFD_FESR_RFEMP_Msk)

/***********************************************************************************************************************
 * Const data
 **********************************************************************************************************************/

/* LUT to convert DLC values to payload size in bytes */
static const uint8_t dlc_to_bytes[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};

#if CANFD_CFG_PARAM_CHECKING_ENABLE

/* LUT to determine the hierarchy of can_operation_mode_t modes. */
static const uint8_t g_mode_order[] = {0, 2, 1, 0, 0, 3};
#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
#if CANFD_CFG_PARAM_CHECKING_ENABLE
static bool r_canfd_bit_timing_parameter_check (can_bit_timing_cfg_t * p_bit_timing);

#endif

static void     r_canfd_mb_read (uint32_t buffer, can_frame_t * const frame);
static void     r_canfd_call_callback (canfd_instance_ctrl_t * p_ctrl, can_callback_args_t * p_args);
static void     r_canfd_mode_transition (canfd_instance_ctrl_t * p_ctrl, can_operation_mode_t operation_mode);
static void     r_canfd_mode_ctr_set (volatile uint32_t * p_ctr_reg, can_operation_mode_t operation_mode);
static uint8_t  r_canfd_bytes_to_dlc (uint8_t bytes);
static uint32_t trailing_zeros (uint32_t n);

#if defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T)
/* Interrupt service routine for CANFD global and channel error group interrupt */
static void      canfd_error_grp_isr(void);

/* Interrupt service routine for CANFD RX FIFO group interrupt */
static void      canfd_rx_fifo_grp_isr(void);

/* Interrupt service routine for CANFD0 Channel TX group interrupt */
static void      canfd_channel_tx_grp_isr(void);
#endif

/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

static const canfd_ch_st_ptr CANFD_CHANNELS[] =
{
    #ifdef CANFD0
        &CANFD0,
    #endif
};

/* make sure that MAX_CHANNELS = the number of CANFD channels */
#define MAX_CHANNELS (sizeof(CANFD_CHANNELS)/sizeof(canfd_ch_st_ptr))

/* Channel control struct array */
static canfd_instance_ctrl_t * gp_ctrl[MAX_CHANNELS] = {NULL};

/* CANFD function pointers   */
const can_api_t g_canfd_on_canfd =
{
    .open           = R_CANFD_Open,
    .close          = R_CANFD_Close,
    .write          = R_CANFD_Write,
    .read           = R_CANFD_Read,
    .modeTransition = R_CANFD_ModeTransition,
    .infoGet        = R_CANFD_InfoGet,
    .callbackSet    = R_CANFD_CallbackSet,
};

/*******************************************************************************************************************//**
 * @addtogroup CANFD
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/***************************************************************************************************************//**
 * @brief Open and configure the CANFD channel for operation.
 * @param[in] p_api_ctrl - Pointer to the CAN control block. Must be declared by user.
 * @param[in] p_cfg - Pointer to CAN configuration structure. All elements of this structure must be set by user.
 * @retval FSP_SUCCESS                            Channel opened successfully.
 * @retval FSP_ERR_ALREADY_OPEN                   Driver already open.
 * @retval FSP_ERR_IN_USE                         Channel is already in use.
 * @retval FSP_ERR_IP_CHANNEL_NOT_PRESENT         Channel does not exist on this MCU.
 * @retval FSP_ERR_ASSERTION                      A required pointer was NULL.
 * @retval FSP_ERR_CAN_INIT_FAILED                The provided nominal or data bitrate is invalid.
 * @retval FSP_ERR_CLOCK_INACTIVE                 CANFD source clock is disabled (PLL or PLL2).
 * @details Open and configure the CANFD channel for operation.
 * @note None.
 *****************************************************************************************************************/
fsp_err_t R_CANFD_Open (can_ctrl_t * const p_api_ctrl, can_cfg_t const * const p_cfg)
{
    volatile struct st_canfd0 R_BSP_EVENACCESS_SFR * canfd_ch_block_p;
    volatile struct st_canfd R_BSP_EVENACCESS_SFR * canfd_block_p;

    /* Assign variable p_ctrl */
    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;

#if ((R_BSP_VERSION_MAJOR == 7) && (R_BSP_VERSION_MINOR >= 20)) || (R_BSP_VERSION_MAJOR >= 8)
bsp_int_ctrl_t int_ctrl;
#endif
#if CANFD_CFG_PARAM_CHECKING_ENABLE

    /* check argument p_ctrl */
    FSP_ASSERT(p_ctrl);

    /* check argument p_cfg */
    FSP_ASSERT(p_cfg);

    /* check argument p_cfg->p_extend */
    FSP_ASSERT(p_cfg->p_extend);

    /* check argument p_cfg->p_callback */
    FSP_ASSERT(p_cfg->p_callback);

    /* check argument p_cfg->p_bit_timing */
    FSP_ASSERT(p_cfg->p_bit_timing);

    uint32_t channel = p_cfg->channel;

    /* Check that the module is not open */
    FSP_ERROR_RETURN(CANFD_OPEN != p_ctrl->open, FSP_ERR_ALREADY_OPEN);

    /* Check  the channel is present and that it is not in use */
    FSP_ERROR_RETURN(channel < MAX_CHANNELS, FSP_ERR_IP_CHANNEL_NOT_PRESENT);

    /* Check  gp_ctrl */
    FSP_ERROR_RETURN(NULL == gp_ctrl[channel], FSP_ERR_IN_USE);

    /* Assign variable p_extend */
    canfd_extended_cfg_t * p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;

    /* Check that the global config is present */
    FSP_ASSERT(p_extend->p_global_cfg);

    /* Check nominal bit timing parameters for correctness */
    FSP_ERROR_RETURN(r_canfd_bit_timing_parameter_check(p_cfg->p_bit_timing), FSP_ERR_CAN_INIT_FAILED);

    /* Assign variable p_data_timing */
    can_bit_timing_cfg_t * p_data_timing = p_extend->p_data_timing;

    /* Check that bit timing for FD bitrate switching is present and correct */
    FSP_ASSERT(p_data_timing);

    /* Check that prescaler is in range */
    FSP_ERROR_RETURN((p_data_timing->baud_rate_prescaler <= CANFD_BAUD_RATE_PRESCALER_MAX) &&
                    (p_data_timing->baud_rate_prescaler >= CANFD_BAUD_RATE_PRESCALER_MIN),
                    FSP_ERR_CAN_INIT_FAILED);

    /* Check that TSEG1 >= TSEG2 >= SJW for data bitrate  */

    /* Check Time Segment 1 is greater than or equal to Time Segment 2 */
    FSP_ERROR_RETURN((uint32_t) p_data_timing->time_segment_1 >= (uint32_t) p_data_timing->time_segment_2, FSP_ERR_CAN_INIT_FAILED);

    /* Check Time Segment 2 is greater than or equal to the synchronization jump width */
    FSP_ERROR_RETURN((uint32_t) p_data_timing->time_segment_2 >= (uint32_t) p_data_timing->synchronization_jump_width,
            FSP_ERR_CAN_INIT_FAILED);

    can_bit_timing_cfg_t * p_bit_timing = p_cfg->p_bit_timing;

    /* Check that data rate > nominal rate */
    uint32_t data_rate_clocks = p_data_timing->baud_rate_prescaler *
                                (p_data_timing->time_segment_1 + p_data_timing->time_segment_2 + 1U);
    uint32_t nominal_rate_clocks = p_bit_timing->baud_rate_prescaler *
                                    (p_bit_timing->time_segment_1 + p_bit_timing->time_segment_2 + 1U);

    /* Check that data_rate_clocks <= nominal_rate_clocks  */
    FSP_ERROR_RETURN(data_rate_clocks <= nominal_rate_clocks, FSP_ERR_CAN_INIT_FAILED);

#else  /* !CANFD_CFG_PARAM_CHECKING_ENABLE */
    uint32_t channel = p_cfg->channel;

    /* Get extended config */
    canfd_extended_cfg_t * p_extend = (canfd_extended_cfg_t *) p_cfg->p_extend;
#endif /* CANFD_CFG_PARAM_CHECKING_ENABLE */

    fsp_err_t err = FSP_SUCCESS;

    /* Assign variable canfd_ch_block_p to &CANFDn with n is the channel */
    canfd_ch_block_p = CANFD_CHANNELS[channel];

    /* Assign variable canfd_block_p to &CANFD */
    canfd_block_p = &CANFD;

    /* Initialize the control block */
    p_ctrl->p_cfg = p_cfg;

    /* Set callback and context pointers, if configured */
    p_ctrl->p_callback        = p_cfg->p_callback;
    p_ctrl->p_context         = p_cfg->p_context;
    p_ctrl->p_callback_memory = NULL;

    /* Get global config */
    canfd_global_cfg_t * p_global_cfg = p_extend->p_global_cfg;

    /* Start module */
    /* CANFD: Module Stop Control Register D (MSTPCRD) bits 10 are for the CANFD
    Peripherals.
    0: The module stop state is canceled.
    1: Transition to the module stop state is made. */
    /* First unlock the protect register. */
    /* Enable writing to PRCR bits while simultaneously enabling PRC1. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
#if ((R_BSP_VERSION_MAJOR == 7) && (R_BSP_VERSION_MINOR >= 20)) || (R_BSP_VERSION_MAJOR >= 8)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
    if (0 == p_cfg->channel)
    {
        /* Write MSTPCRD to 0: The module stop state is canceled. */
#if defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T)
        MSTP(CANFD) = 0;
#else
        MSTP(CANFD0) = 0;
#endif
    }
#if ((R_BSP_VERSION_MAJOR == 7) && (R_BSP_VERSION_MINOR >= 20)) || (R_BSP_VERSION_MAJOR >= 8)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    /* Re-lock the protect register. */
    /* Enable writing to PRCR bits while simultaneously disabling PRC1. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Perform global config only if the module is in Global Sleep or Global Reset */
    if (canfd_block_p->GSR.LONG & R_CANFD_GSR_RSTST_Msk)
    {
        /* Wait for RAM initialization (see RX6T2 User's Manual (R01UH0937EJ0050) section 33.3.3.1 Note 2) */
        /* WAIT_LOOP */
        FSP_HARDWARE_REGISTER_WAIT((canfd_block_p->GSR.LONG & R_CANFD_GSR_RAMST_Msk), 0);

        /* Cancel Global Sleep and wait for transition to Global Reset */
        r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_GLOBAL_RESET);

        /* Configure global TX priority, DLC check/replace functions, external/internal clock select and payload
         * overflow behavior */
        canfd_block_p->GCFG.LONG = p_global_cfg->global_config;

        /* Configure rule count for both channels */
        canfd_block_p->AFCFG.LONG = (CANFD_CFG_AFL_CH0_RULE_NUM << R_CANFD_AFCFG_RNC0_Pos);

        /* Set CAN FD Protocol Exception response (ISO exception state or send error frame) */
        canfd_block_p->GFDCFG.LONG = CANFD_CFG_FD_PROTOCOL_EXCEPTION;

        /* Set number and size of RX message buffers */
        canfd_block_p->RMCR.LONG = p_global_cfg->rx_mb_config;

        /* Configure RX FIFOs and interrupt */
        /* WAIT_LOOP */
        for (uint32_t i = 0; i < CANFD_PRV_RX_FIFO_MAX; i++)
        {
            canfd_block_p->RFCR[i].LONG = p_global_cfg->rx_fifo_config[i];
        }

#if defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T)
        /* Configure Configure RX FIFOs interrupt. Must enable group that it belongs to */
        /* in addition to individual source. */
        R_BSP_InterruptRequestEnable(VECT(ICU, GROUPBL2));
#if ((R_BSP_VERSION_MAJOR == 7) && (R_BSP_VERSION_MINOR >= 20)) || (R_BSP_VERSION_MAJOR >= 8)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
        /* resolves to:  ICU.GEN[GEN_CANFD_RFRI].BIT.EN4 = 1; */
        EN(CANFD, RFRI) = 1;
#if ((R_BSP_VERSION_MAJOR == 7) && (R_BSP_VERSION_MINOR >= 20)) || (R_BSP_VERSION_MAJOR >= 8)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
        /* Configure priority interrupt */
        ICU.IPR[IPR_ICU_GROUPBL2].BIT.IPR = p_global_cfg->rx_fifo_ipl;
        R_BSP_InterruptWrite( BSP_INT_SRC_BL2_CANFD_RFRI, (bsp_int_cb_t) canfd_rx_fifo_grp_isr);
#endif

#if !(defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T))
        /* Enable CANFD RX FIFOs interrupt. */
        R_BSP_InterruptRequestEnable(VECT(CANFD, RFRI));

        /* Configure priority interrupt */
        IPR(CANFD, RFRI) = p_global_cfg->global_interrupts;
#endif
        /* Set global error interrupts */
        canfd_block_p->GCR.LONG = p_global_cfg->global_interrupts;
    }

#if defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T)
    /* Configure global error interrupt. Must enable group that it belongs to */
    /* in addition to individual source. */
    R_BSP_InterruptRequestEnable(VECT(ICU, GROUPBL2));
#if ((R_BSP_VERSION_MAJOR == 7) && (R_BSP_VERSION_MINOR >= 20)) || (R_BSP_VERSION_MAJOR >= 8)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
    /* resolves to:  ICU.GEN[GEN_CANFD_GLEI].BIT.EN3 = 1; */
    EN(CANFD, GLEI) = 1;
#if ((R_BSP_VERSION_MAJOR == 7) && (R_BSP_VERSION_MINOR >= 20)) || (R_BSP_VERSION_MAJOR >= 8)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    /* Configure priority interrupt */
    ICU.IPR[IPR_ICU_GROUPBL2].BIT.IPR = p_global_cfg->global_err_ipl;
    R_BSP_InterruptWrite( BSP_INT_SRC_BL2_CANFD_GLEI, (bsp_int_cb_t) canfd_error_grp_isr);
#endif

#if !(defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T))
    /* Enable CANFD Global Error Interrupt. */
    R_BSP_InterruptRequestEnable(VECT(CANFD, GLEI));

    /* Configure priority interrupt */
    IPR(CANFD, GLEI) = p_global_cfg->global_err_ipl;
#endif

    /* Track ctrl struct */
    gp_ctrl[channel] = p_ctrl;

    /* Get AFL entry and limit */
    uint32_t afl_entry = 0;
    uint32_t afl_max   = CANFD_CFG_AFL_CH0_RULE_NUM;

    /* Unlock AFL */
    canfd_block_p->AFCR.LONG = R_CANFD_AFCR_AFLWE_Msk;

    /* Write all configured AFL entries */
    canfd_afl_t * p_afl = (canfd_afl_t*)p_extend->p_afl;

    /* WAIT_LOOP */
    for (; afl_entry < afl_max; afl_entry++)
    {
        /* AFL register access is performed through a page window comprised of 16 entries. See Section 33.5.7 "Entering
         * Entries in the AFL" in the RX660 User's Manual (R01UH0937EJ0050) for more details. */

        /* Set AFL page */
        canfd_block_p->AFCR.LONG = (afl_entry >> 4) | R_CANFD_AFCR_AFLWE_Msk;

        /* Get pointer to current AFL rule and set it to the rule pointed to by p_afl */
        volatile canfd_afl_t * p_cfdgafl = (canfd_afl_t *)&canfd_block_p->AFL[afl_entry & 0xF];
        *p_cfdgafl = *p_afl++;

        /* Set Information Label 0 to the channel being configured */
        p_cfdgafl->PTR0.BIT.IFL0 = channel & 1U;
    }

    /* Lock AFL */
    canfd_block_p->AFCR.LONG = 0;

    /* Cancel Channel Sleep and wait for transition to Channel Reset */
    r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_RESET);

    /* Configure bit rate */
    canfd_ch_block_p->NBCR.LONG = (uint32_t) (((p_cfg->p_bit_timing->baud_rate_prescaler - 1) & R_CANFD_NBCR_BRP_Msk) <<
        R_CANFD_NBCR_BRP_Pos) |
        ((p_cfg->p_bit_timing->time_segment_1 - 1U) << R_CANFD_NBCR_TSEG1_Pos) |
        ((p_cfg->p_bit_timing->time_segment_2 - 1U) << R_CANFD_NBCR_TSEG2_Pos) |
        ((p_cfg->p_bit_timing->synchronization_jump_width - 1U) << R_CANFD_NBCR_SJW_Pos);

    /* Configure data bit rate for rate switching on FD frames */
    canfd_ch_block_p->DBCR.LONG = (uint32_t) (((p_extend->p_data_timing->baud_rate_prescaler - 1) & R_CANFD_DBCR_BRP_Msk) <<
        R_CANFD_DBCR_BRP_Pos) |
        ((p_extend->p_data_timing->time_segment_1 - 1U) << R_CANFD_DBCR_TSEG1_Pos) |
        ((p_extend->p_data_timing->time_segment_2 - 1U) << R_CANFD_DBCR_TSEG2_Pos) |
        ((p_extend->p_data_timing->synchronization_jump_width - 1U) << R_CANFD_DBCR_SJW_Pos);

    /* Ensure transceiver delay offset is not larger than 8 bits */
    uint32_t tdco = p_extend->p_data_timing->time_segment_1;
    if (tdco > UINT8_MAX)
    {
        tdco = UINT8_MAX;
    }

    /* Configure transceiver delay compensation; allow user to set ESI bit manually */
    canfd_ch_block_p->FDCFG.LONG = (tdco << R_CANFD_FDCFG_TDCO_Pos) | (uint32_t) (p_extend->delay_compensation << R_CANFD_FDCFG_TDCE_Pos) |
        R_CANFD_FDCFG_TESI_Msk | 1U;

    /* Write TX message buffer interrupt enable bits */
    canfd_block_p->TMIER0.LONG = p_extend->txmb_txi_enable & 0xF;

    /* Configure channel error interrupts */
    canfd_ch_block_p->CHCR.LONG = p_extend->error_interrupts | R_CANFD_CHCR_MDC_Msk;

    /* Enable channel interrupts */

#if defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T)
    /* Configure channel error interrupts. Must enable group that it belongs to */
    /* in addition to individual source. */
    R_BSP_InterruptRequestEnable(VECT(ICU, GROUPBL2));
#if ((R_BSP_VERSION_MAJOR == 7) && (R_BSP_VERSION_MINOR >= 20)) || (R_BSP_VERSION_MAJOR >= 8)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
    /* resolves to:  ICU.GEN[GEN_CANFD0_CHEI].BIT.EN1 = 1; */
    EN(CANFD0, CHEI) = 1;
#if ((R_BSP_VERSION_MAJOR == 7) && (R_BSP_VERSION_MINOR >= 20)) || (R_BSP_VERSION_MAJOR >= 8)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    /* Configure priority interrupt */
    ICU.IPR[IPR_ICU_GROUPBL2].BIT.IPR = p_cfg->ipl;
    R_BSP_InterruptWrite( BSP_INT_SRC_BL2_CANFD0_CHEI, (bsp_int_cb_t) canfd_error_grp_isr);

    /* Configure channel interrupts. Must enable group that it belongs to */
    /* in addition to individual source. */
    R_BSP_InterruptRequestEnable(VECT(ICU, GROUPBL2));
#if ((R_BSP_VERSION_MAJOR == 7) && (R_BSP_VERSION_MINOR >= 20)) || (R_BSP_VERSION_MAJOR >= 8)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
    /* resolves to:  ICU.GEN[GEN_CANFD0_CHTI].BIT.EN5 = 1; */
    EN(CANFD0, CHTI) = 1;
#if ((R_BSP_VERSION_MAJOR == 7) && (R_BSP_VERSION_MINOR >= 20)) || (R_BSP_VERSION_MAJOR >= 8)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    /* Configure priority interrupt */
    ICU.IPR[IPR_ICU_GROUPBL2].BIT.IPR = p_cfg->ipl;
    R_BSP_InterruptWrite( BSP_INT_SRC_BL2_CANFD0_CHTI, (bsp_int_cb_t) canfd_channel_tx_grp_isr);
#endif

#if !(defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T))
    /* Enable CANFD0 Channel Error Interrupt. */
    R_BSP_InterruptRequestEnable(VECT(CANFD0, CHEI));

    /* Configure priority interrupt */
    IPR(CANFD0, CHEI) = p_cfg->ipl;

    /* Enable CANFD Global Error Interrupt. */
    R_BSP_InterruptRequestEnable(VECT(CANFD0, CHTI));

    /* Configure priority interrupt */
    IPR(CANFD0, CHTI) = p_cfg->ipl;
#endif

    /* Set global mode to Operation and wait for transition */
    r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_GLOBAL_OPERATION);

    /* Transition to Channel Operation */
    r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_NORMAL);

    /* Set current operation modes */
    p_ctrl->operation_mode = CAN_OPERATION_MODE_NORMAL;
    p_ctrl->test_mode      = CAN_TEST_MODE_DISABLED;

    /* Set driver to open */
    p_ctrl->open = CANFD_OPEN;

    return err;
}
/******************************************************************************
 End of function R_CANFD_Open
 *****************************************************************************/

/***************************************************************************************************************//**
 * @brief Close the CANFD channel.
 * @param[in] p_api_ctrl - Pointer to the CAN control block.
 * @retval FSP_SUCCESS               Channel closed successfully.
 * @retval FSP_ERR_NOT_OPEN          Control block not open.
 * @retval FSP_ERR_ASSERTION         Null pointer presented.
 * @details Close the CANFD channel.
 * @note None.
 *****************************************************************************************************************/
fsp_err_t R_CANFD_Close (can_ctrl_t * const p_api_ctrl)
{
    /* Assigned CAN Instance Control Block */
    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;

#if CANFD_CFG_PARAM_CHECKING_ENABLE

    /* Check p_ctrl */
    FSP_ASSERT(NULL != p_ctrl);

    /* Check that the module is not open */
    FSP_ERROR_RETURN(p_ctrl->open == CANFD_OPEN, FSP_ERR_NOT_OPEN);
#endif

    /* Set driver to closed */
    p_ctrl->open = 0U;

    /* Get configuration structure */
    can_cfg_t * p_cfg = (can_cfg_t *) p_ctrl->p_cfg;

#if defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T)
    /* Disable interrupts. Must disable group that it belongs to */
    R_BSP_InterruptRequestDisable(VECT(ICU, GROUPBL2));
#else
    /* Disable CANFD Global Error Interrupt. */
    R_BSP_InterruptRequestDisable(VECT(CANFD, GLEI));

    /* Disable CANFD RX FIFO Interrupt. */
    R_BSP_InterruptRequestDisable(VECT(CANFD, RFRI));

    /* Disable CANFD0 Channel Error Interrupt. */
    R_BSP_InterruptRequestDisable(VECT(CANFD0, CHEI));

    /* Disable CANFD0 Channel TX Interrupt. */
    R_BSP_InterruptRequestDisable(VECT(CANFD0, CHTI));
#endif

    /* Transition to Global Sleep */
    r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_GLOBAL_RESET);
    r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_GLOBAL_SLEEP);

    /* Stop CANFD module */
    /* CANFD: Module Stop Control Register D (MSTPCRD) bits 10 are for the CANFD
    Peripherals.
    0: The module stop state is canceled.
    1: Transition to the module stop state is made. */
#if ((R_BSP_VERSION_MAJOR == 7) && (R_BSP_VERSION_MINOR >= 20)) || (R_BSP_VERSION_MAJOR >= 8)
bsp_int_ctrl_t int_ctrl;
#endif
    /* First unlock the protect register. */
    /* Enable writing to PRCR bits while simultaneously enabling PRC1. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
#if ((R_BSP_VERSION_MAJOR == 7) && (R_BSP_VERSION_MINOR >= 20)) || (R_BSP_VERSION_MAJOR >= 8)
R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
    if (0 == p_cfg->channel)
    {
        /* Write MSTPCRD to 1: The module stop state is canceled. */
#if defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T)
        MSTP(CANFD) = 1;
#else
        MSTP(CANFD0) = 1;
#endif
    }
#if ((R_BSP_VERSION_MAJOR == 7) && (R_BSP_VERSION_MINOR >= 20)) || (R_BSP_VERSION_MAJOR >= 8)
R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    /* Re-lock the protect register. */
    /* Enable writing to PRCR bits while simultaneously disabling PRC1. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Reset global control structure pointer */
    gp_ctrl[p_cfg->channel] = NULL;

    return FSP_SUCCESS;
}
/******************************************************************************
 End of function R_CANFD_Close
 *****************************************************************************/

/***************************************************************************************************************//**
 * @brief Write data to the CANFD channel.
 * @param[in] p_api_ctrl - Pointer to the CAN control block.
 * @param[in] buffer - Buffer number (mailbox or message buffer) to write to.
 * @param[in] p_frame - Pointer for frame of CAN ID, DLC, data and frame type to write.
 * @retval FSP_SUCCESS                      Operation succeeded.
 * @retval FSP_ERR_NOT_OPEN                 Control block not open.
 * @retval FSP_ERR_CAN_TRANSMIT_NOT_READY   Transmit in progress, cannot write data at this time.
 * @retval FSP_ERR_INVALID_ARGUMENT         Data length or buffer number invalid.
 * @retval FSP_ERR_INVALID_MODE             An FD option was set on a non-FD frame.
 * @retval FSP_ERR_ASSERTION                Null pointer presented
 * @details Write data to the CANFD channel.
 * @note None.
 *****************************************************************************************************************/
fsp_err_t R_CANFD_Write (can_ctrl_t * const p_api_ctrl, uint32_t buffer, can_frame_t * const p_frame)
{
    volatile struct st_canfd R_BSP_EVENACCESS_SFR * canfd_block_p;

    /* Assign variable canfd_block_p to &CANFD */
    canfd_block_p = &CANFD;

#if CANFD_CFG_PARAM_CHECKING_ENABLE
    /* Assigned CAN Instance Control Block */
    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;

    /* Check p_ctrl */
    FSP_ASSERT(NULL != p_ctrl);

    /* Check p_frame */
    FSP_ASSERT(NULL != p_frame);

    /* Check p_ctrl->open */
    FSP_ERROR_RETURN(p_ctrl->open == CANFD_OPEN, FSP_ERR_NOT_OPEN);

    /* CANFD Lite has 4 TX message buffers */
    FSP_ERROR_RETURN(buffer <= 3U, FSP_ERR_INVALID_ARGUMENT);

    /* Check DLC field */
    if (!(p_frame->options & CANFD_FRAME_OPTION_FD))
    {
        /* Check p_frame->data_length_code <= 8 */
        FSP_ERROR_RETURN(p_frame->data_length_code <= 8, FSP_ERR_INVALID_ARGUMENT);

        /* Check p_frame->options == 0 */
        FSP_ERROR_RETURN(p_frame->options == 0, FSP_ERR_INVALID_MODE);
    }
    else if (p_frame->data_length_code > 0)
    {
        /* Make sure the supplied data size corresponds to a valid DLC value */
        FSP_ERROR_RETURN(0U != r_canfd_bytes_to_dlc(p_frame->data_length_code), FSP_ERR_INVALID_ARGUMENT);
    }
    else
    {
        /* Do nothing. */
    }

#else /* !CANFD_CFG_PARAM_CHECKING_ENABLE */
    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;
#endif /* CANFD_CFG_PARAM_CHECKING_ENABLE */

    /* Calculate global TX message buffer number */
    uint32_t txmb = buffer + (p_ctrl->p_cfg->channel * CANFD_PRV_TXMB_CHANNEL_OFFSET);

    /* Ensure MB is ready */
    FSP_ERROR_RETURN(0U == canfd_block_p->TMSR[txmb].BIT.TXSF, FSP_ERR_CAN_TRANSMIT_NOT_READY);

    /* Set TX message buffer registers: HF0 */
    canfd_block_p->TMB[txmb].HF0.LONG = p_frame->id | ((uint32_t) p_frame->type << R_CANFD_TMB_HF0_RTR_Pos) |
                                        ((uint32_t) p_frame->id_mode << R_CANFD_TMB_HF0_IDE_Pos);

    /* Set TX message buffer registers: HF1 */
    canfd_block_p->TMB[txmb].HF1.LONG = (uint32_t) r_canfd_bytes_to_dlc(p_frame->data_length_code) <<
                                        R_CANFD_TMB_HF1_DLC_Pos;

    /* Set FD bits (ESI, BRS and FDF) */
    canfd_block_p->TMB[txmb].HF2.LONG = p_frame->options & 7U;

    /* Copy data_length_code to register buffer */
    uint32_t  len    = p_frame->data_length_code;

    /* Copy data to register buffer */
    uint8_t * p_dest = (uint8_t *) canfd_block_p->TMB[txmb].DF;
    uint8_t * p_src  = p_frame->data;

    /* WAIT_LOOP */
    while (len--)
    {
        *p_dest++ = *p_src++;
    }

    /* Request transmission */
    canfd_block_p->TMCR[txmb].BYTE = 1;

    return FSP_SUCCESS;
}
/******************************************************************************
 End of function R_CANFD_Write
 *****************************************************************************/

/***************************************************************************************************************//**
 * @brief Read data from a CANFD Message Buffer or FIFO.
 * @param[in] p_api_ctrl - Pointer to the CAN control block.
 * @param[in] buffer - Message buffer (number) to read from.
 * @param[in] p_frame - Pointer to store the CAN ID, DLC, data and frame type.
 * @retval FSP_SUCCESS                      Operation succeeded.
 * @retval FSP_ERR_NOT_OPEN                 Control block not open.
 * @retval FSP_ERR_INVALID_ARGUMENT         Buffer number invalid.
 * @retval FSP_ERR_ASSERTION                p_api_ctrl or p_frame is NULL.
 * @retval FSP_ERR_BUFFER_EMPTY             Buffer or FIFO is empty.
 * @details Read data from a CANFD Message Buffer or FIFO.
 * @note None.
 *****************************************************************************************************************/
fsp_err_t R_CANFD_Read (can_ctrl_t * const p_api_ctrl, uint32_t buffer, can_frame_t * const p_frame)
{
    volatile struct st_canfd R_BSP_EVENACCESS_SFR * canfd_block_p;

    /* Assign variable canfd_block_p to &CANFD */
    canfd_block_p = &CANFD;

#if CANFD_CFG_PARAM_CHECKING_ENABLE
    /* Assigned CAN Instance Control Block */
    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;

    /* Check p_ctrl */
    FSP_ASSERT(NULL != p_ctrl);

    /* Check p_frame */
    FSP_ASSERT(NULL != p_frame);

    /* Check p_ctrl->open */
    FSP_ERROR_RETURN(p_ctrl->open == CANFD_OPEN, FSP_ERR_NOT_OPEN);

    /* Check buffer */
    FSP_ERROR_RETURN(buffer < CANFD_PRV_RXMB_MAX + CANFD_PRV_RX_FIFO_MAX, FSP_ERR_INVALID_ARGUMENT);
#else
    /* Check p_api_ctrl */
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
#endif

    uint32_t not_empty;

    /* Return an error if the buffer or FIFO is empty */
    if (buffer < CANFD_PRV_RXMB_MAX)
    {
        not_empty = canfd_block_p->RMNDR.LONG & (1U << buffer);
    }
    else
    {
        not_empty = !(canfd_block_p->FESR.LONG & (1U << (buffer - CANFD_PRV_RXMB_MAX)));
    }

    /* Check not_empty */
    FSP_ERROR_RETURN(not_empty, FSP_ERR_BUFFER_EMPTY);

    /* Retrieve message from buffer */
    r_canfd_mb_read(buffer, p_frame);

    return FSP_SUCCESS;
}
/******************************************************************************
 End of function R_CANFD_Read
 *****************************************************************************/

/***************************************************************************************************************//**
 * @brief Switch to a different channel, global or test mode.
 * @param[in] p_api_ctrl - Pointer to the CAN control block.
 * @param[in] operation_mode - Destination CAN operation state.
 * @param[in] test_mode - Destination CAN test state.
 * @retval FSP_SUCCESS                      Operation succeeded.
 * @retval FSP_ERR_NOT_OPEN                 Control block not open.
 * @retval FSP_ERR_ASSERTION                Null pointer presented
 * @retval FSP_ERR_INVALID_MODE             Cannot change to the requested mode from the current global mode.
 * @details Switch to a different channel, global or test mode.c
 * @note None.
 *****************************************************************************************************************/
fsp_err_t R_CANFD_ModeTransition (can_ctrl_t * const   p_api_ctrl,
                                    can_operation_mode_t operation_mode,
                                    can_test_mode_t      test_mode)
{
    /* Assigned CAN Instance Control Block */
    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;

    fsp_err_t               err    = FSP_SUCCESS;

    volatile struct st_canfd0 R_BSP_EVENACCESS_SFR * canfd_ch_block_p;

    /* Assign variable canfd_ch_block_p to &CANFDn with n is channel */
    canfd_ch_block_p = CANFD_CHANNELS[p_ctrl->p_cfg->channel];

#if CANFD_CFG_PARAM_CHECKING_ENABLE
    volatile struct st_canfd R_BSP_EVENACCESS_SFR * canfd_block_p;

    /* Assign variable canfd_block_p to &CANFD */
    canfd_block_p = &CANFD;

    /* Check p_ctrl */
    FSP_ASSERT(NULL != p_ctrl);

    /* Check p_ctrl->open */
    FSP_ERROR_RETURN(p_ctrl->open == CANFD_OPEN, FSP_ERR_NOT_OPEN);

    /* Get Global Status */
    uint32_t cfdgsts = canfd_block_p->GSR.LONG;

    /* Internal Bus mode is not supported on this MCU */
    FSP_ERROR_RETURN(CAN_TEST_MODE_INTERNAL_BUS != test_mode, FSP_ERR_INVALID_MODE);

    /* Check to ensure the current mode is Global Reset when transitioning into or out of Global Sleep */
    FSP_ERROR_RETURN(((cfdgsts & R_CANFD_GSR_RSTST_Msk) && (CAN_OPERATION_MODE_RESET & operation_mode)) ||
                    (!(cfdgsts & R_CANFD_GSR_SLPST_Msk) && (CAN_OPERATION_MODE_GLOBAL_SLEEP != operation_mode)),
                    FSP_ERR_INVALID_MODE);

    /* Check to ensure the current Global mode supports the requested Channel mode, if applicable. The requested mode
     * and the current global mode are converted into a number 0-3 corresponding to Operation, Halt, Reset and Sleep
     * respectively. The channel mode cannot be switched to a mode with an index lower than the current global mode. */
    if (operation_mode < CAN_OPERATION_MODE_GLOBAL_OPERATION)
    {
        /* Check operation_mode */
        FSP_ERROR_RETURN(g_mode_order[operation_mode] >= g_mode_order[cfdgsts & CANFD_PRV_CTR_MODE_MASK],
                        FSP_ERR_INVALID_MODE);
    }
#endif /* CANFD_CFG_PARAM_CHECKING_ENABLE */

    if (p_ctrl->test_mode != test_mode)
    {
        {
            /* Transition to Channel Halt when changing test modes */
            r_canfd_mode_transition(p_ctrl, CAN_OPERATION_MODE_HALT);

            /* Set channel test mode */
            uint32_t cfdcnctr = canfd_ch_block_p->CHCR.LONG;

            /* Clear CTME and CTMS */
            cfdcnctr &= (~(R_CANFD_CHCR_CTME_Msk | R_CANFD_CHCR_CTMS_Msk));

            /* Write CHCR */
            canfd_ch_block_p->CHCR.LONG = cfdcnctr | ((uint32_t)test_mode << R_CANFD_CHCR_CTME_Pos);
        }

        p_ctrl->test_mode = test_mode;
    }

    if (p_ctrl->operation_mode != operation_mode)
    {
        r_canfd_mode_transition(p_ctrl, operation_mode);
    }

    return err;
}
/******************************************************************************
 End of function R_CANFD_ModeTransition
 *****************************************************************************/

/***************************************************************************************************************//**
 * @brief Get CANFD state and status information for the channel.
 * @param[in] p_api_ctrl - Handle for channel (pointer to channel control block)
 * @param[in] p_info - Memory address to return channel specific data to.
 * @retval  FSP_SUCCESS                     Operation succeeded.
 * @retval  FSP_ERR_NOT_OPEN                Control block not open.
 * @retval  FSP_ERR_ASSERTION               Null pointer presented
 * @details Get CANFD state and status information for the channel.
 * @note None.
 *****************************************************************************************************************/
fsp_err_t R_CANFD_InfoGet (can_ctrl_t * const p_api_ctrl, can_info_t * const p_info)
{
    volatile struct st_canfd0 R_BSP_EVENACCESS_SFR * canfd_ch_block_p;
    volatile struct st_canfd R_BSP_EVENACCESS_SFR * canfd_block_p;


#if CANFD_CFG_PARAM_CHECKING_ENABLE
    /* Assigned CAN Instance Control Block */
    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;

    /* Check pointers for NULL values */
    FSP_ASSERT(NULL != p_ctrl);

    /* Check pointers for NULL values */
    FSP_ASSERT(NULL != p_info);

    /* If channel is not open, return an error */
    FSP_ERROR_RETURN(p_ctrl->open == CANFD_OPEN, FSP_ERR_NOT_OPEN);
#else
    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;
#endif

    /* Assign variable canfd_ch_block_p to &CANFDn with n is channel */
    canfd_ch_block_p = CANFD_CHANNELS[p_ctrl->p_cfg->channel];

    /* Assign variable canfd_block_p to &CANFD */
    canfd_block_p = &CANFD;

    uint32_t cfdcnsts = canfd_ch_block_p->CHSR.LONG;
    p_info->status               = cfdcnsts & UINT16_MAX;

    /* Cast to unit8_t */
    p_info->error_count_receive  = (uint8_t) ((cfdcnsts & R_CANFD_CHSR_REC_Msk) >> R_CANFD_CHSR_REC_Pos);

    /* Cast to unit8_t */
    p_info->error_count_transmit = (uint8_t) ((cfdcnsts & R_CANFD_CHSR_TEC_Msk) >> R_CANFD_CHSR_TEC_Pos);
    p_info->error_code           = canfd_ch_block_p->CHESR.LONG & UINT16_MAX;
    p_info->rx_mb_status         = canfd_block_p->RMNDR.LONG;
    p_info->rx_fifo_status       = (~canfd_block_p->FESR.LONG) & CANFD_PRV_RX_FIFO_STATUS_MASK;

    /* Clear error flags */
    canfd_ch_block_p->CHESR.LONG &= (~((uint32_t)UINT16_MAX));

    return FSP_SUCCESS;
}
/******************************************************************************
 End of function R_CANFD_InfoGet
 *****************************************************************************/

/*******************************************************************************************************************//**
 * @brief Updates the user callback with the option to provide memory for the callback argument structure.
 * @param[in] p_api_ctrl - Control block set in can_api_t::open call.
 * @param[in] p_callback - Callback function to register
 * @param[in] p_context - Pointer to send to callback function
 * @param[in] p_callback_memory - Pointer to volatile memory where callback structure can be allocated. Callback
 * arguments allocated here are only valid during the callback.
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 * @retval  FSP_ERR_NO_CALLBACK_MEMORY   p_callback is non-secure and p_callback_memory is either secure or NULL.
 * @details Updates the user callback with the option to provide memory for the callback argument structure.
 * @note None.
 **********************************************************************************************************************/
fsp_err_t R_CANFD_CallbackSet (can_ctrl_t * const          p_api_ctrl,
                                void (                    * p_callback)(can_callback_args_t *),
                                void const * const          p_context,
                                can_callback_args_t * const p_callback_memory)
{
    /* Assigned CAN Instance Control Block */
    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) p_api_ctrl;

#if CANFD_CFG_PARAM_CHECKING_ENABLE
    /* Check pointers for NULL values */
    FSP_ASSERT(p_ctrl);

    /* Check pointers for NULL values */
    FSP_ASSERT(p_callback);

    /* Check p_ctrl->open */
    FSP_ERROR_RETURN(CANFD_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Store callback and context */
    p_ctrl->p_callback = p_callback;

    p_ctrl->p_context         = p_context;
    p_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}
/**********************************************************************************************************************
 End of function R_CANFD_CallbackSet
 *********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @} (end addtogroup CAN)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
#if CANFD_CFG_PARAM_CHECKING_ENABLE
/******************************************************************************
 * Function Name: r_canfd_bit_timing_parameter_check
 * Description  : .
 * Argument     : p_bit_timing
 * Return Value : .
 *****************************************************************************/
static bool r_canfd_bit_timing_parameter_check (can_bit_timing_cfg_t * const p_bit_timing)
{
    /* Check that prescaler is in range */
    FSP_ERROR_RETURN((p_bit_timing->baud_rate_prescaler <= CANFD_BAUD_RATE_PRESCALER_MAX) &&
                    (p_bit_timing->baud_rate_prescaler >= CANFD_BAUD_RATE_PRESCALER_MIN),
                    false);

    /* Check that TSEG1 > TSEG2 >= SJW for nominal bitrate  */

    /* Check Time Segment 1 is greater than Time Segment 2 */
    FSP_ERROR_RETURN((uint32_t)p_bit_timing->time_segment_1 > (uint32_t)p_bit_timing->time_segment_2, false);

    /* Check Time Segment 2 is greater than or equal to the synchronization jump width */
    FSP_ERROR_RETURN((uint32_t)p_bit_timing->time_segment_2 >= (uint32_t)p_bit_timing->synchronization_jump_width,
                    false);

    return true;
}
/******************************************************************************
 End of function r_canfd_bit_timing_parameter_check
 *****************************************************************************/


#endif

/*******************************************************************************************************************//**
 * Read from a Message Buffer or FIFO.
 *
 * NOTE: Does not index FIFOs.
 *
 * @param[in]     buffer     Index of buffer to read from (MBs 0-31, FIFOs 32+)
 * @param[in]     frame      Pointer to CAN frame to write to
 **********************************************************************************************************************/
/******************************************************************************
 * Function Name: r_canfd_mb_read
 * Description  : Read from a Message Buffer or FIFO.
 * Arguments    : buffer Index of buffer to read from (MBs 0-31, FIFOs 32+)
 *              : frame Pointer to CAN frame to write to
 * Return Value : .
 *****************************************************************************/
static void r_canfd_mb_read (uint32_t buffer, can_frame_t * const frame)
{
    volatile struct st_canfd R_BSP_EVENACCESS_SFR * canfd_block_p;

    /* Assign variable canfd_block_p to &CANFD */
    canfd_block_p = &CANFD;

    bool is_mb = buffer < CANFD_PRV_RXMB_MAX;

    /* Get pointer to message buffer (FIFOs use the same buffer structure) */
    volatile canfd_rmb_t * p_mb_regs =
        (is_mb) ? (volatile canfd_rmb_t *)((&CANFD.RMB0) + (buffer*76)) : /* address of RMBn = &RMB(n-1) + 76 bytes */
        (volatile canfd_rmb_t *) (&CANFD.RFB[buffer-CANFD_PRV_RXMB_MAX]);

    /* Get frame data. */
    uint32_t id = p_mb_regs->HF0.LONG;

    /* Get the frame type */
    frame->type = (can_frame_type_t) ((id & R_CANFD_RMBn_HF0_RTR_Msk) >> R_CANFD_RMBn_HF0_RTR_Pos);

    /* Get FD status bits (ESI, BRS and FDF) */
    frame->options = p_mb_regs->HF2.LONG & 7U;

    /* Get the frame ID */
    frame->id = id & R_CANFD_RMBn_HF0_ID_Msk;

    /* Get the frame ID mode (IDE bit) */
    frame->id_mode = (can_id_mode_t) (id >> R_CANFD_RMBn_HF0_IDE_Pos);

    /* Get the frame data length code */
    frame->data_length_code = dlc_to_bytes[p_mb_regs->HF1.LONG >> R_CANFD_RMBn_HF1_DLC_Pos];

    /* Copy data to frame */
    uint32_t  len    = frame->data_length_code;

    uint8_t * p_dest = frame->data;

    /* Cast to unit8_t */
    uint8_t * p_src  = (uint8_t *) (p_mb_regs->DATA);

    /* WAIT_LOOP */
    while (len--)
    {
        *p_dest++ = *p_src++;
    }

    if (is_mb)
    {
        /* Clear RXMB New Data bit */
        canfd_block_p->RMNDR.LONG &= (~(1U << buffer));
    }
    else
    {
        /* Increment RX FIFO pointer */
        canfd_block_p->RFPCR[buffer - CANFD_PRV_RXMB_MAX] = UINT8_MAX;
    }
}
/******************************************************************************
 End of function r_canfd_mb_read
 *****************************************************************************/


/*******************************************************************************************************************//**
 * Calls user callback.
 *
 * @param[in]     p_ctrl     Pointer to CAN instance control block
 * @param[in]     p_args     Pointer to arguments on stack
 **********************************************************************************************************************/
/******************************************************************************
 * Function Name: r_canfd_call_callback
 * Description  : Calls user callback.
 * Arguments    : p_ctrl Pointer to CAN instance control block
 *              : p_args Pointer to arguments on stack
 * Return Value : .
 *****************************************************************************/
static void r_canfd_call_callback (canfd_instance_ctrl_t * p_ctrl, can_callback_args_t * p_args)
{
    can_callback_args_t args;

    /* Store callback arguments in memory provided by user if available.  This allows callback arguments to be
     * stored in non-secure memory so they can be accessed by a non-secure callback function. */
    can_callback_args_t * p_args_memory = p_ctrl->p_callback_memory;
    if (NULL == p_args_memory)
    {
        /* Use provided args struct on stack */
        p_args_memory = p_args;
    }
    else
    {
        /* Save current arguments on the stack in case this is a nested interrupt. */
        args = *p_args_memory;

        /* Copy the stacked args to callback memory */
        *p_args_memory = *p_args;
    }

    /* If the project is not Trustzone Secure, then it will never need to change security state in order to call the callback. */
    p_ctrl->p_callback(p_args_memory);

    if (NULL != p_ctrl->p_callback_memory)
    {
        /* Restore callback memory in case this is a nested interrupt. */
        *p_ctrl->p_callback_memory = args;
    }
}
/******************************************************************************
 End of function r_canfd_call_callback
 *****************************************************************************/

/******************************************************************************
* Function Name:    canfd_error_isr_common
* Description  :    Common ISR handler for CANFD Global Error and CANFD0 Channel
*                   Error.
* Arguments    :    None
* Return Value :    None
******************************************************************************/
static void canfd_error_isr_common (void)
{
    volatile struct st_canfd0 R_BSP_EVENACCESS_SFR * canfd0_block_p;
    volatile struct st_canfd R_BSP_EVENACCESS_SFR * canfd_block_p;

    /* Assign variable canfd0_block_p to &CANFD0 */
    canfd0_block_p = &CANFD0;

    /* Assign variable canfd_block_p to &CANFD */
    canfd_block_p = &CANFD;

    /* Get IRQ and context */
    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) gp_ctrl[0];

    can_callback_args_t     args = {0U};
    canfd_instance_ctrl_t * p_callback_ctrl;

    if (0 != canfd_block_p->GESR.LONG)
    {
        args.event = CAN_EVENT_ERR_GLOBAL;

        /* Read global error flags. */
        uint32_t cfdgerfl = canfd_block_p->GESR.LONG;

        /* Global errors are in the top halfword of canfd_error_t; move and preserve ECC error flags. */
        args.error = ((cfdgerfl & UINT16_MAX) << 16) + ((cfdgerfl >> 16) << 28);

        /* Clear global error flags. */
        canfd_block_p->GESR.LONG = 0;

        if (args.error & CANFD_ERROR_GLOBAL_MESSAGE_LOST)
        {
            /* Get lowest RX FIFO with Message Lost condition and clear the flag */
            args.buffer = trailing_zeros(canfd_block_p->FMLSR.LONG);

            canfd_block_p->RFSR[args.buffer].LONG &= (~R_CANFD_RFSR_LOST_Msk);
        }

        /* Choose ctrl block for the selected global error handler channel. */
        p_callback_ctrl = p_ctrl;

        /* Set channel and context based on selected global error handler channel. */
        args.channel   = p_ctrl->p_cfg->channel;

        args.p_context = p_callback_ctrl->p_context;
    }
    else
    {
        args.event = CAN_EVENT_ERR_CHANNEL;

        /* Read and clear channel error flags. */
        uint32_t channel = p_ctrl->p_cfg->channel;
        args.error = canfd0_block_p->CHESR.LONG & UINT16_MAX; // Upper halfword contains latest CRC
        canfd0_block_p->CHESR.LONG = 0;

        /* Choose the channel provided by the interrupt context. */
        p_callback_ctrl = p_ctrl;

        args.channel   = channel;
        args.p_context = p_ctrl->p_context;
        args.buffer    = 0U;
    }

    /* Set remaining arguments and call callback */
    args.p_frame = NULL;
    r_canfd_call_callback(p_callback_ctrl, &args);

    /* Clear Error Flag ISR */
#if defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T)
    IR(ICU, GROUPBL2) = 0;
#else
    if (CAN_EVENT_ERR_GLOBAL == args.event)
    {
        IR(CANFD, GLEI) = 0;
    }
    else
    {
        IR(CANFD0, CHEI) = 0;
    }
#endif
}
/******************************************************************************
 End of function canfd_error_isr_common
 *****************************************************************************/


/******************************************************************************
* Function Name:    canfd_global_error_isr
* Description  :    CANFD Global Error ISR calls a common function.
* Arguments    :    None
* Return Value :    None
******************************************************************************/
#if !(defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T))

    R_BSP_PRAGMA_STATIC_INTERRUPT (canfd_global_error_isr, VECT(CANFD, GLEI))
    R_BSP_ATTRIB_STATIC_INTERRUPT void canfd_global_error_isr(void)
    {
        canfd_error_isr_common();
    } /* End of function canfd_global_error_isr */
#endif

/******************************************************************************
* Function Name:    canfd_channel_error_isr
* Description  :    CANFD Channel Error ISR calls a common function.
* Arguments    :    None
* Return Value :    None
******************************************************************************/
#if !(defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T))

    R_BSP_PRAGMA_STATIC_INTERRUPT (canfd_channel_error_isr, VECT(CANFD0, CHEI))
    R_BSP_ATTRIB_STATIC_INTERRUPT void canfd_channel_error_isr(void)
    {
        canfd_error_isr_common();
    } /* End of function canfd_channel_error_isr */
#endif


#if defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T)
/******************************************************************************
* Function Name:    canfd_error_grp_isr
* Description  :    BSP group interrupt handler for register the error callback function
* Arguments    :    None
* Return Value :    None
******************************************************************************/
static void canfd_error_grp_isr (void)
{
    /* Called from BSP group interrupt handler. */
    #if defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T)
        if ((IS(CANFD, GLEI)) || (IS(CANFD0, CHEI)))
        {
            canfd_error_isr_common();
        }
    #endif
} /* End of function canfd_error_grp_isr */
#endif


/******************************************************************************
* Function Name:    canfd_rx_fifo_isr_common
* Description  :    Common ISR handler for CANFD RX FIFO.
* Arguments    :    None
* Return Value :    None
******************************************************************************/
static void canfd_rx_fifo_isr_common (void)
{
    volatile struct st_canfd R_BSP_EVENACCESS_SFR * canfd_block_p;
    canfd_block_p = &CANFD;

    can_callback_args_t args;

    /* Get lowest FIFO requesting interrupt */
    uint32_t fifo = trailing_zeros(canfd_block_p->RFISR.LONG);

    /* Only perform ISR duties if a FIFO has requested it */
    if (fifo < CANFD_PRV_RX_FIFO_MAX)
    {
        /* Set static arguments */
        args.p_frame = &args.frame;
        args.event   = CAN_EVENT_RX_COMPLETE;
        args.buffer  = fifo + CANFD_PRV_RXMB_MAX;

        /* Read from the FIFO until it is empty */
        /* WAIT_LOOP */
        while (!(canfd_block_p->FESR.LONG & (1U << fifo)))
        {
            /* Get channel associated with the AFL entry */
            args.channel = canfd_block_p->RFB[fifo].HF2.BIT.IFL;

            /* Read and index FIFO */
            r_canfd_mb_read(fifo + CANFD_PRV_RXMB_MAX, &args.frame);

            /* Set the remaining callback arguments */
            args.p_context = gp_ctrl[args.channel]->p_context;
            r_canfd_call_callback(gp_ctrl[args.channel], &args);
        }

        /* Clear RX FIFO Interrupt Flag */
        canfd_block_p->RFSR[fifo].LONG &= (~R_CANFD_RFSR_RFIF_Msk);
    }

    if (!canfd_block_p->RFISR.LONG)
    {
        /* Clear interrupt in NVIC if there are no pending RX FIFO IRQs */
        #if defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T)
        IR(ICU, GROUPBL2)  = 0;
        #else
        IR(CANFD, RFRI)  = 0;
        #endif
    }
}
/******************************************************************************
 End of function canfd_rx_fifo_isr_common
 *****************************************************************************/


/******************************************************************************
* Function Name:    canfd_rx_fifo_isr
* Description  :    CANFD RX FIFO ISR.
*                   CANFD RX FIFO ISR calls a common function.
* Arguments    :    None
* Return Value :    None
******************************************************************************/
#if !(defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T))

    R_BSP_PRAGMA_STATIC_INTERRUPT (canfd_rx_fifo_isr, VECT(CANFD, RFRI))
    R_BSP_ATTRIB_STATIC_INTERRUPT void canfd_rx_fifo_isr(void)
    {
        canfd_rx_fifo_isr_common();
    } /* End of function canfd_rx_fifo_isr */
#endif


#if defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T)
/******************************************************************************
* Function Name:    canfd_rx_fifo_grp_isr
* Description  :    BSP group interrupt handler for register the CANFD RX FIFO
*                   callback function
* Arguments    :    None
* Return Value :    None
******************************************************************************/
static void canfd_rx_fifo_grp_isr (void)
{
    /* Called from BSP group interrupt handler. */
    #if defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T)
        if (IS(CANFD, RFRI))
        {
            canfd_rx_fifo_isr_common();
        }
    #endif
} /* End of function canfd_rx_fifo_grp_isr */
#endif


/******************************************************************************
* Function Name:    canfd_channel_tx_isr_common
* Description  :    Common ISR handler for CANFD0 TX Channel.
* Arguments    :    None
* Return Value :    None
******************************************************************************/
static void canfd_channel_tx_isr_common (void)
{
    volatile struct st_canfd R_BSP_EVENACCESS_SFR * canfd_block_p;
    canfd_block_p = &CANFD;

    /* Casting gp_ctrl[0] into pointer and struct canfd_instance_ctrl_t */
    canfd_instance_ctrl_t * p_ctrl = (canfd_instance_ctrl_t *) gp_ctrl[0];

    uint32_t                channel = 0;

    /* Set static arguments */
    can_callback_args_t args;
    args.channel   = channel;

    args.p_context = p_ctrl->p_context;
    args.p_frame   = NULL;

    /* Check the byte of tisr that corresponds to the interrupting channel */
    uint32_t tisr  = *((uint8_t *)(&canfd_block_p->TISR.LONG) + channel);

    /* WAIT_LOOP */
    while (tisr)
    {
        uint32_t txmb;

        volatile uint32_t * p_tmt_sts;

        /* Get relevant TX status register bank */
        if (tisr & R_CANFD_TISR_TSIF0_Msk)
        {
            /* Casting the register into the unsigned interger pointer */
            p_tmt_sts  = (volatile uint32_t *) &canfd_block_p->TMTCSR0.LONG;
            args.event = CAN_EVENT_TX_COMPLETE;
        }
        else
        {
            /* Casting the register into the unsigned interger pointer */
            p_tmt_sts  = (volatile uint32_t *) &canfd_block_p->TMTASR0.LONG;

            args.event = CAN_EVENT_TX_ABORTED;
        }

        /* Calculate lowest TXMB with the specified event */
        txmb = trailing_zeros(*p_tmt_sts);

        /* Clear TX complete/abort flags */
        canfd_block_p->TMSR[txmb].BIT.TXRF = 0;

        /* Set the callback arguments */
        args.buffer = txmb;
        r_canfd_call_callback(gp_ctrl[args.channel], &args);

        /* Check for more interrupts on this channel */
        tisr = *((uint8_t *)(&canfd_block_p->TISR.LONG) + channel);
    }

    /* Clear interrupt */
    #if defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T)
    IR(ICU, GROUPBL2)  = 0;
    #else
    IR(CANFD0, CHTI)  = 0;
    #endif
} /* End of function canfd_channel_tx_isr_common */

/******************************************************************************
* Function Name:    canfd_channel_tx_isr
* Description  :    CANFD0 Channel TX ISR calls a common function.
* Arguments    :    None
* Return Value :    None
******************************************************************************/
#if !(defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T))

    R_BSP_PRAGMA_STATIC_INTERRUPT (canfd_channel_tx_isr, VECT(CANFD0, CHTI))
    R_BSP_ATTRIB_STATIC_INTERRUPT void canfd_channel_tx_isr(void)
    {
        canfd_channel_tx_isr_common();
    } /* End of function canfd_channel_tx_isr */
#endif


#if defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T)
/******************************************************************************
* Function Name:    canfd_channel_tx_grp_isr
* Description  :    BSP group interrupt handler for register the CANFD0 Channel
*                   TX callback function
* Arguments    :    None
* Return Value :    None
******************************************************************************/
static void canfd_channel_tx_grp_isr (void)
{
    /* Called from BSP group interrupt handler. */
    #if defined (BSP_MCU_RX660) || defined (BSP_MCU_RX26T)
        if (IS(CANFD0, CHTI))
        {
            canfd_channel_tx_isr_common();
        }
    #endif
} /* End of function canfd_channel_tx_grp_isr */
#endif


/***************************************************************************************************************//**
 * @brief Switch to a different channel, global or test mode.
 * @param[in] p_api_ctrl - Pointer to the CAN control block.
 * @param[in] operation_mode - Destination CAN operation state.
 * @param[in] test_mode - Destination CAN test state.
 * @retval FSP_SUCCESS                      Operation succeeded.
 * @retval FSP_ERR_NOT_OPEN                 Control block not open.
 * @retval FSP_ERR_ASSERTION                Null pointer presented
 * @retval FSP_ERR_INVALID_MODE             Cannot change to the requested mode from the current global mode.
 * @details Switch to a different channel, global or test mode.c
 * @note None.
 *****************************************************************************************************************/
/******************************************************************************
 * Function Name: r_canfd_mode_transition
 * Description  : Switch to a different channel, global or test mode.
 * Arguments    : p_ctrl         Pointer to the CAN control block.
 *              : operation_mode Destination CAN operation state.
 * Return Value : FSP_ERR_NOT_OPEN        Control block not open.
 *                FSP_ERR_ASSERTION       Null pointer presented.
 *                FSP_ERR_INVALID_MODE    Cannot change to the requested mode
 *                                        from the current global mode.
 *****************************************************************************/
static void r_canfd_mode_transition (canfd_instance_ctrl_t * p_ctrl, can_operation_mode_t operation_mode)
{
    volatile struct st_canfd0 R_BSP_EVENACCESS_SFR * canfd_ch_block_p;
    volatile struct st_canfd R_BSP_EVENACCESS_SFR * canfd_block_p;

    /* Assign variable canfd_ch_block_p to &CANFDn with n is the channel */
    canfd_ch_block_p = CANFD_CHANNELS[p_ctrl->p_cfg->channel];

    /* Assign variable canfd_block_p to &CANFD */
    canfd_block_p = &CANFD;

    /* Get bit 7 from operation_mode to determine if this is a global mode change request */
    bool global_mode = (bool) (operation_mode >> 7);
    operation_mode &= 0xF;

    if (global_mode)
    {
        uint32_t cfdgctr = canfd_block_p->GCR.LONG;

        /* Cast to volatile uint32_t */
        r_canfd_mode_ctr_set((volatile uint32_t *)&canfd_block_p->GCR.LONG, operation_mode);

        /* If CANFD is transitioning out of Reset the FIFOs need to be enabled. */
        if ((cfdgctr & R_CANFD_GSR_RSTST_Msk) && (!(operation_mode & CAN_OPERATION_MODE_RESET)))
        {
            /* Get global configuration */
            canfd_global_cfg_t * p_global_cfg = ((canfd_extended_cfg_t *) p_ctrl->p_cfg->p_extend)->p_global_cfg;

            /* Enable RX FIFOs */
            /* WAIT_LOOP */
            for (uint32_t i = 0; i < CANFD_PRV_RX_FIFO_MAX; i++)
            {
                canfd_block_p->RFCR[i].LONG = p_global_cfg->rx_fifo_config[i];
            }
        }
    }
    else
    {
        uint32_t cfdcnctr = canfd_ch_block_p->CHCR.LONG;

        if (((cfdcnctr & R_CANFD_CHCR_SLPRQ_Msk) && (!(CAN_OPERATION_MODE_RESET & operation_mode))) ||
            ((!(cfdcnctr & CANFD_PRV_CTR_RESET_BIT)) && (CAN_OPERATION_MODE_SLEEP == operation_mode)))
        {
            /* Transition channel to Reset if a transition to/from Sleep is requested */
            r_canfd_mode_ctr_set((volatile uint32_t *) &canfd_ch_block_p->CHCR.LONG, CAN_OPERATION_MODE_RESET);
        }

        /* Request transition to selected mode */
        r_canfd_mode_ctr_set((volatile uint32_t *) &canfd_ch_block_p->CHCR.LONG, operation_mode);
    }

    /* Store operation mode */
    p_ctrl->operation_mode = (can_operation_mode_t) (canfd_ch_block_p->CHCR.LONG & CANFD_PRV_CTR_MODE_MASK);
}
/******************************************************************************
 End of function r_canfd_mode_transition
 *****************************************************************************/


/*******************************************************************************************************************//**
 * Sets the provided CTR register to the requested mode and waits for the associated STS register to reflect the change
 * @param[in]  p_ctr_reg            - pointer to control register
 * @param[in]  operation_mode       - requested mode (not including global bits)
 **********************************************************************************************************************/
/******************************************************************************
 * Function Name: r_canfd_mode_ctr_set
 * Description  : Sets the provided CTR register to the requested mode and waits
 *                for the associated STS register to reflect the change.
 * Arguments    : p_ctr_reg      pointer to control register
 *              : operation_mode requested mode (not including global bits)
 * Return Value : .
 *****************************************************************************/
static void r_canfd_mode_ctr_set (volatile uint32_t * p_ctr_reg, can_operation_mode_t operation_mode)
{
    volatile uint32_t * p_sts_reg = p_ctr_reg + 1;

    /* See definitions for CFDCnCTR, CFDCnSTS, CFDGCTR and CFDGSTS in the RX660 User's Manual (R01UH0937EJ) */
    *p_ctr_reg = ((*p_ctr_reg) & (~CANFD_PRV_CTR_MODE_MASK)) | operation_mode;

    /* WAIT_LOOP */
    FSP_HARDWARE_REGISTER_WAIT((*p_sts_reg & CANFD_PRV_CTR_MODE_MASK), operation_mode);
}
/******************************************************************************
 End of function r_canfd_mode_ctr_set
 *****************************************************************************/


/*******************************************************************************************************************//**
 * Converts bytes into a DLC value
 * @param[in]  bytes       Number of payload bytes
 **********************************************************************************************************************/
/******************************************************************************
 * Function Name: r_canfd_bytes_to_dlc
 * Description  : Converts bytes into a DLC value.
 * Argument     : bytes Number of payload bytes
 * Return Value : .
 *****************************************************************************/
static uint8_t r_canfd_bytes_to_dlc (uint8_t bytes)
{
    if (bytes <= 8)
    {
        return bytes;
    }

    if (bytes <= 24)
    {
        /* Cast to unit8_t */
        return (uint8_t) (8U + ((bytes - 8U) / 4U));
    }

    /* Cast to unit8_t */
    return (uint8_t) (0xDU + ((bytes / 16U) - 2U));
}
/******************************************************************************
 End of function r_canfd_bytes_to_dlc
 *****************************************************************************/


/******************************************************************************
 * Function Name: trailing_zeros
 * Description  : returns the number of trailing zeros in n.
 * Argument     : n
 * Return Value : .
 *****************************************************************************/
static uint32_t trailing_zeros(uint32_t n)
{
    uint32_t count = 0;

/*
     the following loop runs while
     n is greater than zero and right most bit of n is 0
     ( n & 1 ) performs bitwise AND of n and 1
     if the rightmost digit of n is 1, ( n & 1 ) outputs 1
     if the rightmost digit of n is 0, ( n & 1 ) outputs 0
*/
    /* WAIT_LOOP */
    while ((n > 0) && ((n & 1) == 0))
    {
        count = count + 1;  /* increment count */
        n = n >> 1;         /* right shift n by one */
    }

    return count;
}
/******************************************************************************
 End of function trailing_zeros
 *****************************************************************************/

