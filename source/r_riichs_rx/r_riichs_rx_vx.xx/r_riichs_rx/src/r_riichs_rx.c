/***********************************************************************************************************************
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
 * Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_riichs_rx.c
 * Description  : Functions for using RIICHS on RX devices. 
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * History      : DD.MM.YYYY Version  Description
 *              : 30.06.2021 1.00     First Release
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
/* Defines for RIICHS support */
#include "r_riichs_rx_if.h"
#include "r_riichs_rx_private.h"

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
/*----------------------------------------------------------------------------*/
/*   riichs information                                                       */
/*----------------------------------------------------------------------------*/
riichs_ch_dev_status_t g_riichs_ChStatus[RIICHS_MAX_CH_NUM]; /* Channel status */

/*----------------------------------------------------------------------------*/
/*   Callback function                                                        */
/*----------------------------------------------------------------------------*/
volatile riichs_callback g_riichs_callbackfunc_m[RIICHS_MAX_CH_NUM];
volatile riichs_callback g_riichs_callbackfunc_s[RIICHS_MAX_CH_NUM];

/***********************************************************************************************************************
 Private global variables and functions
 **********************************************************************************************************************/
/*----------------------------------------------------------------------------*/
/*   riichs information                                                       */
/*----------------------------------------------------------------------------*/
static riichs_info_t * priichs_info_m[RIICHS_MAX_CH_NUM]; /* IIC driver information */
static riichs_info_t * priichs_info_s[RIICHS_MAX_CH_NUM]; /* IIC driver information */

static riichs_api_event_t riichs_api_event[RIICHS_MAX_CH_NUM]; /* Event flag */
static riichs_api_info_t riichs_api_info[RIICHS_MAX_CH_NUM]; /* Internal status management */

static uint32_t g_riichs_bps[RIICHS_MAX_CH_NUM];
static uint32_t g_riichs_icfer_init[RIICHS_MAX_CH_NUM];
static uint32_t g_riichs_icicr_init[RIICHS_MAX_CH_NUM];
static uint32_t g_riichs_icser_init[RIICHS_MAX_CH_NUM];
static uint32_t g_riichs_ictor_init[RIICHS_MAX_CH_NUM];

uint8_t g_riichs_rxi_int_priority[RIICHS_MAX_CH_NUM];
uint8_t g_riichs_txi_int_priority[RIICHS_MAX_CH_NUM];
uint8_t g_riichs_eei_int_priority[RIICHS_MAX_CH_NUM];
uint8_t g_riichs_tei_int_priority[RIICHS_MAX_CH_NUM];
uint8_t g_riichs_timeout_enable[RIICHS_MAX_CH_NUM];

/*----------------------------------------------------------------------------*/
/*   Main processing of RIICHS Driver API functions                           */
/*----------------------------------------------------------------------------*/
static void riichs_get_initial_setting (riichs_info_t * p_riichs_info);
static riichs_return_t riichs_open (riichs_info_t *);
static riichs_return_t riichs_master_send (riichs_info_t *);
static riichs_return_t riichs_master_receive (riichs_info_t *);
static riichs_return_t riichs_master_send_receive (riichs_info_t *);
static riichs_return_t riichs_slave_transfer (riichs_info_t *);
static void riichs_getstatus (riichs_info_t *, riichs_mcu_status_t *);
static riichs_return_t riichs_control (riichs_info_t *, uint8_t ctrl_ptn);
static void riichs_close (riichs_info_t *);
static riichs_return_t r_riichs_advance (riichs_info_t *);
static riichs_return_t riichs_advance (riichs_info_t *);

/*----------------------------------------------------------------------------*/
/*   Called from function table                                               */
/*----------------------------------------------------------------------------*/
static riichs_return_t riichs_func_table (riichs_api_event_t, riichs_info_t *);
static riichs_return_t riichs_init_driver (riichs_info_t *);
static riichs_return_t riichs_generate_start_cond (riichs_info_t *);
static riichs_return_t riichs_send_master_code_cond (riichs_info_t *);
static riichs_return_t riichs_after_gen_start_cond (riichs_info_t *);
static riichs_return_t riichs_after_send_slvadr (riichs_info_t *);
static riichs_return_t riichs_after_receive_slvadr (riichs_info_t *);
static riichs_return_t riichs_write_data_sending (riichs_info_t *);
static riichs_return_t riichs_read_data_receiving (riichs_info_t *);
static riichs_return_t riichs_after_dtct_stop_cond (riichs_info_t *);
static riichs_return_t riichs_arbitration_lost (riichs_info_t *);
static riichs_return_t riichs_nack (riichs_info_t *);
static riichs_return_t riichs_enable_slave_transfer (riichs_info_t *);
static riichs_return_t riichs_time_out (riichs_info_t *);

/*----------------------------------------------------------------------------*/
/*   Other function                                                           */
/*----------------------------------------------------------------------------*/
static void riichs_api_mode_event_init (riichs_info_t *, riichs_api_mode_t);
static void riichs_api_mode_set (riichs_info_t *, riichs_api_mode_t);
static void riichs_api_status_set (riichs_info_t *, riichs_api_status_t);
static void riichs_set_ch_status (riichs_info_t *, riichs_ch_dev_status_t);
static riichs_return_t riichs_check_chstatus_start (riichs_info_t *);
static riichs_return_t riichs_check_chstatus_advance (riichs_info_t *);
static void riichs_enable (riichs_info_t *);
static void riichs_disable (riichs_info_t *);
static void riichs_init_io_register (riichs_info_t *);
static void riichs_slv_addr_match_int_enable (riichs_info_t *);
static void riichs_int_enable (riichs_info_t *);
static void riichs_int_disable (riichs_info_t *);
static void riichs_int_icsier_iccsier_setting (riichs_info_t *, uint32_t New_icsier, uint32_t New_iccsier);
static void riichs_set_frequency (riichs_info_t *);
static bool riichs_check_bus_busy (riichs_info_t *);
static void riichs_start_cond_generate (riichs_info_t *);
static void riichs_re_start_cond_generate (riichs_info_t * p_riichs_info);
static void riichs_stop_cond_generate (riichs_info_t *);
static void riichs_set_sending_data (riichs_info_t *, uint8_t *p_data);
static uint32_t riichs_get_receiving_data (riichs_info_t *);
static void riichs_receive_wait_setting (riichs_info_t *);
static void riichs_receive_pre_end_setting (riichs_info_t *);
static void riichs_receive_end_setting (riichs_info_t *);
static void riichs_reset_clear (riichs_info_t *);
static void riichs_set_iccrice (riichs_info_t *);
static void riichs_set_icbftr (riichs_info_t *);
static void riichs_reset_set (riichs_info_t *);
static void riichs_all_reset (riichs_info_t *);
static void riichs_clear_ir_flag (riichs_info_t *);

static riichs_return_t riichs_bps_calc(riichs_info_t *, uint16_t kbps, bool first_bps_set_flag);

/* static double riichs_check_freq(void); */

/*----------------------------------------------------------------------------*/
/*   function table                                                           */
/*----------------------------------------------------------------------------*/
static const riichs_mtx_t gc_riichs_mtx_tbl[RIICHS_STS_MAX][RIICHS_EV_MAX] =
{
    /* Uninitialized state */
    {
        { RIICHS_EV_INIT, riichs_init_driver }, /* IIC Driver Initialization */
        { RIICHS_EV_EN_SLV_TRANSFER, NULL }, /* Enable Slave Transfer */
        { RIICHS_EV_GEN_START_COND, NULL }, /* Start Condition Generation */
        { RIICHS_EV_INT_START, NULL }, /* Start Condition Generation Interrupt */
        { RIICHS_EV_INT_ADD, NULL }, /* Address Sending Interrupt */
        { RIICHS_EV_INT_SEND, NULL }, /* Data Sending Interrupt */
        { RIICHS_EV_INT_RECEIVE, NULL }, /* Data Receiving Interrupt */
        { RIICHS_EV_INT_STOP, NULL }, /* Stop Condition Generation Interrupt */
        { RIICHS_EV_INT_AL, NULL }, /* Arbitration-Lost Interrupt */
        { RIICHS_EV_INT_NACK, NULL }, /* No Acknowledge Interrupt */
        { RIICHS_EV_INT_TMO, NULL }, /* Time out Interrupt */
    },

    /* Idle state */
    {
        { RIICHS_EV_INIT, NULL }, /* IIC Driver Initialization */
        { RIICHS_EV_EN_SLV_TRANSFER, riichs_enable_slave_transfer }, /* Enable Slave Transfer */
        { RIICHS_EV_GEN_START_COND, riichs_generate_start_cond }, /* Start Condition Generation */
        { RIICHS_EV_INT_START, NULL }, /* Start Condition Generation Interrupt */
        { RIICHS_EV_INT_ADD, NULL }, /* Address Sending Interrupt */
        { RIICHS_EV_INT_SEND, NULL }, /* Data Sending Interrupt */
        { RIICHS_EV_INT_RECEIVE, NULL }, /* Data Receiving Interrupt */
        { RIICHS_EV_INT_STOP, NULL }, /* Stop Condition Generation Interrupt */
        { RIICHS_EV_INT_AL, NULL }, /* Arbitration-Lost Interrupt */
        { RIICHS_EV_INT_NACK, NULL }, /* No Acknowledge Interrupt */
        { RIICHS_EV_INT_TMO, NULL }, /* Time out Interrupt */
    },
    /* Idle state on enable slave transfer */
    {
        { RIICHS_EV_INIT, NULL }, /* IIC Driver Initialization */
        { RIICHS_EV_EN_SLV_TRANSFER, NULL }, /* Enable Slave Transfer */
        { RIICHS_EV_GEN_START_COND, riichs_generate_start_cond }, /* Start Condition Generation */
        { RIICHS_EV_INT_START, NULL }, /* Start Condition Generation Interrupt */
        { RIICHS_EV_INT_ADD, riichs_after_receive_slvadr }, /* Address Sending Interrupt */
        { RIICHS_EV_INT_SEND, NULL }, /* Data Sending Interrupt */
        { RIICHS_EV_INT_RECEIVE, riichs_after_receive_slvadr }, /* Data Receiving Interrupt */
        { RIICHS_EV_INT_STOP, NULL }, /* Stop Condition Generation Interrupt */
        { RIICHS_EV_INT_AL, NULL }, /* Arbitration-Lost Interrupt */
        { RIICHS_EV_INT_NACK, NULL }, /* No Acknowledge Interrupt */
        { RIICHS_EV_INT_TMO, NULL }, /* Time out Interrupt */
    },

    /* Start condition generation completion wait state */
    {
        { RIICHS_EV_INIT, NULL }, /* IIC Driver Initialization */
        { RIICHS_EV_EN_SLV_TRANSFER, NULL }, /* Enable Slave Transfer */
        { RIICHS_EV_GEN_START_COND, NULL }, /* Start Condition Generation */
        { RIICHS_EV_INT_START, riichs_after_gen_start_cond }, /* Start Condition Generation Interrupt */
        { RIICHS_EV_INT_ADD, NULL }, /* Address Sending Interrupt */
        { RIICHS_EV_INT_SEND, NULL }, /* Data Sending Interrupt */
        { RIICHS_EV_INT_RECEIVE, NULL }, /* Data Receiving Interrupt */
        { RIICHS_EV_INT_STOP, NULL }, /* Stop Condition Generation Interrupt */
        { RIICHS_EV_INT_AL, riichs_arbitration_lost }, /* Arbitration-Lost Interrupt */
        { RIICHS_EV_INT_NACK, riichs_nack }, /* No Acknowledge Interrupt */
        { RIICHS_EV_INT_TMO, riichs_time_out }, /* Time out Interrupt */
    },

    /* Send master code completion wait state */
    {
        { RIICHS_EV_INIT, NULL }, /* IIC Driver Initialization */
        { RIICHS_EV_EN_SLV_TRANSFER, NULL }, /* Enable Slave Transfer */
        { RIICHS_EV_GEN_START_COND, NULL }, /* Start Condition Generation */
        { RIICHS_EV_INT_START, riichs_send_master_code_cond }, /* Start Condition Generation Interrupt */
        { RIICHS_EV_INT_ADD, NULL }, /* Address Sending Interrupt */
        { RIICHS_EV_INT_SEND, NULL }, /* Data Sending Interrupt */
        { RIICHS_EV_INT_RECEIVE, NULL }, /* Data Receiving Interrupt */
        { RIICHS_EV_INT_STOP, NULL }, /* Stop Condition Generation Interrupt */
        { RIICHS_EV_INT_AL, riichs_arbitration_lost }, /* Arbitration-Lost Interrupt */
        { RIICHS_EV_INT_NACK, NULL }, /* No Acknowledge Interrupt */
        { RIICHS_EV_INT_TMO, riichs_time_out }, /* Time out Interrupt */
    },

    /* Slave address [Write] transmission completion wait state */
    {
        { RIICHS_EV_INIT, NULL }, /* IIC Driver Initialization */
        { RIICHS_EV_EN_SLV_TRANSFER, NULL }, /* Enable Slave Transfer */
        { RIICHS_EV_GEN_START_COND, NULL }, /* Start Condition Generation */
        { RIICHS_EV_INT_START, NULL }, /* Start Condition Generation Interrupt */
        { RIICHS_EV_INT_ADD, riichs_after_send_slvadr }, /* Address Sending Interrupt */
        { RIICHS_EV_INT_SEND, NULL }, /* Data Sending Interrupt */
        { RIICHS_EV_INT_RECEIVE, NULL }, /* Data Receiving Interrupt */
        { RIICHS_EV_INT_STOP, NULL }, /* Stop Condition Generation Interrupt */
        { RIICHS_EV_INT_AL, riichs_arbitration_lost }, /* Arbitration-Lost Interrupt */
        { RIICHS_EV_INT_NACK, riichs_nack }, /* No Acknowledge Interrupt */
        { RIICHS_EV_INT_TMO, riichs_time_out }, /* Time out Interrupt */
    },

    /* Slave address [Read] transmission completion wait state */
    {
        { RIICHS_EV_INIT, NULL }, /* IIC Driver Initialization */
        { RIICHS_EV_EN_SLV_TRANSFER, NULL }, /* Enable Slave Transfer */
        { RIICHS_EV_GEN_START_COND, NULL }, /* Start Condition Generation */
        { RIICHS_EV_INT_START, NULL }, /* Start Condition Generation Interrupt */
        { RIICHS_EV_INT_ADD, NULL }, /* Address Sending Interrupt */
        { RIICHS_EV_INT_SEND, NULL }, /* Data Sending Interrupt */
        { RIICHS_EV_INT_RECEIVE, riichs_after_send_slvadr }, /* Data Receiving Interrupt */
        { RIICHS_EV_INT_STOP, NULL }, /* Stop Condition Generation Interrupt  */
        { RIICHS_EV_INT_AL, riichs_arbitration_lost }, /* Arbitration-Lost Interrupt */
        { RIICHS_EV_INT_NACK, riichs_nack }, /* No Acknowledge Interrupt */
        { RIICHS_EV_INT_TMO, riichs_time_out }, /* Time out Interrupt */
    },

    /* Data transmission completion wait state */
    {
        { RIICHS_EV_INIT, NULL }, /* IIC Driver Initialization */
        { RIICHS_EV_EN_SLV_TRANSFER, NULL }, /* Enable Slave Transfer */
        { RIICHS_EV_GEN_START_COND, NULL }, /* Start Condition Generation */
        { RIICHS_EV_INT_START, NULL }, /* Start Condition Generation Interrupt */
        { RIICHS_EV_INT_ADD, NULL }, /* Address Sending Interrupt */
        { RIICHS_EV_INT_SEND, riichs_write_data_sending }, /* Data Sending Interrupt */
        { RIICHS_EV_INT_RECEIVE, NULL }, /* Data Receiving Interrupt */
        { RIICHS_EV_INT_STOP, NULL }, /* Stop Condition Generation Interrupt */
        { RIICHS_EV_INT_AL, riichs_arbitration_lost }, /* Arbitration-Lost Interrupt */
        { RIICHS_EV_INT_NACK, riichs_nack }, /* No Acknowledge Interrupt */
        { RIICHS_EV_INT_TMO, riichs_time_out }, /* Time out Interrupt */
    },

    /* Data reception completion wait state */
    {
        { RIICHS_EV_INIT, NULL }, /* IIC Driver Initialization */
        { RIICHS_EV_EN_SLV_TRANSFER, NULL }, /* Enable Slave Transfer */
        { RIICHS_EV_GEN_START_COND, NULL }, /* Start Condition Generation */
        { RIICHS_EV_INT_START, NULL }, /* Start Condition Generation Interrupt */
        { RIICHS_EV_INT_ADD, NULL }, /* Address Sending Interrupt */
        { RIICHS_EV_INT_SEND, NULL }, /* Data Sending Interrupt */
        { RIICHS_EV_INT_RECEIVE, riichs_read_data_receiving }, /* Data Receiving Interrupt */
        { RIICHS_EV_INT_STOP, NULL }, /* Stop Condition Generation Interrupt */
        { RIICHS_EV_INT_AL, riichs_arbitration_lost }, /* Arbitration-Lost Interrupt */
        { RIICHS_EV_INT_NACK, riichs_nack }, /* No Acknowledge Interrupt */
        { RIICHS_EV_INT_TMO, riichs_time_out }, /* Time out Interrupt */
    },

    /* Stop condition generation completion wait state */
    {
        { RIICHS_EV_INIT, NULL }, /* IIC Driver Initialization */
        { RIICHS_EV_EN_SLV_TRANSFER, NULL }, /* Enable Slave Transfer */
        { RIICHS_EV_GEN_START_COND, NULL }, /* Start Condition Generation */
        { RIICHS_EV_INT_START, NULL }, /* Start Condition Generation Interrupt */
        { RIICHS_EV_INT_ADD, NULL }, /* Address Sending Interrupt */
        { RIICHS_EV_INT_SEND, NULL }, /* Data Sending Interrupt */
        { RIICHS_EV_INT_RECEIVE, NULL }, /* Data Receiving Interrupt */
        { RIICHS_EV_INT_STOP, riichs_after_dtct_stop_cond }, /* Stop Condition Generation Interrupt */
        { RIICHS_EV_INT_AL, NULL }, /* Arbitration-Lost Interrupt */
        { RIICHS_EV_INT_NACK, riichs_nack }, /* No Acknowledge Interrupt */
        { RIICHS_EV_INT_TMO, riichs_time_out }, /* Time out Interrupt */
    },

};

/**********************************************************************************************************************
 * Function Name: R_RIICHS_Open
 *****************************************************************************************************************/ /**
 * @brief This function initializes the RIICHS FIT module. This function must be called before calling any other API 
 *        functions.
 * @param[in,out] *p_riichs_info
 *             This is the pointer to the I2C communication information structure.
 * @retval    RIICHS_SUCCESS             Processing completed successfully
 * @retval    RIICHS_ERR_LOCK_FUNC       The API is locked by the other task
 * @retval    RIICHS_ERR_INVALID_CHAN    Nonexistent channel
 * @retval    RIICHS_ERR_INVALID_ARG     Invalid parameter
 * @retval    RIICHS_ERR_OTHER           The event occurred is invalid in the current state
 * @details   Performs the initialization to start the RIICHS communication. Sets the RIICHS channel specified by the 
 *            parameter. If the state of the channel is 'uninitialized (RIICHS_NO_INIT)', the following processes are 
 *            performed. \n
 *            \li Setting the state flag.
 *            \li Setting I/O ports.
 *            \li Allocating I2C output ports.
 *            \li Canceling RIICHS module-stop state.
 *            \li Initializing variables used by the API.
 *            \li Initializing the RIICHS registers used for the RIICHS communication.
 *            \li Disabling the RIICHS interrupts.
 * @note      None
 */
riichs_return_t R_RIICHS_Open (riichs_info_t * p_riichs_info)
{
    bool chk = RIICHS_FALSE;
    riichs_return_t ret;
    riichs_addr_format_t temp_addr_fortmat0 = p_riichs_info->slave_addr0_format;
    riichs_addr_format_t temp_addr_fortmat1 = p_riichs_info->slave_addr1_format;
    riichs_addr_format_t temp_addr_fortmat2 = p_riichs_info->slave_addr2_format;
    uint16_t temp_addr0 = p_riichs_info->slave_addr0;
    uint16_t temp_addr1 = p_riichs_info->slave_addr1;
    uint16_t temp_addr2 = p_riichs_info->slave_addr2;
    riichs_priority_t temp_rxi_priority = p_riichs_info->rxi_priority;
    riichs_priority_t temp_txi_priority = p_riichs_info->txi_priority;
    riichs_priority_t temp_eei_priority = p_riichs_info->eei_priority;
    riichs_priority_t temp_tei_priority = p_riichs_info->tei_priority;

    /* ---- CHECK ARGUMENTS ---- */
#if (1U == RIICHS_CFG_PARAM_CHECKING_ENABLE)
    if (false == riichs_param_check(p_riichs_info))
    {
        return RIICHS_ERR_INVALID_ARG;
    }

    /* ---- CHECK CHANNEL ---- */
    if (false == riichs_mcu_check_channel(p_riichs_info->ch_no))
    {
        return RIICHS_ERR_INVALID_CHAN;
    }

    /* ---- CHECK High-speed ---- */
    if ((0U >= p_riichs_info->speed_kbps) || (3400U < p_riichs_info->speed_kbps))
    {
        return RIICHS_ERR_INVALID_ARG;
    }

    /* ---- CHECK bps(kbps) before transition to Hs mode ---- */
    if (1000U < p_riichs_info->speed_kbps)
    {
        if ((0U >= p_riichs_info->fs_speed_kbps) || (1000U < p_riichs_info->fs_speed_kbps))
        {
            return RIICHS_ERR_INVALID_ARG;
        }
    }

    /* ---- CHECK Noise Filter Stage ---- */
    if (16U < p_riichs_info->filter_stage)
    {
        return RIICHS_ERR_INVALID_ARG;
    }

    /* ---- CHECK interrupt priority level ---- */
    if ((1U > temp_txi_priority) || (15U < temp_txi_priority)
        || (1U > temp_rxi_priority) || (15U < temp_rxi_priority)
        || (1U > temp_tei_priority) || (15U < temp_tei_priority)
        || (1U > temp_eei_priority) || (15U < temp_eei_priority)
        || (temp_txi_priority > temp_tei_priority)
        || (temp_rxi_priority > temp_tei_priority)
        || (temp_txi_priority > temp_eei_priority)
        || (temp_rxi_priority > temp_eei_priority))
    {
        return RIICHS_ERR_INVALID_ARG;
    }

    /* ---- CHECK bus free time counter ---- */
    if (0x000001FFU < p_riichs_info->bus_free_time)
    {
        return RIICHS_ERR_INVALID_ARG;
    }

    /* ---- CHECK slave address 0 ---- */
    if (false == riichs_slave_address_check(temp_addr_fortmat0, temp_addr0))
    {
        return RIICHS_ERR_INVALID_ARG;
    }

    /* ---- CHECK slave address 1 ---- */
    if (false == riichs_slave_address_check(temp_addr_fortmat1, temp_addr1))
    {
        return RIICHS_ERR_INVALID_ARG;
    }

    /* ---- CHECK slave address 2 ---- */
    if (false == riichs_slave_address_check(temp_addr_fortmat2, temp_addr2))
    {
        return RIICHS_ERR_INVALID_ARG;
    }
#endif

    /*  ---- RIICHS HARDWARE LOCK ---- */
    chk = riichs_mcu_hardware_lock(p_riichs_info->ch_no);
    if (RIICHS_FALSE == chk)
    {
        /* Lock has already been acquired by another task. Needs to try again later. */
        return RIICHS_ERR_LOCK_FUNC;
    }

    /* ---- GET INITIAL SETTING ---- */
    riichs_get_initial_setting(p_riichs_info);

    /* ---- INITIALIZE RIICHS INTERNAL STATUS INFORMATION ---- */
    g_riichs_ChStatus[p_riichs_info->ch_no] = RIICHS_NO_INIT;
    p_riichs_info->dev_sts = RIICHS_NO_INIT;

    /* ---- INITIALIZE CHANNEL ---- */
    /* Calls the API function. */
    ret = riichs_open(p_riichs_info);

    return ret;
} /* End of function R_RIICHS_Open() */

/***********************************************************************************************************************
 * Function Name: riichs_get_initial_setting
 * Description  : Get initial setting.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : none
 **********************************************************************************************************************/
static void riichs_get_initial_setting (riichs_info_t * p_riichs_info)
{
    riichs_priority_t temp_rxi_priority = p_riichs_info->rxi_priority;
    riichs_priority_t temp_txi_priority = p_riichs_info->txi_priority;
    riichs_priority_t temp_eei_priority = p_riichs_info->eei_priority;
    riichs_priority_t temp_tei_priority = p_riichs_info->tei_priority;
    riichs_timeout_t temp_timeout_enable = p_riichs_info->timeout_enable;
    uint32_t temp_speed_kbps = p_riichs_info->speed_kbps;

    /* external global variable */
    g_riichs_rxi_int_priority[p_riichs_info->ch_no] = temp_rxi_priority;
    g_riichs_txi_int_priority[p_riichs_info->ch_no] = temp_txi_priority;
    g_riichs_eei_int_priority[p_riichs_info->ch_no] = temp_eei_priority;
    g_riichs_tei_int_priority[p_riichs_info->ch_no] = temp_tei_priority;
    g_riichs_timeout_enable[p_riichs_info->ch_no] = temp_timeout_enable;

    /* internal global variable */
    g_riichs_bps[p_riichs_info->ch_no] = temp_speed_kbps;

    /* Function Enable Register (ICFER) initial set */
    g_riichs_icfer_init[p_riichs_info->ch_no] = RIICHS_NALE | RIICHS_SALE | RIICHS_SCLE;
    if (RIICHS_MASTER_ARB_LOST_DISABLE == p_riichs_info->master_arb)
    {
        g_riichs_icfer_init[p_riichs_info->ch_no] &= RIICHS_ICFER_MALE_CLR;
    }
    else
    {
        g_riichs_icfer_init[p_riichs_info->ch_no] |= RIICHS_ICFER_MALE_SET;
    }

    if (1000U < g_riichs_bps[p_riichs_info->ch_no])
    {
        g_riichs_icfer_init[p_riichs_info->ch_no] |= RIICHS_ICFER_FMPE_SET;
        g_riichs_icfer_init[p_riichs_info->ch_no] |= RIICHS_ICFER_HSME_SET;
    }
    else if (400U < g_riichs_bps[p_riichs_info->ch_no])
    {
        g_riichs_icfer_init[p_riichs_info->ch_no] |= RIICHS_ICFER_FMPE_SET;
    }
    else
    {
        g_riichs_icfer_init[p_riichs_info->ch_no] &= RIICHS_ICFER_FMPE_CLR;
        g_riichs_icfer_init[p_riichs_info->ch_no] &= RIICHS_ICFER_HSME_CLR;
    }

    if (1000U < g_riichs_bps[p_riichs_info->ch_no])
    {
        /* Set Noise Filter Stage in Hs-mode */
        switch (p_riichs_info->filter_stage)
        {
            case RIICHS_DIGITAL_FILTER_0:
                g_riichs_icicr_init[p_riichs_info->ch_no] &= RIICHS_ICICR_NF_CLR;
            break;
            case RIICHS_DIGITAL_FILTER_1:
                g_riichs_icicr_init[p_riichs_info->ch_no] = 0x000000D0;
            break;
            case RIICHS_DIGITAL_FILTER_2:
                g_riichs_icicr_init[p_riichs_info->ch_no] = 0x000000D4;
            break;
            case RIICHS_DIGITAL_FILTER_3:
                g_riichs_icicr_init[p_riichs_info->ch_no] = 0x000000D8;
            break;
            case RIICHS_DIGITAL_FILTER_4:
                g_riichs_icicr_init[p_riichs_info->ch_no] = 0x000000DC;
            break;
            default:
            break;
        }
    }
    else
    {
        /* Set Noise Filter Stage except in Hs-mode */
        switch (p_riichs_info->filter_stage)
        {
            case RIICHS_DIGITAL_FILTER_0:
                g_riichs_icicr_init[p_riichs_info->ch_no] &= RIICHS_ICICR_NF_CLR;
            break;
            case RIICHS_DIGITAL_FILTER_1:
                g_riichs_icicr_init[p_riichs_info->ch_no] = 0x000000D0;
            break;
            case RIICHS_DIGITAL_FILTER_2:
                g_riichs_icicr_init[p_riichs_info->ch_no] = 0x000000D1;
            break;
            case RIICHS_DIGITAL_FILTER_3:
                g_riichs_icicr_init[p_riichs_info->ch_no] = 0x000000D2;
            break;
            case RIICHS_DIGITAL_FILTER_4:
                g_riichs_icicr_init[p_riichs_info->ch_no] = 0x000000D3;
            break;
            case RIICHS_DIGITAL_FILTER_5:
                g_riichs_icicr_init[p_riichs_info->ch_no] = 0x000000D4;
            break;
            case RIICHS_DIGITAL_FILTER_6:
                g_riichs_icicr_init[p_riichs_info->ch_no] = 0x000000D5;
            break;
            case RIICHS_DIGITAL_FILTER_7:
                g_riichs_icicr_init[p_riichs_info->ch_no] = 0x000000D6;
            break;
            case RIICHS_DIGITAL_FILTER_8:
                g_riichs_icicr_init[p_riichs_info->ch_no] = 0x000000D7;
            break;
            case RIICHS_DIGITAL_FILTER_9:
                g_riichs_icicr_init[p_riichs_info->ch_no] = 0x000000D8;
            break;
            case RIICHS_DIGITAL_FILTER_10:
                g_riichs_icicr_init[p_riichs_info->ch_no] = 0x000000D9;
            break;
            case RIICHS_DIGITAL_FILTER_11:
                g_riichs_icicr_init[p_riichs_info->ch_no] = 0x000000DA;
            break;
            case RIICHS_DIGITAL_FILTER_12:
                g_riichs_icicr_init[p_riichs_info->ch_no] = 0x000000DB;
            break;
            case RIICHS_DIGITAL_FILTER_13:
                g_riichs_icicr_init[p_riichs_info->ch_no] = 0x000000DC;
            break;
            case RIICHS_DIGITAL_FILTER_14:
                g_riichs_icicr_init[p_riichs_info->ch_no] = 0x000000DD;
            break;
            case RIICHS_DIGITAL_FILTER_15:
                g_riichs_icicr_init[p_riichs_info->ch_no] = 0x000000DE;
            break;
            case RIICHS_DIGITAL_FILTER_16:
                g_riichs_icicr_init[p_riichs_info->ch_no] = 0x000000DF;
            break;
            default:
            break;
        }
    }

    /* Status Detection Enable Register (ICSER) initial set */
    g_riichs_icser_init[p_riichs_info->ch_no] = (RIICHS_ICSER_STDE | RIICHS_ICSER_SPDE | RIICHS_ICSER_TEDE);
    if (RIICHS_TMO_DISABLE == p_riichs_info->timeout_enable)
    {
        g_riichs_icser_init[p_riichs_info->ch_no] &= RIICHS_ICSER_TMOE_CLR;
    }
    else
    {
        g_riichs_icser_init[p_riichs_info->ch_no] |= RIICHS_ICSER_TMOE_SET;
    }

    if (RIICHS_NACK_DETC_DISABLE == p_riichs_info->nack_detc_enable)
    {
        g_riichs_icser_init[p_riichs_info->ch_no] &= RIICHS_ICSER_NAKDE_CLR;
    }
    else
    {
        g_riichs_icser_init[p_riichs_info->ch_no] |= RIICHS_ICSER_NAKDE_SET;
    }

    if (RIICHS_ARB_LOST_DISABLE == p_riichs_info->arb_lost_enable)
    {
        g_riichs_icser_init[p_riichs_info->ch_no] &= RIICHS_ICSER_ALE_CLR;
    }
    else
    {
        g_riichs_icser_init[p_riichs_info->ch_no] |= RIICHS_ICSER_ALE_SET;
    }

    /* Timeout Control Register (ICTOR) initial set */
    if (RIICHS_COUNTER_BIT16 == p_riichs_info->counter_bit)
    {
        g_riichs_ictor_init[p_riichs_info->ch_no] |= 0x00000000;
    }
    else if (RIICHS_COUNTER_BIT14 == p_riichs_info->counter_bit)
    {
        g_riichs_ictor_init[p_riichs_info->ch_no] |= 0x00000001;
    }
    else if (RIICHS_COUNTER_BIT8 == p_riichs_info->counter_bit)
    {
        g_riichs_ictor_init[p_riichs_info->ch_no] |= 0x00000002;
    }
    else if (RIICHS_COUNTER_BIT6 == p_riichs_info->counter_bit)
    {
        g_riichs_ictor_init[p_riichs_info->ch_no] |= 0x00000003;
    }
    else
    {

    }

    if (RIICHS_L_COUNT_DISABLE == p_riichs_info->l_count)
    {
        g_riichs_ictor_init[p_riichs_info->ch_no] &= RIICHS_ICTOR_TMOL_CLR;
    }
    else
    {
        g_riichs_ictor_init[p_riichs_info->ch_no] |= RIICHS_ICTOR_TMOL_SET;
    }


    if (RIICHS_H_COUNT_DISABLE == p_riichs_info->h_count)
    {
        g_riichs_ictor_init[p_riichs_info->ch_no] &= RIICHS_ICTOR_TMOH_CLR;
    }
    else
    {
        g_riichs_ictor_init[p_riichs_info->ch_no] |= RIICHS_ICTOR_TMOH_SET;
    }

    if (RIICHS_TIMEOUT_MODE_ALL == p_riichs_info->timeout_mode)
    {
        g_riichs_ictor_init[p_riichs_info->ch_no] |= 0x00000000;
    }
    else if (RIICHS_TIMEOUT_MODE_BUSY == p_riichs_info->timeout_mode)
    {
        g_riichs_ictor_init[p_riichs_info->ch_no] |= 0x00000040;
    }
    else
    {
        g_riichs_ictor_init[p_riichs_info->ch_no] |= 0x00000080;
    }

} /* End of function riichs_get_initial_setting() */

/***********************************************************************************************************************
 * Function Name: riichs_open
 * Description  : sub function of R_RIICHS_Open(). Initializes the I/O register for RIICHS control.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Successful operation
 *              : RIICHS_ERR_OTHER                 ; Other error
 **********************************************************************************************************************/
static riichs_return_t riichs_open (riichs_info_t * p_riichs_info)
{
    riichs_return_t ret;

    /* Updates the channel status. */
    riichs_set_ch_status(p_riichs_info, RIICHS_IDLE);

    /* Initialize API status on RAM */
    riichs_api_mode_event_init(p_riichs_info, RIICHS_MODE_NONE);

    /* Sets the internal status. (Internal information initialization) */
    riichs_api_status_set(p_riichs_info, RIICHS_STS_NO_INIT);

    /* Enables the IIC peripheral registers. */
    /* Includes I/O register read operation at the end of the following function. */
    /* RIICHS HARDWARE ENABLE */
    riichs_mcu_power_on(p_riichs_info->ch_no);

    /* Initializes the IIC driver. */
    ret = riichs_func_table(RIICHS_EV_INIT, p_riichs_info);

    if (RIICHS_SUCCESS != ret)
    {
        ret = RIICHS_ERR_OTHER;

        /* Updates the channel status. */
        riichs_set_ch_status(p_riichs_info, RIICHS_NO_INIT);
    }
    else
    {
        /* Registers a callback function for supported interrupts. */
        riichs_mcu_int_init(p_riichs_info->ch_no);

#if (1U == RIICHS_CFG_PORT_SET_PROCESSING)
        /* Init Port Processing */
        /* Ports open setting */
        /* Includes I/O register read operation at the end of the following function. */
        riichs_mcu_io_open(p_riichs_info->ch_no);

        /* Enables RIICHS multi-function pin controller. */
        /* Includes I/O register read operation at the end of the following function. */
        riichs_mcu_mpc_enable(p_riichs_info->ch_no);
#endif
    }

    return ret;
} /* End of function riichs_open() */

/**********************************************************************************************************************
 * Function Name: R_RIICHS_MasterSend
 *****************************************************************************************************************/ /**
 * @brief Starts master transmission. Changes the transmit pattern according to the parameters. Operates batched 
 *        processing until stop condition generation.
 * @param[in] *p_riichs_info
 *             This is the pointer to the I2C communication information structure. The transmit patterns can be 
 *             selected from four patterns by the parameter setting.
 * @retval    RIICHS_SUCCESS             Processing completed successfully
 * @retval    RIICHS_ERR_INVALID_CHAN    The channel is nonexistent
 * @retval    RIICHS_ERR_INVALID_ARG     The parameter is invalid
 * @retval    RIICHS_ERR_NO_INIT         Uninitialized state
 * @retval    RIICHS_ERR_BUS_BUSY        The bus state is busy
 * @retval    RIICHS_ERR_AL              Arbitration-lost error occurred
 * @retval    RIICHS_ERR_TMO             Timeout is detected
 * @retval    RIICHS_ERR_OTHER           The event occurred is invalid in the current state
 * @details   Starts the RIICHS master transmission. The transmission is performed with the RIICHS channel and transmit 
 *            pattern specified by parameters. If the state of the channel is 'idle (RIICHS_IDLE, RIICHS_FINISH,  or 
 *            RIICHS_NACK)', the following processes are performed. \n
 *            \li Setting the state flag.
 *            \li Initializing variables used by the API.
 *            \li Enabling the RIICHS interrupts.
 *            \li Generating a start condition.
 * 
 *            This function returns RIICHS_SUCCESS as a return value when the processing up to the start condition 
 *            generation ends normally. This function returns RIICHS_ERR_BUS_BUSY as a return value when the following 
 *            conditions are met to the start condition generation ends normally. (Note.1) \n
 *            \li The internal status bit is in busy state.
 *            \li Either SCL or SDA line is in low state. 
 * 
 *            The transmission processing is performed sequentially in subsequent interrupt processing after this 
 *            function return RIICHS_SUCCESS. Section "2.4 Usage of Interrupt Vector" in the application note should be 
 *            referred for the interrupt to be used. For master transmission, the interrupt generation timing should be 
 *            referred from "6.2.1 Master transmission" in the application note. \n
 *            After issuing a stop condition at the end of transmission, the callback function specified by the 
 *            argument is called. \n
 *            The transmission completion is performed normally or not, can be confirmed by checking the device status 
 *            flag specified by the argument or the channel status flag g_riichs_ChStatus [], that is to be
 *            "RIICHS_FINISH" for normal completion.\n
 *            Notes:\n
 *            1. When SCL and SDA pin is not external pull-up, this function may return RIICHS_ERR_BUS_BUSY by
 *               detecting either SCL or SDA line is as in low state.
 * @note      Available settings for each pattern see Section 3.2 in the application note for details.
 */
riichs_return_t R_RIICHS_MasterSend (riichs_info_t * p_riichs_info)
{
    riichs_return_t ret;

    /* ---- CHECK ARGUMENTS ---- */
#if (1U == RIICHS_CFG_PARAM_CHECKING_ENABLE)
    if ((((false == riichs_param_check(p_riichs_info)) || ((0 == p_riichs_info->cnt1st) && ((uint8_t *) FIT_NO_PTR != p_riichs_info->p_data1st)))
            || ((0 == p_riichs_info->cnt2nd) && ((uint8_t *) FIT_NO_PTR != p_riichs_info->p_data2nd)))
            || (NULL == p_riichs_info->callbackfunc)
            || (NULL == p_riichs_info->p_slv_adr))
    {
        return RIICHS_ERR_INVALID_ARG;
    }

    /* ---- CHECK CHANNEL ---- */
    if (false == riichs_mcu_check_channel(p_riichs_info->ch_no))
    {
        return RIICHS_ERR_INVALID_CHAN;
    }
#endif

    /* Calls the API function. */
    ret = riichs_master_send(p_riichs_info);

    return ret;
} /* End of function R_RIICHS_MasterSend() */

/***********************************************************************************************************************
 * Function Name: riichs_master_send
 * Description  : sub function of R_RIICHS_MasterSend().
 *              : Generates the start condition. Starts the master transmission.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Successful operation
 *              : RIICHS_ERR_NO_INIT               ; Uninitialized state
 *              : RIICHS_ERR_BUS_BUSY              ; Bus busy
 *              : RIICHS_ERR_AL                    ; Arbitration lost error
 *              : RIICHS_ERR_TMO                   ; Time out error
 *              : RIICHS_ERR_OTHER                 ; Other error
 **********************************************************************************************************************/
static riichs_return_t riichs_master_send (riichs_info_t * p_riichs_info)
{
    riichs_return_t ret;
    riichs_callback callbackfunc = NULL;

    if (NULL == p_riichs_info->p_slv_adr)
    {
        return RIICHS_ERR_INVALID_ARG;
    }

    /* Checks the channel status. */
    ret = riichs_check_chstatus_start(p_riichs_info);

    if (RIICHS_SUCCESS != ret)
    {
        return ret;
    }

    /* Updates the channel status. */
    riichs_set_ch_status(p_riichs_info, RIICHS_COMMUNICATION);

    /* Initialize API status on RAM */
    riichs_api_mode_event_init(p_riichs_info, RIICHS_MODE_M_SEND);

    /* Copy p_riichs_info address */
    priichs_info_m[p_riichs_info->ch_no] = p_riichs_info;

    /* Sets the callback function. */
    callbackfunc = p_riichs_info->callbackfunc;
    g_riichs_callbackfunc_m[p_riichs_info->ch_no] = callbackfunc;

    /* Generates the start condition.  */
    ret = riichs_func_table(RIICHS_EV_GEN_START_COND, p_riichs_info);

    if (RIICHS_SUCCESS != ret)
    {
        /* Initialize API status on RAM */
        riichs_set_ch_status(p_riichs_info, RIICHS_IDLE);
        if (RIICHS_MODE_S_READY == riichs_api_info[p_riichs_info->ch_no].B_Mode)
        {
            riichs_api_mode_set(p_riichs_info, RIICHS_MODE_NONE);
            riichs_api_mode_set(p_riichs_info, RIICHS_MODE_S_READY);
        }
        else
        {
            riichs_api_mode_set(p_riichs_info, RIICHS_MODE_NONE);
        }
    }

    return ret;
} /* End of function riichs_master_send() */

/**********************************************************************************************************************
 * Function Name: R_RIICHS_MasterReceive
 *****************************************************************************************************************/ /**
 * @brief Starts master reception. Changes the receive pattern according to the parameters. Operates batched processing 
 *        until stop condition generation.
 * @param[in] *p_riichs_info
 *             This is the pointer to the I2C communication information structure. The receive pattern can be selected 
 *             from master reception and master transmit/receive by the parameter setting. 
 * @retval    RIICHS_SUCCESS             Processing completed successfully
 * @retval    RIICHS_ERR_INVALID_CHAN    The channel is nonexistent
 * @retval    RIICHS_ERR_INVALID_ARG     The parameter is invalid
 * @retval    RIICHS_ERR_NO_INIT         Uninitialized state
 * @retval    RIICHS_ERR_BUS_BUSY        The bus state is busy
 * @retval    RIICHS_ERR_AL              Arbitration-lost error occurred
 * @retval    RIICHS_ERR_TMO             Timeout is detected
 * @retval    RIICHS_ERR_OTHER           The event occurred is invalid in the current state
 * @details   Starts the RIICHS master reception. The reception is performed with the RIICHS channel and receive pattern
 *            specified by parameters. If the state of the channel is 'idle (RIICHS_IDLE, RIICHS_FINISH, or 
 *            RIICHS_NACK)', the following processes are performed. \n
 *            \li Setting the state flag.
 *            \li Initializing variables used by the API.
 *            \li Enabling the RIICHS interrupts.
 *            \li Generating a start condition.
 * 
 *            This function returns RIICHS_SUCCESS as a return value when the processing up to the start condition 
 *            generation ends normally. This function returns RIICHS_ERR_BUS_BUSY as a return value when the following 
 *            conditions are met to the start condition generation ends normally. (Note.1) \n
 *            \li The internal status bit is in busy state.
 *            \li Either SCL or SDA line is in low state.
 * 
 *            The reception processing is performed sequentially in subsequent interrupt processing after this 
 *            function return RIICHS_SUCCESS. Section "2.4 Usage of Interrupt Vector" in the application note should be 
 *            referred for the interrupt to be used. For master transmission, the interrupt generation timing should be 
 *            referred from "6.2.2 Master Reception" in the application note . \n
 *            After issuing a stop condition at the end of reception, the callback function specified by the argument 
 *            is called. \n
 *            The reception completion is performed normally or not, can be confirmed by checking the device status 
 *            flag specified by the argument or the channel status flag g_riichs_ChStatus [], that is to be
 *            "RIICHS_FINISH" for normal completion.\n
 *            Notes:\n
 *            1. When SCL and SDA pin is not external pull-up, this function may return RIICHS_ERR_BUS_BUSY by
 *               detecting either SCL or SDA line is as in low state.
 * @note      Available settings for each receive pattern see Section 3.3 in the application note for details.
 */
riichs_return_t R_RIICHS_MasterReceive (riichs_info_t * p_riichs_info)
{
    riichs_return_t ret;

    if (NULL == p_riichs_info->p_slv_adr)
    {
        return RIICHS_ERR_INVALID_ARG;
    }
    /* ---- CHECK ARGUMENTS ---- */
#if (1U == RIICHS_CFG_PARAM_CHECKING_ENABLE)
    /* Parameter check */
    if ((((((false == riichs_param_check(p_riichs_info)) || (NULL == p_riichs_info->p_slv_adr))
            || ((0 != p_riichs_info->cnt1st) && (NULL == p_riichs_info->p_data1st))) || (NULL == p_riichs_info->p_data2nd))
            || (0 == p_riichs_info->cnt2nd)) || (NULL == p_riichs_info->callbackfunc))
    {
        return RIICHS_ERR_INVALID_ARG;
    }

    /* ---- CHECK CHANNEL ---- */
    if (false == riichs_mcu_check_channel(p_riichs_info->ch_no))
    {
        return RIICHS_ERR_INVALID_CHAN;
    }

#endif

    if (0 == p_riichs_info->cnt1st)
    {
        /* Calls the API function. */
        ret = riichs_master_receive(p_riichs_info);
    }
    else
    {
        /* Calls the API function. */
        ret = riichs_master_send_receive(p_riichs_info);
    }

    return ret;
} /* End of function R_RIICHS_MasterReceive() */

/***********************************************************************************************************************
 * Outline      : 
 * Function Name: riichs_master_receive
 * Description  : sub function of R_RIICHS_MasterReceive(). 
 *              : Generates the start condition. Starts the master reception.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Successful operation
 *              : RIICHS_ERR_NO_INIT               ; Uninitialized state
 *              : RIICHS_ERR_BUS_BUSY              ; Bus busy
 *              : RIICHS_ERR_AL                    ; Arbitration lost error
 *              : RIICHS_ERR_TMO                   ; Time out error
 *              : RIICHS_ERR_OTHER                 ; Other error
 **********************************************************************************************************************/
static riichs_return_t riichs_master_receive (riichs_info_t * p_riichs_info)
{
    riichs_return_t ret;
    riichs_callback callbackfunc = NULL;

    /* Checks the channel status. */
    ret = riichs_check_chstatus_start(p_riichs_info);

    if (RIICHS_SUCCESS != ret)
    {
        return ret;
    }

    /* Updates the channel status. */
    riichs_set_ch_status(p_riichs_info, RIICHS_COMMUNICATION);

    /* Initialize API status on RAM */
    riichs_api_mode_event_init(p_riichs_info, RIICHS_MODE_M_RECEIVE);

    /* Copy p_riichs_info address */
    priichs_info_m[p_riichs_info->ch_no] = p_riichs_info;

    /* Sets the callback function. */
    callbackfunc = p_riichs_info->callbackfunc;
    g_riichs_callbackfunc_m[p_riichs_info->ch_no] = callbackfunc;

    /* Generates the start condition. */
    ret = riichs_func_table(RIICHS_EV_GEN_START_COND, p_riichs_info);

    if (RIICHS_SUCCESS != ret)
    {
        /* Initialize API status on RAM */
        riichs_set_ch_status(p_riichs_info, RIICHS_IDLE);
        if (RIICHS_MODE_S_READY == riichs_api_info[p_riichs_info->ch_no].B_Mode)
        {
            riichs_api_mode_set(p_riichs_info, RIICHS_MODE_NONE);
            riichs_api_mode_set(p_riichs_info, RIICHS_MODE_S_READY);
        }
        else
        {
            riichs_api_mode_set(p_riichs_info, RIICHS_MODE_NONE);
        }
    }

    return ret;
} /* End of function riichs_master_receive() */

/***********************************************************************************************************************
 * Outline      : 
 * Function Name: riichs_master_send_receive
 * Description  : sub function of R_RIICHS_MasterReceive().
 *              : Generates the start condition. Starts the master reception.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Successful operation
 *              : RIICHS_ERR_NO_INIT               ; Uninitialized state
 *              : RIICHS_ERR_BUS_BUSY              ; Bus busy
 *              : RIICHS_ERR_AL                    ; Arbitration lost error
 *              : RIICHS_ERR_TMO                   ; Time out error
 *              : RIICHS_ERR_OTHER                 ; Other error
 **********************************************************************************************************************/
static riichs_return_t riichs_master_send_receive (riichs_info_t * p_riichs_info)
{
    riichs_return_t ret;
    riichs_callback callbackfunc = NULL;

    /* Checks the channel status. */
    ret = riichs_check_chstatus_start(p_riichs_info);

    if (RIICHS_SUCCESS != ret)
    {
        return ret;
    }

    /* Updates the channel status. */
    riichs_set_ch_status(p_riichs_info, RIICHS_COMMUNICATION);

    /* Initialize API status on RAM */
    riichs_api_mode_event_init(p_riichs_info, RIICHS_MODE_M_SEND_RECEIVE);

    /* Copy p_riichs_info address */
    priichs_info_m[p_riichs_info->ch_no] = p_riichs_info;

    /* Sets the callback function. */
    callbackfunc = p_riichs_info->callbackfunc;
    g_riichs_callbackfunc_m[p_riichs_info->ch_no] = callbackfunc;

    /* Generates the start condition. */
    ret = riichs_func_table(RIICHS_EV_GEN_START_COND, p_riichs_info);

    if (RIICHS_SUCCESS != ret)
    {
        /* Initialize API status on RAM */
        riichs_set_ch_status(p_riichs_info, RIICHS_IDLE);
        if (RIICHS_MODE_S_READY == riichs_api_info[p_riichs_info->ch_no].B_Mode)
        {
            riichs_api_mode_set(p_riichs_info, RIICHS_MODE_NONE);
            riichs_api_mode_set(p_riichs_info, RIICHS_MODE_S_READY);
        }
        else
        {
            riichs_api_mode_set(p_riichs_info, RIICHS_MODE_NONE);
        }
    }

    return ret;
} /* End of function riichs_master_send_receive() */

/**********************************************************************************************************************
 * Function Name: R_RIICHS_SlaveTransfer
 *****************************************************************************************************************/ /**
 * @brief This function performs slave transmission and reception. Changes the transmit and receive pattern according 
 *        to the parameters.
 * @param[in] *p_riichs_info
 *             This is the pointer to the I2C communication information structure. The operation can be selected from 
 *             preparation for slave reception, slave transmission, or both of them by the parameter setting.
 * @retval    RIICHS_SUCCESS             Processing completed successfully
 * @retval    RIICHS_ERR_INVALID_CHAN    The channel is nonexistent
 * @retval    RIICHS_ERR_INVALID_ARG     The parameter is invalid
 * @retval    RIICHS_ERR_NO_INIT         Uninitialized state
 * @retval    RIICHS_ERR_BUS_BUSY        The bus state is busy
 * @retval    RIICHS_ERR_AL              Arbitration-lost error occurred
 * @retval    RIICHS_ERR_TMO             Timeout is detected
 * @retval    RIICHS_ERR_OTHER           The event occurred is invalid in the current state
 * @details   Prepares for the RIICHS slave transmission or slave reception. If this function is called while the master
 *            is communicating, an error occurs. Sets the RIICHS channel specified by the parameter. If the state of the
 *            channel is 'idle (RIICHS_IDLE, RIICHS_FINISH, or RIICHS_NACK)', the following processes are performed. \n
 *            \li Setting the state flag.
 *            \li Initializing variables used by the API.
 *            \li Initializing the RIICHS registers used for the RIICHS communication.
 *            \li Enabling the RIICHS interrupts.
 *            \li Setting the slave address and enabling the slave address match interrupt.
 *
 *            This function returns RIICHS_SUCCESS as a return value when the setting of slave address and permission of
 *            slave address match interrupt are completed normally. \n
 *            The processing of slave transmission or slave reception is performed sequentially in the subsequent 
 *            interrupt processing. \n
 *            Section "2.4 Usage of Interrupt Vector" in the application note should be referred for the interrupt to 
 *            be used. \n
 *            The interrupt generation timing of slave transmission should be referred from "6.2.4 Slave Transmission" 
 *            in the application note. The interrupt generation timing for slave reception should be referred from 
 *            "6.2.5 Slave reception" in the application note. \n
 *            After detecting the stop condition of slave transmission or slave reception termination, the callback 
 *            function specified by the argument is called. \n
 *            The successful completion of slave reception can be checked by confirming the device status flag or 
 *            channel status flag specified in the argument  g_riichs_ChStatus [], that is to be "RIICHS_FINISH". The
 *            successful completion of slave transmission can be checked by confirming the device status flag or 
 *            channel status flag specified in the argument g_riichs_ChStatus [], that is to be "RIICHS_FINISH" or
 *            "RIICHS_NACK". "RIICHS_NACK" is set when master device transmitted NACK for notify to the slave that last 
 *            data receive completed.
 * @note      Available settings for each receive pattern see Section 3.4 in the application note for details.
 */
riichs_return_t R_RIICHS_SlaveTransfer (riichs_info_t * p_riichs_info)
{
    riichs_return_t ret;

    /* ---- CHECK ARGUMENTS ---- */
#if (1U == RIICHS_CFG_PARAM_CHECKING_ENABLE)
    /* Parameter check */
    if (((false == riichs_param_check(p_riichs_info))
            || (((uint8_t *) FIT_NO_PTR == p_riichs_info->p_data1st) && ((uint8_t *) FIT_NO_PTR == p_riichs_info->p_data2nd)))
            || (NULL == p_riichs_info->callbackfunc))
    {
        return RIICHS_ERR_INVALID_ARG;
    }

    /* ---- CHECK CHANNEL ---- */
    if (false == riichs_mcu_check_channel(p_riichs_info->ch_no))
    {
        return RIICHS_ERR_INVALID_CHAN;
    }

#endif

    /* Calls the API function. */
    ret = riichs_slave_transfer(p_riichs_info);

    return ret;
} /* End of function R_RIICHS_SlaveTransfer() */

/***********************************************************************************************************************
 * Outline      : 
 * Function Name: riichs_slave_transfer
 * Description  : sub function of R_RIICHS_SlaveTransfer.
 *              : Generates the start condition. Starts the slave reception and the slave transmission.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Successful operation
 *              : RIICHS_ERR_NO_INIT               ; Uninitialized state
 *              : RIICHS_ERR_BUS_BUSY              ; Bus busy
 *              : RIICHS_ERR_AL                    ; Arbitration lost error
 *              : RIICHS_ERR_TMO                   ; Time out error
 *              : RIICHS_ERR_OTHER                 ; Other error
 **********************************************************************************************************************/
static riichs_return_t riichs_slave_transfer (riichs_info_t * p_riichs_info)
{
    riichs_return_t ret;
    riichs_callback callbackfunc = NULL;
    volatile uint32_t uctmp = 0x00000000;
    volatile uint32_t temp_picscr_reg = 0x00000000;
    volatile uint32_t * const picscr_reg = RIICHS_ICSCR_ADR;
    temp_picscr_reg = *picscr_reg;

    /* Checks the channel status. */
    ret = riichs_check_chstatus_start(p_riichs_info);

    if (RIICHS_SUCCESS != ret)
    {
        return ret;
    }

    if (RIICHS_MODE_S_READY != riichs_api_info[p_riichs_info->ch_no].N_Mode)
    {
        /* Updates the channel status. */
        riichs_set_ch_status(p_riichs_info, RIICHS_IDLE);

        /* Initialize API status on RAM */
        riichs_api_mode_event_init(p_riichs_info, RIICHS_MODE_S_READY);

        /* Generates the start condition. */
        ret = riichs_func_table(RIICHS_EV_EN_SLV_TRANSFER, p_riichs_info);
    }

    /* Copy p_riichs_info address */
    priichs_info_s[p_riichs_info->ch_no] = p_riichs_info;

    /* Sets the callback function. */
    callbackfunc = p_riichs_info->callbackfunc;
    g_riichs_callbackfunc_s[p_riichs_info->ch_no] = callbackfunc;

    if (1000 < g_riichs_bps[p_riichs_info->ch_no])
    {
        temp_picscr_reg |= RIICHS_ICSCR_HSMCE_SET;
        (*picscr_reg) = temp_picscr_reg;
        uctmp = *picscr_reg; /* Reads ICSCR. */

        RIICHS0.ICCSCR.BIT.WAITAE = 0U; /* Refer to the technical update. */
        RIICHS0.ICCSCR.BIT.WAITRE = 1U; /* Refer to the technical update. */
        /* WAIT_LOOP */
        while ((0U != RIICHS0.ICCSCR.BIT.WAITAE) || (1U != RIICHS0.ICCSCR.BIT.WAITRE))
        {
            /* Do Nothing */
        }
    }

    /* ---- Enables IIC bus interrupt enable register. ---- */
    if (((uint8_t *) FIT_NO_PTR != p_riichs_info->p_data1st) && ((uint8_t *) FIT_NO_PTR != p_riichs_info->p_data2nd))
    {
        /* Enables slave send and slave receive */
        riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_INIT, RIICHS_ICCSIER_TX_RX);
    }
    else if (((uint8_t *) FIT_NO_PTR != p_riichs_info->p_data1st) && ((uint8_t *) FIT_NO_PTR == p_riichs_info->p_data2nd))
    {
        /* Enable slave send */
        riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_INIT, RIICHS_ICCSIER_TX);
    }
    else if (((uint8_t *) FIT_NO_PTR == p_riichs_info->p_data1st) && ((uint8_t *) FIT_NO_PTR != p_riichs_info->p_data2nd))
    {
        /* Enable slave receive */
        riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_INIT, RIICHS_ICCSIER_RX);
    }
    else
    {
        /* Do Nothing */
    }

    return ret;
} /* End of function riichs_slave_transfer() */

/**********************************************************************************************************************
 * Function Name: R_RIICHS_GetStatus
 *****************************************************************************************************************/ /**
 * @brief Returns the state of this module.
 * @param[in] *p_riichs_info
 *             This is the pointer to the I2C communication information structure.
 * @param[in] *p_riichs_status
 *             This contains the variable to store the RIICHS state.
 * @retval    RIICHS_SUCCESS             Processing completed successfully
 * @retval    RIICHS_ERR_INVALID_CHAN    The channel is nonexistent
 * @retval    RIICHS_ERR_INVALID_ARG     The parameter is invalid
 * @details   Returns the state of this module. \n
 *            By reading the register, pin level, variable, or others, obtains the state of the RIICHS channel which 
 *            specified by the parameter, and returns the obtained state as 32-bit structure. \n
 *            When this function is called, the RIICHS arbitration-lost flag and NACK flag are cleared to 0. If the 
 *            device state is "RIICHS_ AL", the value is updated to "RIICHS_FINISH". \n
 * @note      The state flag allocation see Section 3.5 in the application note for details.
 */
riichs_return_t R_RIICHS_GetStatus (riichs_info_t *p_riichs_info, riichs_mcu_status_t *p_riichs_status)
{
    /* ---- CHECK ARGUMENTS ---- */
#if (1U == RIICHS_CFG_PARAM_CHECKING_ENABLE)

    if ((false == riichs_param_check(p_riichs_info)) || (NULL == p_riichs_status))
    {
        return RIICHS_ERR_INVALID_ARG;
    }

    /* ---- CHECK CHANNEL ---- */
    if (false == riichs_mcu_check_channel(p_riichs_info->ch_no))
    {
        return RIICHS_ERR_INVALID_CHAN;
    }

#endif

    /* Calls the API function. */
    riichs_getstatus(p_riichs_info, p_riichs_status);

    return RIICHS_SUCCESS;
} /* End of function R_RIICHS_GetStatus() */

/***********************************************************************************************************************
 * Function Name: riichs_getstatus
 * Description  : sub function of R_RIICHS_GetStatus.
 *                Returns the state of this module.
 *                Obtains the state of the RIICHS channel, which specified by the parameter, by reading the register,
 *                variable, or others, and returns the obtained state as 32-bit structure.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 *                riichs_mcu_status_t * p_riichs_status ; The address to store the I2C state flag.
 * Return Value : none
 **********************************************************************************************************************/
static void riichs_getstatus (riichs_info_t *p_riichs_info, riichs_mcu_status_t *p_riichs_status)
{
    volatile uint32_t uctmp = 0x00000000;
    volatile riichs_mcu_status_t sts_flag;
    volatile uint32_t temp_picsr2_reg = 0x00000000;

    volatile uint32_t * const picimr_reg = RIICHS_ICIMR_ADR;
    volatile uint32_t * const picbsr_reg = RIICHS_ICBSR_ADR;
    volatile uint32_t * const picmmr_reg = RIICHS_ICMMR_ADR;
    volatile uint32_t * const picssr_reg = RIICHS_ICSSR_ADR;
    volatile uint32_t * const picsr2_reg = RIICHS_ICSR2_ADR;
    volatile uint32_t * const piccsr_reg = RIICHS_ICCSR_ADR;
    temp_picsr2_reg = *picsr2_reg;

    sts_flag.LONG = 0x00000000;

    /* ---- Check Bus state ---- */
    if (RIICHS_ICBSR_BFREE_SET == ((*picbsr_reg) & RIICHS_ICBSR_BFREE))
    {
        sts_flag.BIT.BSY = 1U;
    }
    else
    {
        sts_flag.BIT.BSY = 0U;
    }

    /* ---- Check Mode ---- */
    if (RIICHS_ICMMR_TRS_SET == ((*picmmr_reg) & RIICHS_ICMMR_TRS))
    {
        /* Send */
        sts_flag.BIT.TRS = 1U;
    }
    else
    {
        /* Receive */
        sts_flag.BIT.TRS = 0U;
    }

    /* ---- Check Event detection ---- */
    if (RIICHS_ICSR2_NACKF_SET == ((*picsr2_reg) & RIICHS_ICSR2_NACKF))
    {
        /* NACK translation detected*/
        sts_flag.BIT.NACK = 1U;
    }
    else
    {
        /* Not detected */
        sts_flag.BIT.NACK = 0U;
    }

    /* ---- Check Send buffer status ---- */
    if (RIICHS_ICCSR_TDRE_SET == ((*piccsr_reg) & RIICHS_ICCSR_TDRE))
    {
        /* Send buffer empty */
        sts_flag.BIT.SBUF = 1U;
    }
    else
    {
        /* Send buffer not empty */
        sts_flag.BIT.SBUF = 0U;
    }

    /* ---- Check Receive buffer status ---- */
    if (RIICHS_ICCSR_RDRF_SET == ((*piccsr_reg) & RIICHS_ICCSR_RDRF))
    {
        /* Receive buffer not empty */
        sts_flag.BIT.RBUF = 1U;
    }
    else
    {
        /* Receive buffer empty */
        sts_flag.BIT.RBUF = 0U;
    }

    /* ---- Check SDA pin level ---- */
    if (RIICHS_ICIMR_SDAI_SET == ((*picimr_reg) & RIICHS_ICIMR_SDAI))
    {
        /* SDA pin is High level */
        sts_flag.BIT.SDAI = 1U;
    }
    else
    {
        /* SDA pin is Low level */
        sts_flag.BIT.SDAI = 0U;
    }

    /* ---- Check SCL pin level ---- */
    if (RIICHS_ICIMR_SCLI_SET == ((*picimr_reg) & RIICHS_ICIMR_SCLI))
    {
        /* SCL pin is High level */
        sts_flag.BIT.SCLI = 1U;
    }
    else
    {
        /* SCL pin is Low level */
        sts_flag.BIT.SCLI = 0U;
    }

    /* ---- Check SDA output control status ---- */
    if (RIICHS_ICIMR_SDAO_SET == ((*picimr_reg) & RIICHS_ICIMR_SDAO))
    {
        /* SDA pin is Open */
        sts_flag.BIT.SDAO = 1U;
    }
    else
    {
        /* SDA pin is Low hold */
        sts_flag.BIT.SDAO = 0U;
    }

    /* ---- Check SCL output control status ---- */
    if (RIICHS_ICIMR_SCLO_SET == ((*picimr_reg) & RIICHS_ICIMR_SCLO))
    {
        /* SCL pin is Open */
        sts_flag.BIT.SCLO = 1U;
    }
    else
    {
        /* SCL pin is Low hold */
        sts_flag.BIT.SCLO = 0U;
    }

    /* ---- Check Event detection ---- */
    if (RIICHS_ICSR2_START_SET == ((*picsr2_reg) & RIICHS_ICSR2_START))
    {
        /* Start condition detected*/
        sts_flag.BIT.ST = 1U;
    }
    else
    {
        /* Not detected */
        sts_flag.BIT.ST = 0U;
    }

    /* ---- Check Event detection ---- */
    if (RIICHS_ICSR2_STOP_SET == ((*picsr2_reg) & RIICHS_ICSR2_STOP))
    {
        /* Stop condition detected*/
        sts_flag.BIT.SP = 1U;
    }
    else
    {
        /* Not detected */
        sts_flag.BIT.SP = 0U;
    }

    /* ---- Check Event detection ---- */
    if (RIICHS_ICSR2_AL_SET == ((*picsr2_reg) & RIICHS_ICSR2_AL))
    {
        /* Arbitration lost detected*/
        sts_flag.BIT.AL = 1U;
    }
    else
    {
        /* Not detected */
        sts_flag.BIT.AL = 0U;
    }

    /* ---- Check Event detection ---- */
    if (RIICHS_ICSR2_TMOF_SET == ((*picsr2_reg) & RIICHS_ICSR2_TMOF))
    {
        /* Time out detected*/
        sts_flag.BIT.TMO = 1U;
    }
    else
    {
        /* Not detected */
        sts_flag.BIT.TMO = 0U;
    }

    /* ---- Check Master/Slave Mode ---- */
    if (RIICHS_ICMMR_MST_SET == ((*picmmr_reg) & RIICHS_ICMMR_MST))
    {
        /* Master Mode */
        sts_flag.BIT.MST = 1U;
    }
    else
    {
        /* Slave Mode */
        sts_flag.BIT.MST = 0U;
    }

    /* ---- Check Host Address detection ---- */
    if (RIICHS_ICSSR_HOA_SET == ((*picssr_reg) & RIICHS_ICSSR_HOA))
    {
        /* Host Address detected */
        sts_flag.BIT.HOA = 1U;
    }
    else
    {
        /* Not detected */
        sts_flag.BIT.HOA = 0U;
    }

    /* ---- Check Device ID Address detection ---- */
    if (RIICHS_ICSSR_DID_SET == ((*picssr_reg) & RIICHS_ICSSR_DID))
    {
        /* Device ID Address detected */
        sts_flag.BIT.DID = 1U;
    }
    else
    {
        /* Not detected */
        sts_flag.BIT.DID = 0U;
    }

    /* ---- Check Hs-mode Master Code detection ---- */
    if (RIICHS_ICSSR_HSMC_SET == ((*picssr_reg) & RIICHS_ICSSR_HSMC))
    {
        /* Hs-mode Master Code detected */
        sts_flag.BIT.HSMC = 1U;
    }
    else
    {
        /* Not detected */
        sts_flag.BIT.HSMC = 0U;
    }

    /* ---- Check General call Address detection ---- */
    if (RIICHS_ICSSR_GCA_SET == ((*picssr_reg) & RIICHS_ICSSR_GCA))
    {
        /* General call Address detected */
        sts_flag.BIT.GCA = 1U;
    }
    else
    {
        /* Not detected */
        sts_flag.BIT.GCA = 0U;
    }

    /* ---- Check Slave0 Address detection ---- */
    if (RIICHS_ICSSR_AAS0_SET == ((*picssr_reg) & RIICHS_ICSSR_AAS0))
    {
        /* Slave0 Address detected */
        sts_flag.BIT.AAS0 = 1U;
    }
    else
    {
        /* Not detected */
        sts_flag.BIT.AAS0 = 0U;
    }

    /* ---- Check Slave1 Address detection ---- */
    if (RIICHS_ICSSR_AAS1_SET == ((*picssr_reg) & RIICHS_ICSSR_AAS1))
    {
        /* Slave1 Address detected */
        sts_flag.BIT.AAS1 = 1U;
    }
    else
    {
        /* Not detected */
        sts_flag.BIT.AAS1 = 0U;
    }

    /* ---- Check Slave2 Address detection ---- */
    if (RIICHS_ICSSR_AAS2_SET == ((*picssr_reg) & RIICHS_ICSSR_AAS2))
    {
        /* Slave2 Address detected */
        sts_flag.BIT.AAS2 = 1U;
    }
    else
    {
        /* Not detected */
        sts_flag.BIT.AAS2 = 0U;
    }

    /* ---- Set the state ---- */
    *p_riichs_status = sts_flag;

    /* ---- clear flag ---- */
    temp_picsr2_reg &= RIICHS_ICSR2_AL_CLR;
    (*picsr2_reg) = temp_picsr2_reg;
    temp_picsr2_reg &= RIICHS_ICSR2_NACKF_CLR;
    (*picsr2_reg) = temp_picsr2_reg;
    uctmp = *picsr2_reg;

    if (RIICHS_AL == g_riichs_ChStatus[p_riichs_info->ch_no])
    {
        /* Initialize the channel status. */
        riichs_set_ch_status(p_riichs_info, RIICHS_FINISH);
    }
} /* End of function riichs_getstatus() */

/**********************************************************************************************************************
 * Function Name: R_RIICHS_Control
 *****************************************************************************************************************/ /**
 * @brief This function outputs conditions, Hi-Z from the SDA, and one-shot of the SCL clock. Also it resets the 
 *        settings of this module. This function is mainly used when a communication error occurs.
 * @param[in] *p_riichs_info
 *             This is the pointer to the I2C communication information structure.
 * @param[in] ctrl_ptn
 *             Specifies the output pattern. See Section 3.6 in the application note for details.
 * @retval    RIICHS_SUCCESS             Processing completed successfully
 * @retval    RIICHS_ERR_INVALID_CHAN    Nonexistent channel
 * @retval    RIICHS_ERR_INVALID_ARG     Invalid parameter
 * @retval    RIICHS_ERR_BUS_BUSY        Bus is busy
 * @retval    RIICHS_ERR_AL              Arbitration-lost error occurred
 * @retval    RIICHS_ERR_OTHER           The event occurred is invalid in the current state
 * @details   Outputs control signals of the RIICHS. Outputs conditions specified by the argument, Hi-Z from the SDA pin, 
 *            and one-shot of the SCL clock. Also resets the RIICHS module settings.
 * @note      One-shot output of the SCL clock. \n
 *            In master mode, if the clock signals from the master and slave devices go out of synchronization due to 
 *            noise or other factors, the slave device may hold the SDA line low (bus hang up). Then the SDA line can 
 *            be released from being held low by outputting one clock of the SCL at a time. \n
 *            In this module, one clock of the SCL can be output by setting the output pattern "RIICHS_GEN_SCL_ONESHOT" 
 *            (one-shot output of the SCL clock) and calling R_RIICHS_Control().
 */
riichs_return_t R_RIICHS_Control (riichs_info_t * p_riichs_info, uint8_t ctrl_ptn)
{
    riichs_return_t ret;

    /* ---- CHECK ARGUMENTS ---- */
#if (1U == RIICHS_CFG_PARAM_CHECKING_ENABLE)

    if ((false == riichs_param_check(p_riichs_info)) || (0 == ctrl_ptn))
    {
        return RIICHS_ERR_INVALID_ARG;
    }

    /* ---- CHECK CHANNEL ---- */
    if (false == riichs_mcu_check_channel(p_riichs_info->ch_no))
    {
        return RIICHS_ERR_INVALID_CHAN;
    }

#endif

    /* Calls the API function. */
    ret = riichs_control(p_riichs_info, ctrl_ptn);

    return ret;
} /* End of function R_RIICHS_Control() */

/***********************************************************************************************************************
 * Function Name: riichs_control
 * Description  : sub function of R_RIICHS_Control.
 *                This function is mainly used when a communication error occurs.
 *                Outputs control signals of the simple I2C mode.
 *                Outputs conditions specified by the argument, NACK,
 *                and one-shot of the SSCL clock. Also resets the simple I2C mode settings.
 * Arguments    : r_iic_info_t * p_riichs_info     ; IIC Information
 *                uint8_t cntl_ptn               ; Output Pattern
 *                 [Options]
 *                  RIICHS_GEN_START_CON
 *                  RIICHS_GEN_STOP_CON
 *                  RIICHS_GEN_RESTART_CON 
 *                  RIICHS_GEN_SDA_HI_Z
 *                  RIICHS_GEN_SCL_ONESHOT
 *                  RIICHS_GEN_RESET
 * Return Value : RIICHS_SUCCESS                   ; Successful operation
 *              : RIICHS_ERR_INVALID_ARG           ; Parameter error
 *              : RIICHS_ERR_BUS_BUSY              ; Bus busy
 *              : RIICHS_ERR_AL                    ; Arbitration lost error
 *              : RIICHS_ERR_OTHER                 ; Other error
 **********************************************************************************************************************/
static riichs_return_t riichs_control (riichs_info_t * p_riichs_info, uint8_t ctrl_ptn)
{
    volatile riichs_return_t ret = RIICHS_SUCCESS;
    volatile uint32_t uctmp = 0x00000000;
    volatile uint32_t cnt = 0x00000000;
    volatile uint32_t temp_picocr_reg = 0x00000000;
    volatile uint32_t temp_picsr2_reg = 0x00000000;

    /* Creates the register pointer for the specified RIICHS channel. */
    volatile uint32_t * const picsr2_reg = RIICHS_ICSR2_ADR;
    volatile uint32_t * const picocr_reg = RIICHS_ICOCR_ADR;
    volatile uint32_t * const picbsr_reg = RIICHS_ICBSR_ADR;
    volatile uint32_t * const piccr_reg = RIICHS_ICCR_ADR;
    temp_picocr_reg = *picocr_reg;
    temp_picsr2_reg = *picsr2_reg;

    /* Disable all RIICHS interrupt */
    riichs_int_disable(p_riichs_info);

    /* ==== Release reset(IICRST) of RIICHS ==== */
    riichs_reset_clear(p_riichs_info);

    if (RIICHS_ICCR_ICE_SET != ((*piccr_reg) & RIICHS_ICCR_ICE)) /* Check ICCR.ICE = 0 */
    {
        /* Set ICE bit to enable RIICHS module */
        (*piccr_reg) = RIICHS_ICCR_ICE_SET;
    }

    if ((0 != (uint8_t) (((RIICHS_GEN_START_CON | RIICHS_GEN_RESTART_CON) | RIICHS_GEN_STOP_CON) & ctrl_ptn))
            && (0 == (uint8_t) (((RIICHS_GEN_SDA_HI_Z | RIICHS_GEN_SCL_ONESHOT) | RIICHS_GEN_RESET) & ctrl_ptn)))
    {
        /* ==== Check request output pattern ==== */
        /* ---- Generate the start condition ---- */
        if (RIICHS_GEN_START_CON == (ctrl_ptn & RIICHS_GEN_START_CON))
        {
            /* Checks bus busy. */
            if (RIICHS_ICBSR_BFREE_SET == ((*picbsr_reg) & RIICHS_ICBSR_BFREE))
            {
                /* When BFREE bit is "0"(Bus busy) */
                ret = RIICHS_ERR_BUS_BUSY;
                return ret;
            }

            /* Generates the start condition. */
            riichs_start_cond_generate(p_riichs_info);

            /* Wait the request generation has been completed */
            cnt = p_riichs_info->bus_check_counter;
            /* WAIT_LOOP */
            while (RIICHS_ICSR2_START_SET != ((*picsr2_reg) & RIICHS_ICSR2_START))
            {
                /* Check Arbitration */
                if (RIICHS_ICSR2_AL_SET == ((*picsr2_reg) & RIICHS_ICSR2_AL))
                {
                    /* When AL bit is "1" */
                    ret = RIICHS_ERR_AL;
                    return ret;
                }

                /* Check Reply */
                if (0x00000000 < cnt)
                {
                    cnt--;
                }
                else
                {
                    /* When Non Reply */
                    ret = RIICHS_ERR_BUS_BUSY;
                    return ret;
                }
            }

            /* Clear START detect bit */
            temp_picsr2_reg &= RIICHS_ICSR2_START_CLR;
            (*picsr2_reg) = temp_picsr2_reg;
        }

        /* ---- Generate the restart condition ---- */
        if (RIICHS_GEN_RESTART_CON == (ctrl_ptn & RIICHS_GEN_RESTART_CON))
        {
            /* Checks bus busy. */
            if (RIICHS_ICBSR_BFREE_SET != ((*picbsr_reg) & RIICHS_ICBSR_BFREE))
            {
                /* When BFREE bit is "1"(Not Bus busy) */
                ret = RIICHS_ERR_OTHER;
                return ret;
            }

            /* Generates the restart condition. */
            riichs_re_start_cond_generate(p_riichs_info);

            /* Wait the request generation has been completed*/
            cnt = p_riichs_info->bus_check_counter;
            /* WAIT_LOOP */
            while (RIICHS_ICSR2_START_SET != ((*picsr2_reg) & RIICHS_ICSR2_START))
            {
                /* Check Arbitration */
                if (RIICHS_ICSR2_AL_SET == ((*picsr2_reg) & RIICHS_ICSR2_AL))
                {
                    /* When AL bit is "1" */
                    ret = RIICHS_ERR_AL;
                    return ret;
                }

                /* Check Reply */
                if (0x00000000 < cnt)
                {
                    cnt--;
                }
                else
                {
                    /* When Non Reply */
                    ret = RIICHS_ERR_BUS_BUSY;
                    return ret;
                }
            }

            /* Clear START detect bit */
            temp_picsr2_reg &= RIICHS_ICSR2_START_CLR;
            (*picsr2_reg) = temp_picsr2_reg;
        }

        /* ---- Generate the stop condition ---- */
        if (RIICHS_GEN_STOP_CON == (ctrl_ptn & RIICHS_GEN_STOP_CON))
        {
            /* Checks bus busy. */
            if (RIICHS_ICBSR_BFREE_SET != ((*picbsr_reg) & RIICHS_ICBSR_BFREE))
            {
                /* When BFREE bit is "1"(Not Bus busy) */
                ret = RIICHS_ERR_OTHER;
                return ret;
            }

            /* Generates the stop condition. */
            riichs_stop_cond_generate(p_riichs_info);

            /* Wait the request generation has been completed*/
            cnt = p_riichs_info->bus_check_counter;
            /* WAIT_LOOP */
            while (RIICHS_ICSR2_STOP_SET != ((*picsr2_reg) & RIICHS_ICSR2_STOP))
            {
                /* Check Arbitration */
                if (RIICHS_ICSR2_AL_SET == ((*picsr2_reg) & RIICHS_ICSR2_AL))
                {
                    /* When AL bit is "1" */
                    ret = RIICHS_ERR_AL;
                    return ret;
                }

                /* Check Reply */
                if (0x00000000 < cnt)
                {
                    cnt--;
                }
                else
                {
                    /* When Non Reply */
                    ret = RIICHS_ERR_BUS_BUSY;
                    return ret;
                }
            }

            /* Clear STOP detect bit */
            temp_picsr2_reg &= RIICHS_ICSR2_STOP_CLR;
            (*picsr2_reg) = temp_picsr2_reg;
        }
    }
    else if ((0x00 != (uint8_t) ((RIICHS_GEN_SDA_HI_Z | RIICHS_GEN_SCL_ONESHOT) & ctrl_ptn))
            && (0x00 == (uint8_t) ((((RIICHS_GEN_START_CON | RIICHS_GEN_RESTART_CON) | RIICHS_GEN_STOP_CON) |
            RIICHS_GEN_RESET) & ctrl_ptn)))
    {
        /* ---- Select SDA pin in a high-impedance state ---- */
        if (RIICHS_GEN_SDA_HI_Z == (ctrl_ptn & RIICHS_GEN_SDA_HI_Z))
        {
            /* Changes the SDA pin in a high-impedance state. */
            /* Sets for ICOCR.SDAO bit. */
            *picocr_reg = (uint32_t) ((*picocr_reg) | RIICHS_ICOCR_SDAO_SET | RIICHS_ICOCR_SOWP_SET);
            *picocr_reg = (uint32_t) ((*picocr_reg) & RIICHS_ICOCR_SOWP_CLR);
        }

        /* ---- Generate OneShot of SSCL clock ---- */
        if (RIICHS_GEN_SCL_ONESHOT == (ctrl_ptn & RIICHS_GEN_SCL_ONESHOT))
        {
            /* Output SCL oneshot */
            temp_picocr_reg |= RIICHS_ICOCR_CLO_SET;
            (*picocr_reg) = temp_picocr_reg;

            /* Wait output scl oneshot has been completed */
            cnt = p_riichs_info->bus_check_counter;
            /* WAIT_LOOP */
            while (RIICHS_ICOCR_CLO_SET == ((*picocr_reg) & RIICHS_ICOCR_CLO_SET))
            {
                /* Check Reply */
                if (0x00000000 < cnt)
                {
                    cnt--;
                }
                else
                {
                    /* Set SCLO bit is "1"(Hi-z). */
                    *picocr_reg = (uint32_t) ((*picocr_reg) | RIICHS_ICOCR_SCLO_SET | RIICHS_ICOCR_SOWP_SET);
                    *picocr_reg = (uint32_t) ((*picocr_reg) & RIICHS_ICOCR_SOWP_CLR);

                    /* When Non Reply */
                    ret = RIICHS_ERR_BUS_BUSY;
                    return ret;
                }
            }
        }
    }
    else if ((0x00 != (uint8_t) ((RIICHS_GEN_RESET) & ctrl_ptn))
            && (0x00 == (uint8_t) (((((RIICHS_GEN_START_CON | RIICHS_GEN_RESTART_CON) | RIICHS_GEN_STOP_CON) |
            RIICHS_GEN_SDA_HI_Z) | RIICHS_GEN_SCL_ONESHOT) & ctrl_ptn)))
    {
        /* ---- Generate Reset ---- */
        /* Updates the channel status. */
        riichs_set_ch_status(p_riichs_info, RIICHS_IDLE);

        /* Initialize API status on RAM */
        riichs_api_mode_event_init(p_riichs_info, RIICHS_MODE_NONE);

        /* Initializes the IIC registers. */
        /* Sets the internal status. */
        /* Enables the interrupt. */
        /* Clears the RIICHS reset or internal reset. */
        riichs_enable(p_riichs_info);
    }
    else
    {
        /* parameter error */
        ret = RIICHS_ERR_INVALID_ARG;
    }
    uctmp = *picocr_reg;

    riichs_clear_ir_flag(p_riichs_info); /* Clear IR flag for RXI and TXI */

    return ret;
} /* End of function riichs_control() */

/**********************************************************************************************************************
 * Function Name: R_RIICHS_Close
 *****************************************************************************************************************/ /**
 * @brief This function completes the RIICHS communication and releases the RIICHS used.
 * @param[in] *p_riichs_info
 *             This is the pointer to the I2C communication information structure.
 * @retval    RIICHS_SUCCESS           Processing completed successfully
 * @retval    RIICHS_ERR_INVALID_CHAN  The channel is nonexistent
 * @retval    RIICHS_ERR_INVALID_ARG   Invalid parameter
 * @details   Configures the settings to complete the RIICHS communication. Disables the RIICHS channel specified by the
 *            parameter. The following processes are performed in this function. \n
 *            \li Entering the RIICHS module-stop state.
 *            \li Releasing I2C output ports.
 *            \li Disabling the RIICHS interrupt.
 * 
 *            To restart the communication, call the R_RIICHS_Open() function (initialization function). If the 
 *            communication is forcibly terminated, that communication is not guaranteed. \n
 * @note      None
 */
riichs_return_t R_RIICHS_Close (riichs_info_t * p_riichs_info)
{
    /* ---- CHECK ARGUMENTS ---- */
#if (1U == RIICHS_CFG_PARAM_CHECKING_ENABLE)
    if (false == riichs_param_check(p_riichs_info))
    {
        return RIICHS_ERR_INVALID_ARG;
    }

    /* ---- CHECK CHANNEL ---- */
    if (false == riichs_mcu_check_channel(p_riichs_info->ch_no))
    {
        return RIICHS_ERR_INVALID_CHAN;
    }

#endif

    /* ---- Hardware Unlock ---- */
    riichs_mcu_hardware_unlock(p_riichs_info->ch_no);

    /* Calls the API function. */
    riichs_close(p_riichs_info);

    return RIICHS_SUCCESS;
} /* End of function R_RIICHS_Close() */

/***********************************************************************************************************************
 * Function Name: riichs_close
 * Description  : sub function of R_RIICHS_Close. 
 *                Resets the RIICHS driver.
 *              : The processing performs internal reset by setting to ICRCR.MRST bit.
 *              : Forcibly, stops the IIC communication.
 *              : After called the processing, channel status becomes "RIICHS_NO_INIT".
 *              : When starts the communication again, please call an initialization processing.
 * Arguments    : riichs_info_t * p_riichs_info      ;   IIC Information
 * Return Value : none
 **********************************************************************************************************************/
static void riichs_close (riichs_info_t * p_riichs_info)
{
    /* Updates the channel status. */
    riichs_set_ch_status(p_riichs_info, RIICHS_NO_INIT);

    /* Disables IIC. */
    riichs_disable(p_riichs_info);

#if (1U == RIICHS_CFG_PORT_SET_PROCESSING)
    /* Disables RIICHS multi-function pin controller after setting SCL and SDA to Hi-z by Reset. */
    /* Includes I/O register read operation at the end of the following function. */
    riichs_mcu_mpc_disable(p_riichs_info->ch_no);
#endif

    riichs_mcu_power_off(p_riichs_info->ch_no);
} /* End of function riichs_close() */

/**********************************************************************************************************************
 * Function Name: R_RIICHS_GetVersion
 *****************************************************************************************************************/ /**
 * @brief Returns the current version of this module.
 * @return    Version number
 * @details   This function will return the version of the currently installed RIICHS FIT module. The version number is 
 *            encoded where the top 2 bytes are the major version number and the bottom 2 bytes are the minor version 
 *            number. For example, Version 4.25 would be returned as 0x00040019.
 * @note      None
 */
uint32_t R_RIICHS_GetVersion (void)
{
    uint32_t const version = (RIICHS_VERSION_MAJOR << 16) | RIICHS_VERSION_MINOR;

    return version;
} /* End of function R_RIICHS_GetVersion() */

/***********************************************************************************************************************
 * Function Name: r_riichs_advance
 * Description  : Advances the IIC communication.
 *              : The return value shows the communication result. Refer to the return value.
 * Arguments    : riichs_info_t * p_riichs_info     ;   IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Successful operation
 *              : RIICHS_ERR_INVALID_ARG           ; Parameter error
 *              : RIICHS_ERR_BUS_BUSY              ; Bus busy
 *              : RIICHS_ERR_AL                    ; Arbitration lost error
 *              : RIICHS_ERR_TMO                   ; Time out error
 *              : RIICHS_ERR_OTHER                 ; Other error
 **********************************************************************************************************************/
static riichs_return_t r_riichs_advance (riichs_info_t * p_riichs_info)
{
    riichs_return_t ret;

    /* Parameter check */
    if (false == riichs_param_check(p_riichs_info))
    {
        ret = RIICHS_ERR_INVALID_ARG;
    }
    else
    {
        /* Calls the API function. */
        ret = riichs_advance(p_riichs_info);
    }

    return ret;
} /* End of function r_riichs_advance() */

/***********************************************************************************************************************
 * Function Name: riichs_advance
 * Description  : sub function of r_riichs_advance.
 *                Advances the IIC communication.
 *              : The return value shows the communication result. Refer to the return value.
 * Arguments    : riichs_info_t * p_riichs_info     ;   IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Successful operation
 *              : RIICHS_ERR_BUS_BUSY              ; Bus busy
 *              : RIICHS_ERR_NO_INIT               ; Uninitialized state
 *              : RIICHS_ERR_AL                    ; Arbitration lost error
 *              : RIICHS_ERR_TMO                   ; Time out error
 *              : RIICHS_ERR_OTHER                 ; Other error
 **********************************************************************************************************************/
static riichs_return_t riichs_advance (riichs_info_t * p_riichs_info)
{
    volatile uint32_t * const picsr2_reg = RIICHS_ICSR2_ADR;
    volatile uint32_t * const picsier_reg = RIICHS_ICSIER_ADR;
    volatile uint32_t * const piccsier_reg = RIICHS_ICCSIER_ADR;
    riichs_return_t ret;

    /* Checks the channel status. */
    ret = riichs_check_chstatus_advance(p_riichs_info);
    if (RIICHS_SUCCESS != ret)
    {
        return ret;
    }

    /* Event happened? */
    if (RIICHS_EV_INIT != riichs_api_event[p_riichs_info->ch_no])
    {
        /* IIC communication processing */
        ret = riichs_func_table(riichs_api_event[p_riichs_info->ch_no], p_riichs_info);

        /* return value? */
        switch (ret)
        {
            case RIICHS_SUCCESS :
                if (RIICHS_STS_TMO == riichs_api_info[p_riichs_info->ch_no].N_status)
                {
                    /* Checks the callback function. */
                    if ((((((RIICHS_MODE_M_SEND == riichs_api_info[p_riichs_info->ch_no].N_Mode)
                            || (RIICHS_MODE_M_SEND == riichs_api_info[p_riichs_info->ch_no].B_Mode))
                            || (RIICHS_MODE_M_RECEIVE == riichs_api_info[p_riichs_info->ch_no].N_Mode))
                            || (RIICHS_MODE_M_RECEIVE == riichs_api_info[p_riichs_info->ch_no].B_Mode))
                            || (RIICHS_MODE_M_SEND_RECEIVE == riichs_api_info[p_riichs_info->ch_no].N_Mode))
                            || (RIICHS_MODE_M_SEND_RECEIVE == riichs_api_info[p_riichs_info->ch_no].B_Mode))
                    {
                        riichs_set_ch_status(priichs_info_m[p_riichs_info->ch_no], RIICHS_TMO);
                        if (NULL != g_riichs_callbackfunc_m[p_riichs_info->ch_no])
                        {
                            g_riichs_callbackfunc_m[p_riichs_info->ch_no]();
                        }
                    }

                    if ((RIICHS_MODE_S_SEND == riichs_api_info[p_riichs_info->ch_no].N_Mode)
                            || (RIICHS_MODE_S_RECEIVE == riichs_api_info[p_riichs_info->ch_no].N_Mode))
                    {
                        riichs_set_ch_status(priichs_info_s[p_riichs_info->ch_no], RIICHS_TMO);
                        if (NULL != g_riichs_callbackfunc_s[p_riichs_info->ch_no])
                        {
                            g_riichs_callbackfunc_s[p_riichs_info->ch_no]();
                        }
                    }
                }

                else if (((RIICHS_STS_SP_COND_WAIT != riichs_api_info[p_riichs_info->ch_no].N_status)
                        || (0x00000000 != ((*picsr2_reg) & RIICHS_ICSR2_STOP))) || (0x00000000 != ((*picsier_reg) & RIICHS_ICSIER_SP)))
                {
                    /* Advances communication. (Not finished) */
                    /* Do Nothing */
                }
                else
                {
                    /* Finished communication.(after detect stop condition) */
                    /* Updates the channel status. */
                    if (RIICHS_ICSR2_AL == ((*picsr2_reg) & RIICHS_ICSR2_AL))
                    {
                        if (((RIICHS_MODE_S_READY == riichs_api_info[p_riichs_info->ch_no].N_Mode)
                                || (RIICHS_MODE_S_SEND == riichs_api_info[p_riichs_info->ch_no].N_Mode))
                                || (RIICHS_MODE_S_RECEIVE == riichs_api_info[p_riichs_info->ch_no].N_Mode))
                        {
                            riichs_set_ch_status(priichs_info_s[p_riichs_info->ch_no], RIICHS_FINISH);
                        }

                        riichs_set_ch_status(priichs_info_m[p_riichs_info->ch_no], RIICHS_AL);
                    }
                    else if (RIICHS_ICSR2_NACKF == ((*picsr2_reg) & RIICHS_ICSR2_NACKF))
                    {
                        riichs_set_ch_status(p_riichs_info, RIICHS_NACK);
                    }
                    else
                    {
                        riichs_set_ch_status(p_riichs_info, RIICHS_FINISH);
                    }

                    /* Checks the callback function. */
                    if ((((((RIICHS_MODE_M_SEND == riichs_api_info[p_riichs_info->ch_no].N_Mode)
                            || (RIICHS_MODE_M_SEND == riichs_api_info[p_riichs_info->ch_no].B_Mode))
                            || (RIICHS_MODE_M_RECEIVE == riichs_api_info[p_riichs_info->ch_no].N_Mode))
                            || (RIICHS_MODE_M_RECEIVE == riichs_api_info[p_riichs_info->ch_no].B_Mode))
                            || (RIICHS_MODE_M_SEND_RECEIVE == riichs_api_info[p_riichs_info->ch_no].N_Mode))
                            || (RIICHS_MODE_M_SEND_RECEIVE == riichs_api_info[p_riichs_info->ch_no].B_Mode))
                    {
                        if (NULL != g_riichs_callbackfunc_m[p_riichs_info->ch_no])
                        {
                            g_riichs_callbackfunc_m[p_riichs_info->ch_no]();
                        }
                    }

                    if ((RIICHS_MODE_S_SEND == riichs_api_info[p_riichs_info->ch_no].N_Mode)
                            || (RIICHS_MODE_S_RECEIVE == riichs_api_info[p_riichs_info->ch_no].N_Mode))
                    {
                        if (NULL != g_riichs_callbackfunc_s[p_riichs_info->ch_no])
                        {
                            g_riichs_callbackfunc_s[p_riichs_info->ch_no]();
                        }
                    }

                    /* Return to the last mode */
                    if ((RIICHS_MODE_S_READY == riichs_api_info[p_riichs_info->ch_no].N_Mode)
                            || (RIICHS_MODE_S_READY == riichs_api_info[p_riichs_info->ch_no].B_Mode))
                    {
                        /* Initialize API status on RAM */
                        riichs_api_mode_set(p_riichs_info, RIICHS_MODE_NONE);
                        riichs_api_mode_set(p_riichs_info, RIICHS_MODE_S_READY);
                        riichs_api_status_set(p_riichs_info, RIICHS_STS_IDLE_EN_SLV);

                        /* ---- Enables IIC bus interrupt enable register. ---- */
                        if (((uint8_t *) FIT_NO_PTR != priichs_info_s[p_riichs_info->ch_no]->p_data1st)
                                && ((uint8_t *) FIT_NO_PTR != priichs_info_s[p_riichs_info->ch_no]->p_data2nd))
                        {
                            /* Enables slave send and slave receive */
                            riichs_int_icsier_iccsier_setting(priichs_info_s[p_riichs_info->ch_no], RIICHS_ICSIER_INIT, RIICHS_ICCSIER_TX_RX);
                        }
                        else if (((uint8_t *) FIT_NO_PTR != priichs_info_s[p_riichs_info->ch_no]->p_data1st)
                                && ((uint8_t *) FIT_NO_PTR == priichs_info_s[p_riichs_info->ch_no]->p_data2nd))
                        {
                            /* Enable slave send */
                            riichs_int_icsier_iccsier_setting(priichs_info_s[p_riichs_info->ch_no], RIICHS_ICSIER_INIT, RIICHS_ICCSIER_TX);
                        }
                        else if (((uint8_t *) FIT_NO_PTR == priichs_info_s[p_riichs_info->ch_no]->p_data1st)
                                && ((uint8_t *) FIT_NO_PTR != priichs_info_s[p_riichs_info->ch_no]->p_data2nd))
                        {
                            /* Enable slave receive */
                            riichs_int_icsier_iccsier_setting(priichs_info_s[p_riichs_info->ch_no], RIICHS_ICSIER_INIT, RIICHS_ICCSIER_RX);
                        }
                        else
                        {
                            /* Do Nothing */
                        }
                    }
                    else
                    {
                        /* Initialize API status on RAM */
                        riichs_api_mode_set(p_riichs_info, RIICHS_MODE_NONE);
                        riichs_api_status_set(p_riichs_info, RIICHS_STS_IDLE);

                        /* Initializes ICSIER register. */
                        *picsier_reg = RIICHS_ICSIER_INIT;
                        /* WAIT_LOOP */
                        while (RIICHS_ICSIER_INIT != (*picsier_reg))
                        {
                            /* Do Nothing */
                        }

                        /* Initializes ICCSIER register. */
                        *piccsier_reg = RIICHS_ICCSIER_INIT;
                        /* WAIT_LOOP */
                        while (RIICHS_ICCSIER_INIT != (*piccsier_reg))
                        {
                            /* Do Nothing */
                        }
                    }
                }
            break;

            default :

                /* Updates the channel status. */
                riichs_set_ch_status(p_riichs_info, RIICHS_ERROR);
            break;
        }
    }

    /* Event nothing. */
    else
    {
        ret = RIICHS_ERR_OTHER;
    }

    return ret;
} /* End of function riichs_advance() */

/***********************************************************************************************************************
 * Function Name: riichs_func_table
 * Description  : IIC Protocol Execution Processing.
 *                Executes the function which set in "pfunc".
 * Arguments    : riichs_api_event_t event    ; Event
 *              : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : Refer to the each calling function.
 **********************************************************************************************************************/
static riichs_return_t riichs_func_table (riichs_api_event_t event, riichs_info_t * p_riichs_info)
{
    riichs_return_t ret;
    riichs_return_t (*pFunc) (riichs_info_t *);
    riichs_api_status_t n_status;

    /* Acquires a now state. */
    n_status = riichs_api_info[p_riichs_info->ch_no].N_status;

    /* Checks the parameter. */
    if ((RIICHS_STS_MAX > n_status) && (RIICHS_EV_MAX > event))
    {
        /* Checks the appointed function. */
        if (NULL != gc_riichs_mtx_tbl[n_status][event].proc)
        {
            /* Sets the function. */
            pFunc = gc_riichs_mtx_tbl[n_status][event].proc;

            /* Event flag initialization. */
            riichs_api_event[p_riichs_info->ch_no] = RIICHS_EV_INIT;

            /* Calls the status transition function. */
            ret = (*pFunc)(p_riichs_info);
        }
        else
        {
            /* Appointed function error */
            ret = RIICHS_ERR_OTHER;
        }
    }
    else
    {
        /* Appointed function error */
        ret = RIICHS_ERR_OTHER;
    }

    return ret;
} /* End of function riichs_func_table() */

/***********************************************************************************************************************
 * Function Name: riichs_init_driver
 * Description  : Initialize IIC Driver Processing.
 *                Initializes the IIC registers.
 *              : Sets the internal status.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Successful operation, idle state
 **********************************************************************************************************************/
static riichs_return_t riichs_init_driver (riichs_info_t * p_riichs_info)
{
    /* Initializes the IIC registers. */
    /* Sets the internal status. */
    /* Enables the interrupt. */
    /* Clears the RIICHS reset or internal reset. */
    riichs_enable(p_riichs_info);

    return RIICHS_SUCCESS;
} /* End of function riichs_init_driver() */

/***********************************************************************************************************************
 * Function Name: riichs_send_master_code_cond
 * Description  : Send master code Processing.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Successful operation, communication state
 **********************************************************************************************************************/
static riichs_return_t riichs_send_master_code_cond (riichs_info_t * p_riichs_info)
{
    uint8_t buf_send_data = 0x08;

    /* Enables slave send and slave receive */
    riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_TEND_NAK_AL | RIICHS_ICSIER_ST_NAK_AL, RIICHS_ICCSIER_RX);

    riichs_set_sending_data(p_riichs_info, &buf_send_data);

    return RIICHS_SUCCESS;
} /* End of function riichs_send_master_code_cond() */

/***********************************************************************************************************************
 * Function Name: riichs_generate_start_cond
 * Description  : Generate Start Condition Processing.
 *                Checks bus busy. Sets the internal status. Generates the start condition.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Successful operation, communication state
 *              : RIICHS_ERR_BUS_BUSY              ; Bus busy error
 **********************************************************************************************************************/
static riichs_return_t riichs_generate_start_cond (riichs_info_t * p_riichs_info)
{
    riichs_return_t ret;
    bool boolret;
    volatile uint32_t uctmp;
    volatile uint32_t temp_picsr2_reg;
    volatile uint32_t temp_picsier_reg;

    /* Creates the register pointer for the specified RIICHS channel. */
    volatile uint32_t * const picsier_reg = RIICHS_ICSIER_ADR;
    volatile uint32_t * const picsr2_reg = RIICHS_ICSR2_ADR;
    temp_picsr2_reg = *picsr2_reg;
    temp_picsier_reg = *picsier_reg;

    /* Checks bus busy. */
    boolret = riichs_check_bus_busy(p_riichs_info);
    if (RIICHS_FALSE == boolret)
    {
        ret = RIICHS_ERR_BUS_BUSY; /* Bus busy */
    }
    else
    {
        /* Clears each status flag. */
        temp_picsr2_reg &= RIICHS_ICSR2_STOP_CLR;
        (*picsr2_reg) = temp_picsr2_reg;
        /* WAIT_LOOP */
        while (0x00000000 != (((*picsr2_reg) & RIICHS_ICSR2_START) || ((*picsr2_reg) & RIICHS_ICSR2_STOP)))
        {
            /* Do Nothing */
        }
        /* Sets the internal status. */
        riichs_api_status_set(p_riichs_info, RIICHS_STS_ST_COND_WAIT);


        /* Clears ALIE bit. */
        temp_picsier_reg &= (~RIICHS_ICSIER_AL);
        (*picsier_reg) = temp_picsier_reg;
        uctmp = *picsier_reg; /* Reads ICSIER. */

        /* Enables IIC bus interrupt enable register. */
        if (RIICHS_MODE_S_READY == riichs_api_info[p_riichs_info->ch_no].B_Mode)
        {
            if (((uint8_t *) FIT_NO_PTR != priichs_info_s[p_riichs_info->ch_no]->p_data1st)
                    && ((uint8_t *) FIT_NO_PTR != priichs_info_s[p_riichs_info->ch_no]->p_data2nd))
            {
                /* Enables slave send and slave receive */
                riichs_int_icsier_iccsier_setting(priichs_info_s[p_riichs_info->ch_no], RIICHS_ICSIER_ST_NAK_AL, RIICHS_ICCSIER_TX_RX);
            }
            else if (((uint8_t *) FIT_NO_PTR != priichs_info_s[p_riichs_info->ch_no]->p_data1st)
                    && ((uint8_t *) FIT_NO_PTR == priichs_info_s[p_riichs_info->ch_no]->p_data2nd))
            {
                /* Enable slave send */
                riichs_int_icsier_iccsier_setting(priichs_info_s[p_riichs_info->ch_no], RIICHS_ICSIER_ST_NAK_AL, RIICHS_ICCSIER_TX);
            }
            else if (((uint8_t *) FIT_NO_PTR == priichs_info_s[p_riichs_info->ch_no]->p_data1st)
                    && ((uint8_t *) FIT_NO_PTR != priichs_info_s[p_riichs_info->ch_no]->p_data2nd))
            {
                /* Enable slave receive */
                riichs_int_icsier_iccsier_setting(priichs_info_s[p_riichs_info->ch_no], RIICHS_ICSIER_ST_NAK_AL, RIICHS_ICCSIER_RX);
            }
            else
            {
                /* Do Nothing */
            }
        }
        else
        {
            riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_ST_NAK_AL, RIICHS_ICCSIER_INIT);
        }

        /* Generates the start condition. */
        riichs_start_cond_generate(p_riichs_info);

        ret = RIICHS_SUCCESS;
    }

    return ret;
} /* End of function riichs_generate_start_cond() */

/***********************************************************************************************************************
 * Function Name: riichs_after_gen_start_cond
 * Description  : After Start Condition Generation Processing.
 *                Performs one of the following processing according to the state.
 *                Transmits the slave address for writing.
 *                Transmits the slave address for reading.
 *                Generates the stop condition.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Successful operation, communication state
 *              : RIICHS_ERR_OTHER                 ; Other error
 **********************************************************************************************************************/
static riichs_return_t riichs_after_gen_start_cond (riichs_info_t * p_riichs_info)
{
    riichs_return_t ret = RIICHS_SUCCESS;
    uint8_t buf_send_data;
    volatile uint32_t cnt;

    /* IIC mode? */
    switch (riichs_api_info[p_riichs_info->ch_no].N_Mode)
    {
        case RIICHS_MODE_M_SEND :
        case RIICHS_MODE_M_SEND_RECEIVE :

            /* Checks the previous status. */
            switch (riichs_api_info[p_riichs_info->ch_no].B_status)
            {
                /* The previous status is idle. */
                case RIICHS_STS_IDLE :
                case RIICHS_STS_IDLE_EN_SLV :
                case RIICHS_STS_MASTER_CODE_WAIT :
                    /* Is the slave address pointer set? */
                    if ((uint8_t *) FIT_NO_PTR != p_riichs_info->p_slv_adr) /* Pattern 4 of Master Write */
                    {
                        /* Sets a write code. */
                        buf_send_data = (uint8_t) ((*(p_riichs_info->p_slv_adr)) << 1U);
                        buf_send_data &= W_CODE;

                        /* Sets the internal status. */
                        riichs_api_status_set(p_riichs_info, RIICHS_STS_SEND_SLVADR_W_WAIT);

                        /* Enables the IIC bus interrupt. */
                        /* Transmit data empty interrupt request is enabled. */
                        if (RIICHS_MODE_S_READY == riichs_api_info[p_riichs_info->ch_no].B_Mode)
                        {
                            if (((uint8_t *) FIT_NO_PTR != priichs_info_s[p_riichs_info->ch_no]->p_data1st)
                                    && ((uint8_t *) FIT_NO_PTR != priichs_info_s[p_riichs_info->ch_no]->p_data2nd))
                            {
                                /* Enables slave send and slave receive */
                                riichs_int_icsier_iccsier_setting(priichs_info_s[p_riichs_info->ch_no], RIICHS_ICSIER_TEND_NAK_AL, RIICHS_ICCSIER_TX_RX);
                            }
                            else if (((uint8_t *) FIT_NO_PTR != priichs_info_s[p_riichs_info->ch_no]->p_data1st)
                                    && ((uint8_t *) FIT_NO_PTR == priichs_info_s[p_riichs_info->ch_no]->p_data2nd))
                            {
                                /* Enable slave send */
                                riichs_int_icsier_iccsier_setting(priichs_info_s[p_riichs_info->ch_no], RIICHS_ICSIER_TEND_NAK_AL, RIICHS_ICCSIER_TX);
                            }
                            else if (((uint8_t *) FIT_NO_PTR == priichs_info_s[p_riichs_info->ch_no]->p_data1st)
                                    && ((uint8_t *) FIT_NO_PTR != priichs_info_s[p_riichs_info->ch_no]->p_data2nd))
                            {
                                /* Enable slave receive */
                                riichs_int_icsier_iccsier_setting(priichs_info_s[p_riichs_info->ch_no], RIICHS_ICSIER_TEND_NAK_AL, RIICHS_ICCSIER_RX);
                            }
                            else
                            {
                                /* Do Nothing */
                            }
                        }
                        else
                        {
                            riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_TEND_NAK_AL, RIICHS_ICCSIER_INIT);
                        }

                        /* Transmits the slave address. */
                        riichs_set_sending_data(p_riichs_info, &buf_send_data);
                    }
                    else
                    {
                        /* Do Nothing */
                    }
                break;

                    /* Previous status is data transfer completion waiting status. */
                case RIICHS_STS_SEND_DATA_WAIT :

                    /* Sets a read code. */
                    buf_send_data = (uint8_t) ((*(p_riichs_info->p_slv_adr)) << 1U);
                    buf_send_data |= R_CODE;

                    /* Sets the internal status. */
                    riichs_api_status_set(p_riichs_info, RIICHS_STS_SEND_SLVADR_R_WAIT);

                    /* Enables the IIC bus interrupt. */
                    riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_NAK_AL, RIICHS_ICCSIER_RX);
                    /* Transmits the slave address. */
                    riichs_set_sending_data(p_riichs_info, &buf_send_data);

                break;

                default :

                    /* None status. */
                    ret = RIICHS_ERR_OTHER;
                break;
            }
        break;

        case RIICHS_MODE_M_RECEIVE :

            /* Sets a read code. */
            buf_send_data = (uint8_t) ((*(p_riichs_info->p_slv_adr)) << 1U);
            buf_send_data |= R_CODE;

            /* Sets the internal status. */
            riichs_api_status_set(p_riichs_info, RIICHS_STS_SEND_SLVADR_R_WAIT);

            /* Enables the IIC bus interrupt. */
            if (RIICHS_MODE_S_READY == riichs_api_info[p_riichs_info->ch_no].B_Mode)
            {
                if (((uint8_t *) FIT_NO_PTR != priichs_info_s[p_riichs_info->ch_no]->p_data1st)
                        && ((uint8_t *) FIT_NO_PTR != priichs_info_s[p_riichs_info->ch_no]->p_data2nd))
                {
                    /* Enables slave send and slave receive */
                    riichs_int_icsier_iccsier_setting(priichs_info_s[p_riichs_info->ch_no], RIICHS_ICSIER_NAK_AL, RIICHS_ICCSIER_TX_RX);
                }
                else if (((uint8_t *) FIT_NO_PTR != priichs_info_s[p_riichs_info->ch_no]->p_data1st)
                        && ((uint8_t *) FIT_NO_PTR == priichs_info_s[p_riichs_info->ch_no]->p_data2nd))
                {
                    /* Enable slave send */
                    riichs_int_icsier_iccsier_setting(priichs_info_s[p_riichs_info->ch_no], RIICHS_ICSIER_NAK_AL, RIICHS_ICCSIER_TX_RX);
                }
                else if (((uint8_t *) FIT_NO_PTR == priichs_info_s[p_riichs_info->ch_no]->p_data1st)
                        && ((uint8_t *) FIT_NO_PTR != priichs_info_s[p_riichs_info->ch_no]->p_data2nd))
                {
                    /* Enable slave receive */
                    riichs_int_icsier_iccsier_setting(priichs_info_s[p_riichs_info->ch_no], RIICHS_ICSIER_NAK_AL, RIICHS_ICCSIER_RX);
                }
                else
                {
                    /* Do Nothing */
                }
            }
            else
            {
                riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_NAK_AL, RIICHS_ICCSIER_RX);
            }

            /* Transmits the slave address. */
            riichs_set_sending_data(p_riichs_info, &buf_send_data);

        break;

        default :
            ret = RIICHS_ERR_OTHER;
        break;
    }
    return ret;
} /* End of function riichs_after_gen_start_cond() */

/***********************************************************************************************************************
 * Function Name: riichs_after_send_slvadr
 * Description  : After Slave Address Transmission Processing.
 *                Performs one of the following processing according to the state.
 *                Transmits a data. Generates the stop condition. Receives a data.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Successful operation, communication state
 *              : RIICHS_ERR_OTHER                 ; Other error
 **********************************************************************************************************************/
static riichs_return_t riichs_after_send_slvadr (riichs_info_t * p_riichs_info)
{
    riichs_return_t ret = RIICHS_SUCCESS;
    volatile uint32_t uctmp = 0x00000000;

    /* IIC mode? */
    switch (riichs_api_info[p_riichs_info->ch_no].N_Mode)
    {
        case RIICHS_MODE_M_SEND :

            /* --- Pattern 1 of Master Write --- */
            if (((uint8_t *) FIT_NO_PTR != (uint8_t *) p_riichs_info->p_data1st)
                    && ((uint8_t *) FIT_NO_PTR != (uint8_t *) p_riichs_info->p_data2nd))
            {
                /* Sets the internal status. */
                riichs_api_status_set(p_riichs_info, RIICHS_STS_SEND_DATA_WAIT);

                /* Enables the IIC bus interrupt. */
                riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_TEND_NAK_AL, RIICHS_ICCSIER_INIT);

                /* 1st data counter = 0?  */
                if (0x00000000 != p_riichs_info->cnt1st)
                {
                    /* Transmits a 1st data. */
                    riichs_set_sending_data(p_riichs_info, p_riichs_info->p_data1st);

                    /* Decreases the 1st data counter. */
                    p_riichs_info->cnt1st--;

                    /* Increases the 1st data buffer pointer. */
                    p_riichs_info->p_data1st++;
                }
                else
                {
                    ret = RIICHS_ERR_OTHER;
                }
            }

            /* --- Pattern 2 of Master Write --- */
            else if (((uint8_t *) FIT_NO_PTR == p_riichs_info->p_data1st)
                    && ((uint8_t *) FIT_NO_PTR != p_riichs_info->p_data2nd))
            {
                /* Sets the internal status. */
                riichs_api_status_set(p_riichs_info, RIICHS_STS_SEND_DATA_WAIT);

                /* Enables the IIC bus interrupt. */
                riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_TEND_NAK_AL, RIICHS_ICCSIER_INIT);

                /* 2nd data counter = 0?  */
                if (0x00000000 != p_riichs_info->cnt2nd)
                {
                    /* Transmits a 2nd data. */
                    riichs_set_sending_data(p_riichs_info, p_riichs_info->p_data2nd);

                    /* Decreases the 2nd data counter. */
                    p_riichs_info->cnt2nd--;

                    /* Increases the 2nd data buffer pointer. */
                    p_riichs_info->p_data2nd++;
                }
                else
                {
                    ret = RIICHS_ERR_OTHER;
                }
            }

            /* --- Pattern 3 of Master Write --- */
            else if (((uint8_t *) FIT_NO_PTR == p_riichs_info->p_data1st)
                    && ((uint8_t *) FIT_NO_PTR == p_riichs_info->p_data2nd))
            {
                /* Sets the internal status. */
                riichs_api_status_set(p_riichs_info, RIICHS_STS_SP_COND_WAIT);

                /* Enables the IIC bus interrupt. */
                riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_SP_NAK_AL, RIICHS_ICCSIER_INIT);

                /* Generates the stop condition. */
                riichs_stop_cond_generate(p_riichs_info);
            }
            else
            {
                ret = RIICHS_ERR_OTHER;
            }

        break;

        case RIICHS_MODE_M_RECEIVE :

            /* Sets the internal status. */
            riichs_api_status_set(p_riichs_info, RIICHS_STS_RECEIVE_DATA_WAIT);

            /* Enables the IIC bus interrupt. */
            riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_NAK_AL, RIICHS_ICCSIER_RX);

            if (2U >= p_riichs_info->cnt2nd)
            {
                /* The period between the ninth clock cycle and the first clock cycle is held low. */
                riichs_receive_wait_setting(p_riichs_info);
            }

            if (0x00000001 >= p_riichs_info->cnt2nd)
            {
                /* Processing before receiving the "last byte - 1byte" in RX. */
                riichs_receive_pre_end_setting(p_riichs_info);
            }

            /* Performs dummy read. */
            uctmp = riichs_get_receiving_data(p_riichs_info);

        break;

        case RIICHS_MODE_M_SEND_RECEIVE :

            /* Now status? */
            switch (riichs_api_info[p_riichs_info->ch_no].N_status)
            {
                case RIICHS_STS_SEND_SLVADR_W_WAIT :

                    /* Sets the internal status. */
                    riichs_api_status_set(p_riichs_info, RIICHS_STS_SEND_DATA_WAIT);

                    /* Enables the IIC bus interrupt. */
                    riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_TEND_NAK_AL, RIICHS_ICCSIER_INIT);

                    /* Transmits a 1st data. */
                    riichs_set_sending_data(p_riichs_info, p_riichs_info->p_data1st);

                    /* Decreases the 1st data counter. */
                    p_riichs_info->cnt1st--;

                    /* Increases the 1st Data buffer pointer. */
                    p_riichs_info->p_data1st++;

                break;

                case RIICHS_STS_SEND_SLVADR_R_WAIT :

                    /* Sets the internal status. */
                    riichs_api_status_set(p_riichs_info, RIICHS_STS_RECEIVE_DATA_WAIT);

                    /* Enables the IIC bus interrupt. */
                    riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_NAK_AL, RIICHS_ICCSIER_RX);

                    if (2U >= p_riichs_info->cnt2nd)
                    {
                        /* The period between the ninth clock cycle and the first clock cycle is held low. */
                        riichs_receive_wait_setting(p_riichs_info);
                    }

                    if (0x00000001 >= p_riichs_info->cnt2nd)
                    {
                        /* Processing before receiving the "last byte - 1byte" in RX. */
                        riichs_receive_pre_end_setting(p_riichs_info);
                    }

                    /* Performs dummy read. */
                    uctmp = riichs_get_receiving_data(p_riichs_info);

                break;

                default :
                    ret = RIICHS_ERR_OTHER;
                break;
            }
        break;

        default :
            ret = RIICHS_ERR_OTHER;
        break;

    }

    return ret;
} /* End of function riichs_after_send_slvadr() */

/***********************************************************************************************************************
 * Outline      : 
 * Function Name: riichs_after_receive_slvadr
 * Description  : After Slave Address Receive Processing
 *                Performs one of the following processing according to the state.
 *                Transmits a data. Generates the stop condition. Receives a data.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Successful operation, communication state
 *              : RIICHS_ERR_OTHER                 ; Other error
 **********************************************************************************************************************/
static riichs_return_t riichs_after_receive_slvadr (riichs_info_t * p_riichs_info)
{
    volatile uint32_t * const picsr2_reg = RIICHS_ICSR2_ADR;
    volatile uint32_t * const picmmr_reg = RIICHS_ICMMR_ADR;
    volatile uint32_t uctmp = 0x00000000;
    volatile uint32_t temp_picsr2_reg = 0x00000000;
    temp_picsr2_reg = *picsr2_reg;
    uint8_t blank_data[1] =
    { BLANK };

    /* Clears each status flag. */
    temp_picsr2_reg &= RIICHS_ICSR2_STOP_CLR;
    (*picsr2_reg) = temp_picsr2_reg;
    /* WAIT_LOOP */
    while (0x00000000 != ((*picsr2_reg) & RIICHS_ICSR2_STOP))
    {
        /* Do Nothing */
    }

    /* ---- Enables IIC bus interrupt enable register. ---- */
    if (((uint8_t *) FIT_NO_PTR != p_riichs_info->p_data1st) && ((uint8_t *) FIT_NO_PTR != p_riichs_info->p_data2nd))
    {
        /* Enables slave send and slave receive */
        riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_SP_NAK, RIICHS_ICCSIER_TX_RX);
    }
    else if (((uint8_t *) FIT_NO_PTR != p_riichs_info->p_data1st) && ((uint8_t *) FIT_NO_PTR == p_riichs_info->p_data2nd))
    {
        /* Enable slave send */
        riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_SP_NAK, RIICHS_ICCSIER_TX);
    }
    else if (((uint8_t *) FIT_NO_PTR == p_riichs_info->p_data1st) && ((uint8_t *) FIT_NO_PTR != p_riichs_info->p_data2nd))
    {
        /* Enable slave receive */
        riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_SP, RIICHS_ICCSIER_RX);
    }
    else
    {
        /* Do Nothing */
    }

    /* Slave receive mode? */
    if (0x00 == ((*picmmr_reg) & RIICHS_ICMMR_TRS))
    {
        /* Sets the internal status. */
        riichs_api_status_set(p_riichs_info, RIICHS_STS_RECEIVE_DATA_WAIT);

        if (0x00000002 >= p_riichs_info->cnt2nd)
        {
            /* The period between the ninth clock cycle and the first clock cycle is held low. */
            riichs_receive_wait_setting(p_riichs_info);
        }

        if (0x00000001 >= p_riichs_info->cnt2nd)
        {
            /* Processing before receiving the "last byte - 1byte" in RX. */
            riichs_receive_pre_end_setting(p_riichs_info);
        }

        /* Performs dummy read. */
        uctmp = riichs_get_receiving_data(p_riichs_info);
    }

    /* Slave send mode */
    else
    {
        /* Sets the internal status. */
        riichs_api_status_set(p_riichs_info, RIICHS_STS_SEND_DATA_WAIT);

        /* 1st data counter = 0?  */
        if (0x00000000 != p_riichs_info->cnt1st)
        {
            if ((uint8_t *) FIT_NO_PTR == p_riichs_info->p_data1st)
            {
                /* Internal error */
                return RIICHS_ERR_OTHER;
            }

            /* Transmits a 1st data. */
            riichs_set_sending_data(p_riichs_info, p_riichs_info->p_data1st);

            /* Decreases the 1st data counter. */
            p_riichs_info->cnt1st--;

            /* Increases the 1st data buffer pointer. */
            p_riichs_info->p_data1st++;
        }
        else
        {
            /* Transmits a "FFh" data. */
            riichs_set_sending_data(p_riichs_info, blank_data);
        }

    }

    return RIICHS_SUCCESS;

} /* End of function riichs_after_receive_slvadr() */

/***********************************************************************************************************************
 * Function Name: riichs_write_data_sending
 * Description  : After Transmitting Data Processing.
 *                Performs one of the following processing  according to the state.
 *                Transmits a data. Generates stop condition. Generates restart condition.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Successful operation, communication state
 *              : RIICHS_ERR_OTHER                 ; Other error
 **********************************************************************************************************************/
static riichs_return_t riichs_write_data_sending (riichs_info_t * p_riichs_info)
{
    riichs_return_t ret = RIICHS_SUCCESS;
    uint8_t blank_data[1] =
    { BLANK };

    /* IIC mode? */
    switch (riichs_api_info[p_riichs_info->ch_no].N_Mode)
    {
        case RIICHS_MODE_M_SEND :

            /* Is 1st data pointer set? */
            if ((uint8_t *) FIT_NO_PTR != p_riichs_info->p_data1st)
            {
                /* 1st data counter = 0?  */
                if (0x00000000 != p_riichs_info->cnt1st) /* Pattern 1 of Master Write */
                {
                    /* Transmits a 1st data. */
                    riichs_set_sending_data(p_riichs_info, p_riichs_info->p_data1st);

                    /* Decreases the 1st data counter. */
                    p_riichs_info->cnt1st--;

                    /* Increases the 1st data buffer pointer. */
                    p_riichs_info->p_data1st++;

                    break;
                }
            }

            /* Is 2nd data pointer set? */
            if ((uint8_t *) FIT_NO_PTR != p_riichs_info->p_data2nd)
            {
                /* 2nd data counter = 0? */
                if (0x00000000 != p_riichs_info->cnt2nd) /* Pattern 2 of Master Write */
                {
                    /* Transmits a 2nd data. */
                    riichs_set_sending_data(p_riichs_info, p_riichs_info->p_data2nd);

                    /* Decreases the 2nd data counter. */
                    p_riichs_info->cnt2nd--;

                    /* Increases the 2nd data buffer pointer. */
                    p_riichs_info->p_data2nd++;

                    break;
                }
            }

            /* Sets the internal status. */
            riichs_api_status_set(p_riichs_info, RIICHS_STS_SP_COND_WAIT);

            /* Enables the IIC bus interrupt. */
            riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_SP_NAK_AL, RIICHS_ICCSIER_INIT);

            /* Generates the stop condition. */
            riichs_stop_cond_generate(p_riichs_info);

        break;

        case RIICHS_MODE_M_SEND_RECEIVE :

            /* Is 1st data pointer set? */
            if (0x00000000 != p_riichs_info->cnt1st)
            {
                /* Transmits a 1st data. */
                riichs_set_sending_data(p_riichs_info, p_riichs_info->p_data1st);

                /* Decreases the 1st data counter. */
                p_riichs_info->cnt1st--;

                /* Increases the 1st data buffer pointer. */
                p_riichs_info->p_data1st++;

                break;
            }

            /* Sets the internal status. */
            riichs_api_status_set(p_riichs_info, RIICHS_STS_ST_COND_WAIT);

            /* Enables the IIC bus interrupt. */
            riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_ST_NAK_AL, RIICHS_ICCSIER_INIT);

            /* Restarts the condition generation */
            riichs_re_start_cond_generate(p_riichs_info);

        break;

        case RIICHS_MODE_S_SEND :

            /* Is 1st data pointer set? */
            if ((uint8_t *) FIT_NO_PTR != p_riichs_info->p_data1st)
            {
                /* 1st data counter = 0?  */
                if (0x00000000 != p_riichs_info->cnt1st)
                {
                    /* Transmits a 1st data. */
                    riichs_set_sending_data(p_riichs_info, p_riichs_info->p_data1st);

                    /* Decreases the 1st data counter. */
                    p_riichs_info->cnt1st--;

                    /* Increases the 1st data buffer pointer. */
                    p_riichs_info->p_data1st++;
                }
                else
                {
                    /* Transmits a "FFh" data. */
                    riichs_set_sending_data(p_riichs_info, blank_data);
                }
            }
            else
            {
                /* Transmits a "FFh" data. */
                riichs_set_sending_data(p_riichs_info, blank_data);
            }

        break;

        default :
            ret = RIICHS_ERR_OTHER;
        break;
    }

    return ret;
} /* End of function riichs_write_data_sending() */

/***********************************************************************************************************************
 * Function Name: riichs_read_data_receiving
 * Description  : After Receiving Data Processing.
 *                Performs one of the following processing  according to the state.
 *                Receives a data. Generates stop condition.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Successful operation, communication state
 **********************************************************************************************************************/
static riichs_return_t riichs_read_data_receiving (riichs_info_t * p_riichs_info)
{
    volatile uint32_t uctmp = 0x00000000;

    if (0x00000003 >= p_riichs_info->cnt2nd)
    {
        /* The period between the ninth clock cycle and the first clock cycle is held low. */
        riichs_receive_wait_setting(p_riichs_info);
    }

    if (0x00000002 >= p_riichs_info->cnt2nd)
    {
        /* Processing before receiving the "last byte - 1byte" in RX. */
        riichs_receive_pre_end_setting(p_riichs_info);
    }

    if (0x00000001 >= p_riichs_info->cnt2nd)
    {
        if (RIICHS_MODE_S_RECEIVE != riichs_api_info[p_riichs_info->ch_no].N_Mode)
        {
            /* Sets the internal status. */
            riichs_api_status_set(p_riichs_info, RIICHS_STS_SP_COND_WAIT);

            /* Enables the IIC bus interrupt. */
            riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_SP_NAK_AL, RIICHS_ICCSIER_INIT);

            /* Generates the stop condition. */
            riichs_stop_cond_generate(p_riichs_info);
        }

        if (0x00000000 != p_riichs_info->cnt2nd)
        {
            /* Stores the received data. */
            *p_riichs_info->p_data2nd = riichs_get_receiving_data(p_riichs_info);
            p_riichs_info->cnt2nd--; /* Decreases the receiving data counter. */
            p_riichs_info->p_data2nd++; /* Increases the receiving data buffer pointer. */
        }
        else
        {
            /* Performs dummy read. */
            uctmp = riichs_get_receiving_data(p_riichs_info);
        }

        /* Reception end setting */
        riichs_receive_end_setting(p_riichs_info);
    }
    else
    {
        /* Stores the received data. */
        *p_riichs_info->p_data2nd = riichs_get_receiving_data(p_riichs_info);
        p_riichs_info->cnt2nd--; /* Decreases the receiving data counter. */
        p_riichs_info->p_data2nd++; /* Increases the receiving data buffer pointer. */
    }

    return RIICHS_SUCCESS;
} /* End of function riichs_read_data_receiving() */

/***********************************************************************************************************************
 * Function Name: riichs_after_dtct_stop_cond
 * Description  : After Generating Stop Condition Processing.
 *                Checks bus busy.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Successful operation, finished communication and idle state
 *              : RIICHS_ERR_BUS_BUSY              ; Bus busy error
 *              : RIICHS_ERR_AL                    ; Arbitration lost error
 *              : RIICHS_ERR_TMO                  ; Time out error
 **********************************************************************************************************************/
static riichs_return_t riichs_after_dtct_stop_cond (riichs_info_t * p_riichs_info)
{
    riichs_return_t ret = RIICHS_SUCCESS;
    bool boolret = RIICHS_FALSE;
    volatile uint32_t * const picscr_reg = RIICHS_ICSCR_ADR;
    volatile uint32_t temp_picscr_reg = 0x00000000;
    temp_picscr_reg = *picscr_reg;

    /* Waits from "bus busy" to "bus ready". */
    boolret = riichs_check_bus_busy(p_riichs_info);
    if (RIICHS_FALSE == boolret)
    {
        ret = RIICHS_ERR_BUS_BUSY;
    }
    else
    {
        if (RIICHS_TMO == g_riichs_ChStatus[p_riichs_info->ch_no])
        {
            ret = RIICHS_ERR_TMO;
        }

        if (RIICHS_AL == g_riichs_ChStatus[p_riichs_info->ch_no])
        {
            ret = RIICHS_ERR_AL;
        }
    }

    if ((RIICHS_MODE_S_SEND == riichs_api_info[p_riichs_info->ch_no].N_Mode)
            || (RIICHS_MODE_S_RECEIVE == riichs_api_info[p_riichs_info->ch_no].N_Mode))
    {
        /* Disable slave address1 */
        temp_picscr_reg &= RIICHS_ICSCR_SAR_DISABLE;
        (*picscr_reg) = temp_picscr_reg;
    }

    return ret;
} /* End of function riichs_after_dtct_stop_cond() */

/***********************************************************************************************************************
 * Function Name: riichs_arbitration_lost
 * Description  : Arbitration Lost Error Processing. 
 *                Returns RIICHS_ERR_AL.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : RIICHS_SUCCESS                    ; Arbitration lost error
 **********************************************************************************************************************/
static riichs_return_t riichs_arbitration_lost (riichs_info_t * p_riichs_info)
{
    /* Checks the internal mode. */
    /* slave transfer not enable in now */
    if (((RIICHS_MODE_M_SEND == riichs_api_info[p_riichs_info->ch_no].N_Mode)
            || (RIICHS_MODE_M_RECEIVE == riichs_api_info[p_riichs_info->ch_no].N_Mode))
            || (RIICHS_MODE_M_SEND_RECEIVE == riichs_api_info[p_riichs_info->ch_no].N_Mode))
    {
        /* slave transfer not enable in now and before */
        if ((((RIICHS_MODE_M_SEND == riichs_api_info[p_riichs_info->ch_no].B_Mode)
                || (RIICHS_MODE_M_RECEIVE == riichs_api_info[p_riichs_info->ch_no].B_Mode))
                || (RIICHS_MODE_M_SEND_RECEIVE == riichs_api_info[p_riichs_info->ch_no].B_Mode))
                || (RIICHS_MODE_NONE == riichs_api_info[p_riichs_info->ch_no].B_Mode))
        {
            /* Sets the internal status. */
            riichs_api_status_set(p_riichs_info, RIICHS_STS_SP_COND_WAIT);

            /* Enables the IIC bus interrupt. */
            riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_SP, RIICHS_ICCSIER_INIT);

        }
        else if (RIICHS_MODE_S_READY == riichs_api_info[p_riichs_info->ch_no].B_Mode)
        {
            /* Initialize API status on RAM */
            riichs_api_mode_event_init(priichs_info_s[p_riichs_info->ch_no], RIICHS_MODE_S_READY);

            /* Sets the internal status. */
            riichs_api_status_set(priichs_info_s[p_riichs_info->ch_no], RIICHS_STS_AL);

            /* ---- Enables IIC bus interrupt enable register. ---- */
            if (((uint8_t *) FIT_NO_PTR != priichs_info_s[p_riichs_info->ch_no]->p_data1st)
                    && ((uint8_t *) FIT_NO_PTR != priichs_info_s[p_riichs_info->ch_no]->p_data2nd))
            {
                /* Enables slave send and slave receive */
                riichs_int_icsier_iccsier_setting(priichs_info_s[p_riichs_info->ch_no], RIICHS_ICSIER_SP_NAK, RIICHS_ICCSIER_TX_RX);
            }
            else if (((uint8_t *) FIT_NO_PTR != priichs_info_s[p_riichs_info->ch_no]->p_data1st)
                    && ((uint8_t *) FIT_NO_PTR == priichs_info_s[p_riichs_info->ch_no]->p_data2nd))
            {
                /* Enable slave send */
                riichs_int_icsier_iccsier_setting(priichs_info_s[p_riichs_info->ch_no], RIICHS_ICSIER_SP_NAK, RIICHS_ICCSIER_TX);
            }
            else if (((uint8_t *) FIT_NO_PTR == priichs_info_s[p_riichs_info->ch_no]->p_data1st)
                    && ((uint8_t *) FIT_NO_PTR != priichs_info_s[p_riichs_info->ch_no]->p_data2nd))
            {
                /* Enable slave receive */
                riichs_int_icsier_iccsier_setting(priichs_info_s[p_riichs_info->ch_no], RIICHS_ICSIER_SP, RIICHS_ICCSIER_RX);
            }
            else
            {
                /* Do Nothing */
            }
        }
        else
        {
            /* Do Nothing */
        }
    }

    /* slave transfer enable in now */
    else if ((RIICHS_MODE_S_SEND == riichs_api_info[p_riichs_info->ch_no].N_Mode)
            || (RIICHS_MODE_S_RECEIVE == riichs_api_info[p_riichs_info->ch_no].N_Mode))
    {
        /* Do Nothing */
    }
    else
    {
        /* Do Nothing */
    }

    return RIICHS_SUCCESS;
} /* End of function riichs_arbitration_lost() */

/***********************************************************************************************************************
 * Function Name: riichs_time_out
 * Description  : Time Out Error Processing.
 *                Generates the time out error and returns RIICHS_TMO.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Arbitration lost error
 **********************************************************************************************************************/
static riichs_return_t riichs_time_out (riichs_info_t * p_riichs_info)
{
    /* Sets the internal status. */
    riichs_api_status_set(p_riichs_info, RIICHS_STS_TMO);

    return RIICHS_SUCCESS;
} /* End of function riichs_time_out() */

/***********************************************************************************************************************
 * Function Name: riichs_nack
 * Description  : Nack Error Processing.
 *                Generates the stop condition and returns RIICHS_NACK.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Successful operation
 **********************************************************************************************************************/
static riichs_return_t riichs_nack (riichs_info_t * p_riichs_info)
{
    volatile uint32_t * const picmmr_reg = RIICHS_ICMMR_ADR;
    volatile uint32_t uctmp = 0x00000000;

    /* Sets the internal status. */
    riichs_api_status_set(p_riichs_info, RIICHS_STS_SP_COND_WAIT);

    if (RIICHS_ICMMR_MST_SET == ((*picmmr_reg) & RIICHS_ICMMR_MST))
    {
        /* Enables the IIC bus interrupt. */
        riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_SP_AL, RIICHS_ICCSIER_INIT);

        /* Generates the stop condition. */
        riichs_stop_cond_generate(p_riichs_info);
    }
    else
    {
        /* Enables the IIC bus interrupt. */
        riichs_int_icsier_iccsier_setting(p_riichs_info, RIICHS_ICSIER_SP, RIICHS_ICCSIER_INIT);
    }

    /* Checks the internal mode. */
    if ((((RIICHS_MODE_M_RECEIVE == riichs_api_info[p_riichs_info->ch_no].N_Mode)
            || (RIICHS_MODE_M_SEND_RECEIVE == riichs_api_info[p_riichs_info->ch_no].N_Mode))
            || (RIICHS_MODE_S_SEND == riichs_api_info[p_riichs_info->ch_no].N_Mode))
            || (RIICHS_MODE_S_RECEIVE == riichs_api_info[p_riichs_info->ch_no].N_Mode))
    {
        /* Dummy Read */
        /* Refer to the RX User's Manual. */
        uctmp = riichs_get_receiving_data(p_riichs_info);
    }

    return RIICHS_SUCCESS;
} /* End of function riichs_nack() */

/***********************************************************************************************************************
 * Function Name: riichs_enable_slave_transfer
 * Description  : Enable IIC Slave Transfer Processing.
 *                Enable the slave address match interrupt. Sets the internal status.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Successful operation, idle state on enable slave transfer
 **********************************************************************************************************************/
static riichs_return_t riichs_enable_slave_transfer (riichs_info_t * p_riichs_info)
{
    /* Setting the IIC registers. */
    /* Includes I/O register read operation at the end of the following function. */
    riichs_slv_addr_match_int_enable(p_riichs_info);

    /* Sets the internal status. */
    riichs_api_status_set(p_riichs_info, RIICHS_STS_IDLE_EN_SLV);

    return RIICHS_SUCCESS;
} /* End of function riichs_enable_slave_transfer() */

/***********************************************************************************************************************
 * Function Name: riichs_api_mode_event_init
 * Description  : Initializes RAM.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 *              : riichs_api_mode_t new_mode  ; New mode
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_api_mode_event_init (riichs_info_t * p_riichs_info, riichs_api_mode_t new_mode)
{
    uint8_t ch_no = p_riichs_info->ch_no;

    /* Clears the event flag. */
    riichs_api_event[ch_no] = RIICHS_EV_INIT;

    /* Sets the previous mode. */
    riichs_api_info[ch_no].B_Mode = riichs_api_info[ch_no].N_Mode;

    /* Sets the mode to RIICHS_MODE_NONE. */
    riichs_api_info[ch_no].N_Mode = new_mode;
} /* End of function riichs_api_mode_event_init() */

/***********************************************************************************************************************
 * Function Name: riichs_api_mode_set
 * Description  : Update Internal Status Processing.
 *                Updates the now status and the previous status.
 * Arguments    : riichs_info_t * p_riichs_info          ; IIC Information
 *              : riichs_api_mode_t new_status    ; New status
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_api_mode_set (riichs_info_t * p_riichs_info, riichs_api_mode_t new_mode)
{
    uint8_t ch_no = p_riichs_info->ch_no;
    
    /* Sets the previous status. */
    riichs_api_info[ch_no].B_Mode = riichs_api_info[ch_no].N_Mode;

    /* Sets the now status. */
    riichs_api_info[ch_no].N_Mode = new_mode;
} /* End of function riichs_api_mode_set() */

/***********************************************************************************************************************
 * Function Name: riichs_api_status_set
 * Description  : Update Internal Status Processing.
 *                Updates the now status and the previous status.
 * Arguments    : riichs_info_t * p_riichs_info          ; IIC Information
 *              : riichs_api_status_t new_status  ; New status
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_api_status_set (riichs_info_t * p_riichs_info, riichs_api_status_t new_status)
{
    uint8_t ch_no = p_riichs_info->ch_no;
    
    /* Sets the previous status. */
    riichs_api_info[ch_no].B_status = riichs_api_info[ch_no].N_status;

    /* Sets the now status. */
    riichs_api_info[ch_no].N_status = new_status;
} /* End of function riichs_api_status_set() */

/***********************************************************************************************************************
 * Function Name: riichs_set_ch_status
 * Description  : Updates the channel status.
 * Arguments    : riichs_info_t * p_riichs_info          ; IIC Information
 *              : riichs_ch_dev_status_t status               ; channel status, device status
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_set_ch_status (riichs_info_t * p_riichs_info, riichs_ch_dev_status_t status)
{
    /* Sets the channel status. */
    g_riichs_ChStatus[p_riichs_info->ch_no] = status;

    /* Sets the device status. */
    p_riichs_info->dev_sts = status;
} /* End of function riichs_set_ch_status() */

/***********************************************************************************************************************
 * Function Name: riichs_check_chstatus_start
 * Description  : Check Channel Status Processing (at Start Function).
 *                When calls the starting function, checks the channel status.
 * Arguments    : riichs_info_t * p_riichs_info          ; IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Successful operation
 *              : RIICHS_ERR_NO_INIT               ; Uninitialized state
 *              : RIICHS_ERR_BUS_BUSY              ; Bus busy
 *              : RIICHS_ERR_AL                    ; Arbitration lost error
 *              : RIICHS_ERR_TMO                   ; Time out error
 *              : RIICHS_ERR_OTHER                 ; Other error
 **********************************************************************************************************************/
static riichs_return_t riichs_check_chstatus_start (riichs_info_t * p_riichs_info)
{
    volatile uint32_t * const picbsr_reg = RIICHS_ICBSR_ADR;
    riichs_return_t ret;

    /* Checks the channel status. */
    switch (g_riichs_ChStatus[p_riichs_info->ch_no])
    {
        case RIICHS_NO_INIT :

            /* It is necessary to reinitialize. */
            /* Sets the return value to uninitialized state. */
            ret = RIICHS_ERR_NO_INIT;
        break;

        case RIICHS_IDLE :
        case RIICHS_FINISH :
        case RIICHS_NACK :

            /* Checks the device status. */
            if ((((RIICHS_NO_INIT == p_riichs_info->dev_sts) || (RIICHS_IDLE == p_riichs_info->dev_sts))
                    || (RIICHS_FINISH == p_riichs_info->dev_sts)) || (RIICHS_NACK == p_riichs_info->dev_sts))
            {
                /* Sets the return value to success state. */
                ret = RIICHS_SUCCESS;
            }
            else if (RIICHS_COMMUNICATION == p_riichs_info->dev_sts)
            {
                /* Another device performed a driver reset processing
                 when this device is in idle state or communicating state. */
                /* It is necessary to reinitialize. */
                /* Sets the return value to bus busy state. */
                ret = RIICHS_ERR_BUS_BUSY;
            }
            else if (RIICHS_TMO == p_riichs_info->dev_sts)
            {
                /* The device is in error state. */
                /* Sets the return value to time out error state. */
                ret = RIICHS_ERR_TMO;
            }
            else if (RIICHS_AL == p_riichs_info->dev_sts)
            {
                /* The device is in error state. */
                /* Sets the return value to error state. */
                ret = RIICHS_ERR_AL;
            }
            else
            {
                ret = RIICHS_ERR_OTHER;
            }
        break;

        case RIICHS_COMMUNICATION :

            /* The device or another device is on communication. */
            /* Sets the return value to communication state. */
            ret = RIICHS_ERR_BUS_BUSY;
        break;

        case RIICHS_TMO :

            /* The channel is in error state. */
            /* Sets the return value to time out error state. */
            ret = RIICHS_ERR_TMO;
        break;

        case RIICHS_AL :

            /* The channel is in error state. */
            /* Sets the return value to error state. */
            ret = RIICHS_ERR_AL;
        break;

        default :
            ret = RIICHS_ERR_OTHER;
        break;
    }

    if (RIICHS_MSK_BFREE != ((*picbsr_reg) & RIICHS_MSK_BFREE))
    {
        ret = RIICHS_ERR_BUS_BUSY;
    }

    return ret;
} /* End of function riichs_check_chstatus_start() */

/***********************************************************************************************************************
 * Function Name: riichs_check_chstatus_advance
 * Description  : Check Channel Status Processing (at Advance Function).
 *                When calls the advance function, checks channel status.
 * Arguments    : riichs_info_t * p_riichs_info          ; IIC Information
 * Return Value : RIICHS_SUCCESS                   ; Successful operation
 *              : RIICHS_ERR_NO_INIT               ; Uninitialized state
 *              : RIICHS_ERR_AL                    ; Arbitration lost error
 *              : RIICHS_ERR_TMO                   ; Time out error
 *              : RIICHS_ERR_OTHER                 ; Other error
 **********************************************************************************************************************/
static riichs_return_t riichs_check_chstatus_advance (riichs_info_t * p_riichs_info)
{
    riichs_return_t ret;

    /* Checks the channel status. */
    switch (g_riichs_ChStatus[p_riichs_info->ch_no])
    {
        case RIICHS_NO_INIT :

            /* It is necessary to reinitialize. */
            /* Sets the return value to uninitialized state. */
            ret = RIICHS_ERR_NO_INIT;
        break;

        case RIICHS_IDLE :
        case RIICHS_FINISH :
        case RIICHS_NACK :

            /* Sets the return value to error state. */
            ret = RIICHS_ERR_OTHER;
        break;

        case RIICHS_COMMUNICATION :

            /* Checks the device status. */
            if ((((RIICHS_NO_INIT == p_riichs_info->dev_sts) || (RIICHS_IDLE == p_riichs_info->dev_sts))
                    || (RIICHS_FINISH == p_riichs_info->dev_sts)) || (RIICHS_NACK == p_riichs_info->dev_sts))
            {
                /* Sets the return value to idle state. */
                ret = RIICHS_ERR_OTHER;
            }
            else if (RIICHS_COMMUNICATION == p_riichs_info->dev_sts)
            {
                /* The device or another device is on communication. */
                /* Sets the return value to communication state. */
                ret = RIICHS_SUCCESS;
            }
            else if (RIICHS_TMO == p_riichs_info->dev_sts)
            {
                /* The device is in error state. */
                /* Sets the return value to time out error state. */
                ret = RIICHS_ERR_TMO;
            }
            else if (RIICHS_AL == p_riichs_info->dev_sts)
            {
                /* The device is in error state. */
                /* Sets the return value to error state. */
                ret = RIICHS_ERR_AL;
            }
            else
            {
                ret = RIICHS_ERR_OTHER;
            }
        break;

        case RIICHS_TMO :

            /* The channel is in time out error state. */
            /* Sets the return value to time out error state. */
            ret = RIICHS_ERR_TMO;
        break;

        case RIICHS_AL :

            /* The channel is in error state. */
            /* Sets the return value to error state. */
            ret = RIICHS_ERR_AL;
        break;

        default :
            ret = RIICHS_ERR_OTHER;
        break;
    }

    return ret;

} /* End of function riichs_check_chstatus_advance() */

/***********************************************************************************************************************
 * Function Name: riichs_enable
 * Description  : Enables IIC bus interrupt enable register.
 *              : Enables interrupt.
 *              : Enables IIC.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_enable (riichs_info_t * p_riichs_info)
{
    /* Initializes the IIC registers. */
    /* Includes I/O register read operation at the end of the following function. */
    riichs_init_io_register(p_riichs_info);

    /* Clears the interrupt request register. */
    riichs_clear_ir_flag(p_riichs_info);

    /* Sets the internal status. */
    riichs_api_status_set(p_riichs_info, RIICHS_STS_IDLE);

    /* Enables the interrupt. */
    riichs_int_enable(p_riichs_info);

    /* Set ICCR.ICE to 1. */
    riichs_set_iccrice (p_riichs_info);
} /* End of function riichs_enable() */

/***********************************************************************************************************************
 * Function Name: riichs_disable
 * Description  : Disables IIC.
 *              : Disables interrupt.
 *              : Initializes IIC bus interrupt enable register.
 *              : Sets internal status.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_disable (riichs_info_t * p_riichs_info)
{
    /* Disables IIC. */
    /* Sets SCLn and SDAn to non-driven state. */
    /* Resets all RIICHS registers and the internal status. (RIICHS reset) */
    riichs_all_reset(p_riichs_info);

    /* Sets the internal mode. */
    riichs_api_mode_event_init(p_riichs_info, RIICHS_MODE_NONE);

    /* Sets the internal status. */
    riichs_api_status_set(p_riichs_info, RIICHS_STS_NO_INIT);

    /* Disables the interrupt. */
    riichs_int_disable(p_riichs_info);

    /* Clears the RIICHS reset or internal reset. */
    riichs_reset_clear(p_riichs_info);
} /* End of function riichs_disable() */

/***********************************************************************************************************************
 * Outline      : Initialize IIC Driver Processing
 * Function Name: riichs_init_io_register
 * Description  : Initializes RIICHS register.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_init_io_register (riichs_info_t * p_riichs_info)
{
    volatile uint32_t uctmp = 0x00000000;

    /* Creates the register pointer for the specified RIICHS channel. */

    volatile uint32_t * const psar0_reg = RIICHS_SAR0_ADR;
    volatile uint32_t * const psar1_reg = RIICHS_SAR1_ADR;
    volatile uint32_t * const psar2_reg = RIICHS_SAR2_ADR;
    volatile uint32_t * const picscr_reg = RIICHS_ICSCR_ADR;
    volatile uint32_t * const pictor_reg = RIICHS_ICTOR_ADR;
    volatile uint32_t * const picicr_reg = RIICHS_ICICR_ADR;
    volatile uint32_t * const picfer_reg = RIICHS_ICFER_ADR;
    volatile uint32_t * const picser_reg = RIICHS_ICSER_ADR;
    volatile uint32_t * const piccser_reg = RIICHS_ICCSER_ADR;
    volatile uint32_t * const picackr_reg = RIICHS_ICACKR_ADR;
    volatile uint32_t * const piccscr_reg = RIICHS_ICCSCR_ADR;
    volatile uint32_t * const picocr_reg = RIICHS_ICOCR_ADR;

    /* Sets SCLn and SDAn to non-driven state. */
    /* Resets all RIICHS registers and the internal status. (RIICHS reset) */
    riichs_all_reset(p_riichs_info);

    /* Sets slave address format and slave address. */
    *psar0_reg = RIICHS_SAR0_INIT; /* Sets SARy.(y=0,1,2) */
    *psar1_reg = RIICHS_SAR1_INIT; /* Sets SARy.(y=0,1,2) */
    *psar2_reg = RIICHS_SAR2_INIT; /* Sets SARy.(y=0,1,2) */
    *picscr_reg = RIICHS_ICSCR_INIT; /* Sets ICSCR register. */

    /* Set SARy.FS bit */
    *psar0_reg = (uint32_t)((RIICHS_SAR0_INIT) | (RIICHS_SAR_10BIT_FORMAT_SET));

    uctmp = *psar0_reg; /* Reads SAR0. */
    uctmp = *picscr_reg; /* Reads ICSCR. */

    /* Initializes ICTOR,ICICR,ICOCR registers. */
    *pictor_reg = g_riichs_ictor_init[p_riichs_info->ch_no];
    *picicr_reg = g_riichs_icicr_init[p_riichs_info->ch_no];
    *picocr_reg = RIICHS_CH0_ICOCR_INIT;

    /* Sets a transfer clock. */
    /* Includes I/O register read operation at the end of the following function. */
    riichs_set_frequency(p_riichs_info); /* Sets ICRCCR.CKS[2:0] bit, ICFBR and ICHBR registers. */

    /* Initializes ICACKR and ICCSCR registers. */
    *picackr_reg = RIICHS_CH0_ICACKR_INIT;
    *piccscr_reg = RIICHS_CH0_ICCSCR_INIT;

    /* Disable interrupts each target MCU.  */
    riichs_mcu_int_disable(p_riichs_info->ch_no);

    /* Initializes ICFER register. */
    *picfer_reg = g_riichs_icfer_init[p_riichs_info->ch_no];
    uctmp = *picfer_reg; /* Reads ICFER. */

    /* Bus Free Time Setting */
    riichs_set_icbftr(p_riichs_info);

    /* Initializes ICSER register. */
    *picser_reg = g_riichs_icser_init[p_riichs_info->ch_no];
    uctmp = *picser_reg; /* Reads ICSER. */

    /* Initializes ICCSER register. */
    *piccser_reg = RIICHS_ICCSER_TDE_SET | RIICHS_ICCSER_RDE_SET;
    uctmp = *piccser_reg; /* Reads ICCSER. */
} /* End of function riichs_init_io_register() */

/***********************************************************************************************************************
 * Function Name: riichs_slv_addr_match_int_enable
 * Description  : Enable Slave Address Match Interrupt Processing.
 *                Enables slave address match interrupt.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_slv_addr_match_int_enable (riichs_info_t * p_riichs_info)
{
    volatile uint32_t uctmp = 0x00000000;
    volatile uint32_t temp_picscr_reg = 0x00000000;

    /* Creates the register pointer for the specified RIICHS channel. */
    volatile uint32_t * const psar0_reg = RIICHS_SAR0_ADR;
    volatile uint32_t * const psar1_reg = RIICHS_SAR1_ADR;
    volatile uint32_t * const psar2_reg = RIICHS_SAR2_ADR;
    volatile uint32_t * const picscr_reg = RIICHS_ICSCR_ADR;
    temp_picscr_reg = *picscr_reg;

    /* internal reset */
    riichs_reset_set(p_riichs_info);

    /* Enables slave address match interrupt. */
    if (1U == p_riichs_info->slave_addr0_format)
    {
        /* 7bit address format setting */
        *psar0_reg = (uint32_t)p_riichs_info->slave_addr0;
        temp_picscr_reg |= RIICHS_ICSCR_SAR0E_SET;
        (*picscr_reg) = temp_picscr_reg;
    }
    else if (2U == p_riichs_info->slave_addr0_format)
    {
        /* 10bit address format setting */
        *psar0_reg = (uint32_t)p_riichs_info->slave_addr0;
        temp_picscr_reg |= RIICHS_ICSCR_SAR0E_SET;
        (*picscr_reg) = temp_picscr_reg;
    }
    else
    {
        /* Disable slave address0 */
        temp_picscr_reg &= RIICHS_ICSCR_SAR0E_CLR;
        (*picscr_reg) = temp_picscr_reg;
    }

    if (1U == p_riichs_info->slave_addr1_format)
    {
        /* 7bit address format setting */
        *psar1_reg = (uint32_t)p_riichs_info->slave_addr1;
        temp_picscr_reg |= RIICHS_ICSCR_SAR1E_SET;
        (*picscr_reg) = temp_picscr_reg;
    }
    else if (2U == p_riichs_info->slave_addr1_format)
    {
        /* 10bit address format setting */
        *psar1_reg = (uint32_t)p_riichs_info->slave_addr1;
        temp_picscr_reg |= RIICHS_ICSCR_SAR1E_SET;
        (*picscr_reg) = temp_picscr_reg;
    }
    else
    {
        /* Disable slave address1 */
        temp_picscr_reg &= RIICHS_ICSCR_SAR1E_CLR;
        (*picscr_reg) = temp_picscr_reg;
    }

    if (1U == p_riichs_info->slave_addr2_format)
    {
        /* 7bit address format setting */
        *psar2_reg = (uint32_t)p_riichs_info->slave_addr2;
        temp_picscr_reg |= RIICHS_ICSCR_SAR2E_SET;
        (*picscr_reg) = temp_picscr_reg;
    }
    else if (2 == p_riichs_info->slave_addr2_format)
    {
        /* 10bit address format setting */
        *psar2_reg = (uint32_t)p_riichs_info->slave_addr2;
        temp_picscr_reg |= RIICHS_ICSCR_SAR2E_SET;
        (*picscr_reg) = temp_picscr_reg;
    }
    else
    {
        /* Disable slave address2 */
        temp_picscr_reg &= RIICHS_ICSCR_SAR2E_CLR;
        (*picscr_reg) = temp_picscr_reg;
    }

    if (1U == p_riichs_info->gca_enable)
    {
        /* General call address setting */
        temp_picscr_reg |= RIICHS_ICSCR_GCAE_SET;
        (*picscr_reg) = temp_picscr_reg;
    }
    else
    {
        /* Disable General call address */
        temp_picscr_reg &= RIICHS_ICSCR_GCAE_CLR;
        (*picscr_reg) = temp_picscr_reg;
    }
    uctmp = *picscr_reg; /* Reads ICSCR. */

    /* Clears internal reset. */
    riichs_reset_clear(p_riichs_info);
} /* End of function riichs_slv_addr_match_int_enable() */

/***********************************************************************************************************************
 * Function Name: riichs_int_enable
 * Description  : Enable Interrupt Processing.
 *                Clears interrupt request register. Enables interrupt.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_int_enable (riichs_info_t * p_riichs_info)
{
    /* Clears the interrupt request register. */
    riichs_clear_ir_flag(p_riichs_info);

    /* Enable interrupts each target MCU.  */
    riichs_mcu_int_enable(p_riichs_info->ch_no);
} /* End of function riichs_int_enable() */

/***********************************************************************************************************************
 * Function Name: riichs_int_disable
 * Description  : Disable Interrupt Processing.
 *                Disables interrupt. Sets interrupt source priority. Clears interrupt request register.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_int_disable (riichs_info_t * p_riichs_info)
{
    /* Disable interrupts each target MCU.  */
    riichs_mcu_int_disable(p_riichs_info->ch_no);

    /* Clears the interrupt request register. */
    /* Includes I/O register read operation at the end of the following function. */
    riichs_clear_ir_flag(p_riichs_info);
} /* End of function riichs_int_disable() */

/***********************************************************************************************************************
 * Function Name: riichs_int_icsier_iccsier_setting
 * Description  : Set ICSIER and ICCSIER Register Processing.
 *                Sets an interrupt condition of RIICHS.
 * Arguments    : riichs_info_t *  p_riichs_info     ; IIC Information
 : uint32_t        New_icsier        ; New ICSIER value
 : uint32_t        New_iccsier       ; New ICCSIER value
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_int_icsier_iccsier_setting (riichs_info_t * p_riichs_info, uint32_t New_icsier,uint32_t New_iccsier)
{
    /* Setting TMOIE bit of interrupt enable register. */
    riichs_mcu_int_icsier_iccsier_setting (p_riichs_info->ch_no, New_icsier, New_iccsier);

} /* End of function riichs_int_icsier_iccsier_setting() */

/***********************************************************************************************************************
 * Function Name: riichs_set_frequency
 * Description  : Set IIC Frequency Processing. Sets ICRCCR, ICFBR and ICHBR registers.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_set_frequency (riichs_info_t * p_riichs_info)
{
    riichs_bps_calc(p_riichs_info, g_riichs_bps[p_riichs_info->ch_no], true); /* Set BPS */

    if (1000 < g_riichs_bps[p_riichs_info->ch_no])
    {
        riichs_bps_calc(p_riichs_info, p_riichs_info->fs_speed_kbps, false); /* set bit rate before Hs-mode communication */
    }

} /* End of function riichs_set_frequency() */

/***********************************************************************************************************************
 * Function Name: riichs_check_bus_busy
 * Description  : Check Bus Busy Processing.
 *                When the bits of the RIICHS registers are the following set values, the bus is released.
 *              :   ICBSR.BFREE = 1 ; The I2C bus is released (bus free state).
 *              :   ICIMR.SCLI = 1 ; SCLn pin input is at a high level.
 *              :   ICIMR.SDAI = 1 ; SDAn pin input is at a high level.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : RIICHS_TRUE                      ; Bus ready
 *              : RIICHS_FALSE                     ; Bus busy
 **********************************************************************************************************************/
static bool riichs_check_bus_busy (riichs_info_t * p_riichs_info)
{
    volatile uint32_t cnt = 0x00000000;

    /* Creates the register pointer for the specified RIICHS channel. */
    volatile uint32_t * const picimr_reg = RIICHS_ICIMR_ADR;
    volatile uint32_t * const picbsr_reg = RIICHS_ICBSR_ADR;

    /* Checks bus busy. */
    /* WAIT_LOOP */
    for (cnt = p_riichs_info->bus_check_counter; cnt > 0x00000000; cnt--)
    {
        /* Bus busy? */
        if ((RIICHS_MSK_BFREE == ((*picbsr_reg) & RIICHS_MSK_BFREE)) && /* ICBSR.BFREE = 1 */
        (RIICHS_MSK_SCLSDA == ((*picimr_reg) & RIICHS_MSK_SCLSDA))) /* ICIMR.SCLI = 1, ICIMR.SDAI = 1 */
        {
            return RIICHS_TRUE; /* Bus ready */
        }
    }
    return RIICHS_FALSE; /* Bus busy */
} /* End of function riichs_check_bus_busy() */

/***********************************************************************************************************************
 * Function Name: riichs_start_cond_generate
 * Description  : Generate Start Condition Processing.
 *                Sets ICCGR.ST bit.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_start_cond_generate (riichs_info_t * p_riichs_info)
{
    volatile uint32_t uctmp = 0x00000000;
    volatile uint32_t temp_picsr2_reg = 0x00000000;

    /* Creates the register pointer for the specified RIICHS channel. */
    volatile uint32_t * const picsr2_reg = RIICHS_ICSR2_ADR;
    volatile uint32_t * const piccgr_reg = RIICHS_ICCGR_ADR;
    temp_picsr2_reg = *picsr2_reg;

    /* Clears start condition detection flag. */
    if (0x00000000 != (((*picsr2_reg) & RIICHS_ICSR2_START_SET)))
    {
        temp_picsr2_reg &= RIICHS_ICSR2_START_CLR;
        (*picsr2_reg) = temp_picsr2_reg;
    }

    /* Generates a start condition. */
    (*piccgr_reg) = RIICHS_ICCGR_ST; /* Sets ICCGR.ST bit. */
    uctmp = *piccgr_reg; /* Reads ICCGR. */
} /* End of function riichs_start_cond_generate() */

/***********************************************************************************************************************
 * Function Name: riichs_re_start_cond_generate
 * Description  : Generate Restart Condition Processing.
 *                Sets ICCGR.RS bit.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : None
 **********************************************************************************************************************/
static void  riichs_re_start_cond_generate (riichs_info_t * p_riichs_info)
{
    volatile uint32_t uctmp = 0x00000000;
    volatile uint32_t temp_picsr2_reg = 0x00000000;

    /* Creates the register pointer for the specified RIICHS channel. */
    volatile uint32_t * const picsr2_reg = RIICHS_ICSR2_ADR;
    volatile uint32_t * const piccgr_reg = RIICHS_ICCGR_ADR;
    temp_picsr2_reg = *picsr2_reg;

    /* Clears start condition detection flag. */
    if (0x00000000 != (((*picsr2_reg) & RIICHS_ICSR2_START_SET)))
    {
        temp_picsr2_reg &= RIICHS_ICSR2_START_CLR;
        (*picsr2_reg) = temp_picsr2_reg;
    }

    /* Generates a restart condition. */
    (*piccgr_reg) = RIICHS_ICCGR_RS; /* Sets ICCGR.RS bit. */
    uctmp = *piccgr_reg; /* Reads ICCGR. */


} /* End of function riichs_re_start_cond_generate() */

/***********************************************************************************************************************
 * Function Name: riichs_stop_cond_generate
 * Description  : Generate Stop Condition Processing.
 *                Clears ICSR2.STOP bit and sets ICCGR.SP bit.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_stop_cond_generate (riichs_info_t * p_riichs_info)
{
    volatile uint32_t uctmp = 0x00000000;
    volatile uint32_t temp_picsr2_reg = 0x00000000;

    /* Creates the register pointer for the specified RIICHS channel. */
    volatile uint32_t * const picsr2_reg = RIICHS_ICSR2_ADR;
    volatile uint32_t * const piccgr_reg = RIICHS_ICCGR_ADR;
    temp_picsr2_reg = *picsr2_reg;

    /* Clears stop condition detection flag. */
    if (0x00000000 != (((*picsr2_reg) & RIICHS_ICSR2_STOP_SET)))
    {
        temp_picsr2_reg &= RIICHS_ICSR2_STOP_CLR;
        (*picsr2_reg) = temp_picsr2_reg;
    }

    /* Generates a stop condition. */
    (*piccgr_reg) = RIICHS_ICCGR_SP; /* Sets ICCGR.SP bit. */
    uctmp = *piccgr_reg; /* Reads ICCGR. */
} /* End of function riichs_stop_cond_generate() */

/***********************************************************************************************************************
 * Function Name: riichs_set_sending_data
 * Description  : Transmit Data Processing.
 *                Sets transmission data to ICDRT register.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 *              : uint8_t *p_data                ; Transmitted data buffer pointer
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_set_sending_data (riichs_info_t * p_riichs_info, uint8_t *p_data)
{
    volatile uint32_t uctmp = 0x00000000;

    /* Creates the register pointer for the specified RIICHS channel. */
    volatile uint32_t * const picdr_reg = RIICHS_ICDR_ADR;

    /* Clears TXI interrupt request register. */
    /* For the measure to keep the IR flag for the TXI interrupt from stocking twice,               */
    /* when the IR flag is 1 before writing data to the data buffer in the TXI interrupt handling,  */
    /* the IR flag is cleared before transmission.                                                  */
    /* This issue happens, for example, when the following three all occur.                         */
    /*  - An AL occurred during MasterSend.                                                         */
    /*  - In the communication with an AL occurred, the RX device is specified with SlaveSend       */
    /*    by the communication target Master.                                                       */
    /*  - The TXI interrupt handling is in wait state after receiving the address due to            */
    /*    another interrupt execution, etc.                                                         */
    riichs_mcu_clear_ir_txi(p_riichs_info->ch_no);

    /* Sets the transmitting data. */
    *picdr_reg = *p_data; /* Writes data to RIICHS in order to transmit. */
    uctmp = *picdr_reg; /* Reads ICDR. */
} /* End of function riichs_set_sending_data() */

/***********************************************************************************************************************
 * Function Name: riichs_get_receiving_data
 * Description  : Receive Data Processing.
 *                Stores received data from ICDR register.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : Returns received data.
 **********************************************************************************************************************/
static uint32_t riichs_get_receiving_data (riichs_info_t * p_riichs_info)
{
    /* Creates the register pointer for the specified RIICHS channel. */
    volatile uint32_t * const picdr_reg = RIICHS_ICDR_ADR;

    /* Stores the received data. */
    return *picdr_reg;
} /* End of function riichs_get_receiving_data() */

/***********************************************************************************************************************
 * Function Name: riichs_receive_wait_setting
 * Description  : Receive "last byte - 2bytes" Setting Proccesing.
 *                Sets ICCSCR.WAITRE bit.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_receive_wait_setting (riichs_info_t * p_riichs_info)
{
    volatile uint32_t uctmp = 0x00000000;
    volatile uint32_t temp_piccscr_reg = 0x00000000;

    /* Creates the register pointer for the specified RIICHS channel. */
    volatile uint32_t * const piccscr_reg = RIICHS_ICCSCR_ADR;
    temp_piccscr_reg = *piccscr_reg;

    /* Sets ICCSCR.WAIT bit. */
    temp_piccscr_reg |= RIICHS_ICCSCR_WAITRE_SET;
    (*piccscr_reg) = temp_piccscr_reg;
    uctmp = *piccscr_reg; /* Reads ICCSCR. */
} /* End of function riichs_receive_wait_setting() */

/***********************************************************************************************************************
 * Function Name: riichs_receive_pre_end_setting
 * Description  : Receive "last byte - 1byte" Setting Proccesing.
 *                Sets ICACKR.ACKBT bit and ICCSCR.WAITAE bit.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_receive_pre_end_setting (riichs_info_t * p_riichs_info)
{
    volatile uint32_t uctmp = 0x00000000;
    volatile uint32_t temp_picackr_reg = 0x00000000;
    volatile uint32_t temp_piccscr_reg = 0x00000000;

    /* Creates the register pointer for the specified RIICHS channel. */
    volatile uint32_t * const piccscr_reg = RIICHS_ICCSCR_ADR;
    volatile uint32_t * const picackr_reg = RIICHS_ICACKR_ADR;
    temp_picackr_reg = *picackr_reg;
    temp_piccscr_reg = *piccscr_reg;

    /* Sets ICCSCR.WAITAE bit.*/
    temp_piccscr_reg |= RIICHS_ICCSCR_WAITAE_SET;
    (*piccscr_reg) = temp_piccscr_reg;

    /* Sets ICACKR.ACKBT bit. */
    temp_picackr_reg |= (RIICHS_ICACKR_ACKWP_SET | RIICHS_ICACKR_ACKBT_SET);
    (*picackr_reg) = temp_picackr_reg; /* Modification of the ACKBT bit is enabled.A 1 is sent as the acknowledge bit (NACK transmission). */
    temp_picackr_reg &= RIICHS_ICACKR_ACKWP_CLR; /* Modification of the ACKBT bit is disabled. */
    (*picackr_reg) = temp_picackr_reg;
    uctmp = *piccscr_reg; /* Reads ICCSCR. */
    uctmp = *picackr_reg; /* Reads ICACKR. */
} /* End of function riichs_receive_pre_end_setting() */

/***********************************************************************************************************************
 * Function Name: riichs_receive_end_setting
 * Description  : Receive End Setting Processing.
 *                Sets ICACKR.ACKBT bit and clears ICCSCR.WAIT bit.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_receive_end_setting (riichs_info_t * p_riichs_info)
{
    volatile uint32_t uctmp = 0x00000000;
    volatile uint32_t temp_picackr_reg = 0x00000000;
    volatile uint32_t temp_piccscr_reg = 0x00000000;

    /* Creates the register pointer for the specified RIICHS channel. */
    volatile uint32_t * const piccscr_reg = RIICHS_ICCSCR_ADR;
    volatile uint32_t * const picackr_reg = RIICHS_ICACKR_ADR;
    temp_picackr_reg = *picackr_reg;

    /* Sets ICACKR.ACKBT bit. */
    temp_picackr_reg |= (RIICHS_ICACKR_ACKWP_SET | RIICHS_ICACKR_ACKBT_SET); /* Modification of the ACKBT bit is enabled. */
    (*picackr_reg) = temp_picackr_reg;
    temp_picackr_reg &= RIICHS_ICACKR_ACKWP_CLR; /* Modification of the ACKBT bit is disabled. */
    (*picackr_reg) = temp_picackr_reg;

    /* Clears ICCSCR.WAIT bit. */
    temp_piccscr_reg &= RIICHS_ICCSCR_WAITRE_CLR;
    (*piccscr_reg) = temp_piccscr_reg;
    uctmp = *piccscr_reg; /* Reads ICCSCR. */
    uctmp = *picackr_reg; /* Reads ICACKR. */
} /* End of function riichs_receive_end_setting() */

/***********************************************************************************************************************
 * Function Name: riichs_reset_clear
 * Description  : Enable IIC Function Processing.
 *                Enables RIICHS communication and enables RIICHS multi-function pin controller.
 * Arguments    : riichs_info_t * p_riichs_info     ; IIC Information
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_reset_clear (riichs_info_t * p_riichs_info)
{
    volatile uint32_t uctmp = 0x00000000;
    volatile uint32_t temp_picrcr_reg = 0x00000000;

    /* Creates the register pointer for the specified RIICHS channel. */
    volatile uint32_t * const picrcr_reg = RIICHS_ICRCR_ADR;
    temp_picrcr_reg = *picrcr_reg;

    /* Enables RIICHS. */
    temp_picrcr_reg &= RIICHS_ICRCR_ISRST_CLR; /* Clears ICRCR.ISRST bit. */
    (*picrcr_reg) = temp_picrcr_reg;
    temp_picrcr_reg &= RIICHS_ICRCR_ENABLE; /* Clears ICRCR.MRST bit. */
    (*picrcr_reg) = temp_picrcr_reg;
    uctmp = *picrcr_reg; /* Reads ICRCR. */
} /* End of function riichs_reset_clear() */

/***********************************************************************************************************************
 * Function Name: riichs_set_iccrice
 * Description  : Set ICCR.ICE.
 * Arguments    : riichs_info_t * p_riichs_info     ; IIC Information
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_set_iccrice (riichs_info_t * p_riichs_info)
{
    volatile uint32_t uctmp = 0x00000000;

    /* Creates the register pointer for the specified RIICHS channel. */
    volatile uint32_t * const piccr_reg = RIICHS_ICCR_ADR;

    (*piccr_reg) = RIICHS_ICCR_ICE_SET; /* Set the ICCR.ICE bit to 1. */
    uctmp = *piccr_reg; /* Reads ICCR. */
} /* End of function riichs_set_iccrice() */

/***********************************************************************************************************************
 * Function Name: riichs_set_icbftr
 * Description  : Bus Free Time Setting.
 * Arguments    : riichs_info_t * p_riichs_info     ; IIC Information
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_set_icbftr (riichs_info_t * p_riichs_info)
{
    volatile uint32_t uctmp = 0x00000000;
    uint32_t temp_bus_free_time = p_riichs_info->bus_free_time;

    /* Creates the register pointer for the specified RIICHS channel. */
    volatile uint32_t * const picbftr_reg = RIICHS_ICBFTR_ADR;

    /* Set BUS free time. */
    (*picbftr_reg) = temp_bus_free_time; /* Set ICBFTR. */
    uctmp = *picbftr_reg; /* Reads ICBFTR. */
} /* End of function riichs_set_icbftr() */

/***********************************************************************************************************************
 * Function Name: riichs_reset_set
 * Description  : Disable IIC Function Processing.
 *                Disables RIICHS communication and disables RIICHS multi-function pin controller.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_reset_set (riichs_info_t * p_riichs_info)
{
    volatile uint32_t uctmp = 0x00000000;

    /* Creates the register pointer for the specified RIICHS channel. */
    volatile uint32_t * const picrcr_reg = RIICHS_ICRCR_ADR;

    /* Resets RIICHS registers. */
    (*picrcr_reg) = RIICHS_ICRCR_ISRST_SET; /* Sets ICRCR.ISRST bit. */
    uctmp = *picrcr_reg; /* Reads ICRCR. */
} /* End of function riichs_reset_set() */

/***********************************************************************************************************************
 * Function Name: riichs_all_reset
 * Description  : All reset IIC Function Processing.
 *                Resets all registers and internal states of the RIICHS.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_all_reset (riichs_info_t * p_riichs_info)
{
    volatile uint32_t uctmp = 0x00000000;
    volatile uint32_t temp_piccr_reg = 0x00000000;
    volatile uint32_t temp_picrcr_reg = 0x00000000;

    /* Creates the register pointer for the specified RIICHS channel. */
    volatile uint32_t * const piccr_reg = RIICHS_ICCR_ADR;
    volatile uint32_t * const picrcr_reg = RIICHS_ICRCR_ADR;
    temp_piccr_reg = *piccr_reg;
    temp_picrcr_reg = *picrcr_reg;

    /* Resets RIICHS registers. */
    temp_piccr_reg &= RIICHS_ICCR_ICE_CLR; /* Clear ICCR.ICE bit. */
    (*piccr_reg) = temp_piccr_reg;
    temp_picrcr_reg |= RIICHS_ICRCR_RIICHS_RESET; /* Sets ICRCR.MRST bit to 1. */
    (*picrcr_reg) = temp_picrcr_reg;

    /* Waiting for reset completion ICRCR.MRST bit = 0? */
    while (RIICHS_ICRCR_INIT != ((*picrcr_reg) & RIICHS_ICRCR_RIICHS_RESET))
    {
        /* Do Nothing */
    }

    uctmp = *picrcr_reg; /* Reads ICRCR. */
} /* End of function riichs_all_reset() */

/***********************************************************************************************************************
 * Function Name: riichs_clear_ir_flag
 * Description  : Clears Interrupt Request Flag Processing.
 *                Clears interrupt request register.
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 * Return Value : None
 **********************************************************************************************************************/
static void riichs_clear_ir_flag (riichs_info_t * p_riichs_info)
{
    uint8_t internal_flag = 0x00; /* Determines whether reinitialization is necessary. */
    volatile uint32_t uctmp = 0x00000000;

    /* Creates the register pointer for the specified RIICHS channel. */

    volatile uint32_t * const picrcr_reg = RIICHS_ICRCR_ADR;
    volatile uint32_t * const picsier_reg = RIICHS_ICSIER_ADR;
    volatile uint32_t * const piccsier_reg = RIICHS_ICCSIER_ADR;
    /* Checks IR flag. */
    /* If IR flag is set, clears IR flag. */
    if ((RIICHS_IR_SET == riichs_mcu_check_ir_txi(p_riichs_info->ch_no))
            || (RIICHS_IR_SET == riichs_mcu_check_ir_rxi(p_riichs_info->ch_no)))
    {
        /* Sets return value. */
        internal_flag = 0x01;
        /* sets ICRCR.MRST bit. */
        (*picrcr_reg) = RIICHS_ICRCR_RIICHS_RESET;
        /* WAIT_LOOP */
        while (RIICHS_ICRCR_RIICHS_RESET != ((*picrcr_reg) & RIICHS_ICRCR_RIICHS_RESET))
        {
            /* Do Nothing */
        }
    }

    /* Initializes ICSIER register. */
    *picsier_reg = RIICHS_ICSIER_INIT;
    /* WAIT_LOOP */
    while (RIICHS_ICSIER_INIT != (*picsier_reg))
    {
        /* Do Nothing */
    }

    /* Initializes ICCSIER register. */
    *piccsier_reg = RIICHS_ICCSIER_INIT;
    /* WAIT_LOOP */
    while (RIICHS_ICCSIER_INIT != (*piccsier_reg))
    {
        /* Do Nothing */
    }

    riichs_mcu_clear_ir_rxi(p_riichs_info->ch_no); /* Clears RXI interrupt request register. */
    riichs_mcu_clear_ir_txi(p_riichs_info->ch_no); /* Clears TXI interrupt request register. */

    /* Checks the need of the reinitialization. */
    if (0x01 == internal_flag)
    {
        internal_flag = 0x00;

        /* Reinitializes RIICHS register because cleared ICRCR.MRST bit. */
        /* Includes I/O register read operation at the end of the following function. */
        riichs_init_io_register(p_riichs_info);
        riichs_set_iccrice (p_riichs_info);
    }
}
 /* End of function riichs_clear_ir_flag() */

/***********************************************************************************************************************
 * Function Name: riichs_bps_calc
 * Description  : Calculation for ICBRH and ICBRL registers value
 *              :  and CKS bits value from setting bps of the argument.
 *              :
 * Arguments    : riichs_info_t * p_riichs_info      ; IIC Information
 *              : uint16_t kbps                  ; bit rate(kHz)
 *              : bool first_bps_set_flag         ; first bps set flag
 * Return Value : RIICHS_SUCCESS                   ; Successful operation, communication state
 *              : RIICHS_ERR_OTHER                 ; Other error
 **********************************************************************************************************************/
static riichs_return_t riichs_bps_calc (riichs_info_t * p_riichs_info, uint16_t kbps, bool first_bps_set_flag)
{
    volatile uint32_t uctmp = 0x00000000;
    volatile uint32_t temp = 0x00000000;
    uint32_t temp_replace = 0x00000000;
    volatile uint32_t nf = 0x00000000;
    uint32_t nf_replace = 0x00000000;
    volatile uint32_t * const picrccr_reg = RIICHS_ICRCCR_ADR;
    volatile uint32_t * const picfbr_reg = RIICHS_ICFBR_ADR;
    volatile uint32_t * const pichbr_reg = RIICHS_ICHBR_ADR;
    volatile uint32_t * const picicr_reg = RIICHS_ICICR_ADR;

    const uint8_t d_cks[RIICHS_MAX_DIV] =
    { 1, 2, 4, 8, 16, 32, 64, 128 }; /* divider array of RIICHS clock  */
    double pclk_val;

    volatile double bps = (double) (kbps * 1000);
    volatile double L_time; /* H Width period */
    volatile double H_time; /* L Width period */
    double temp_L_time;
    double temp_H_time;
    double bps_tmp;

    volatile double calc_val; /* Using for calculation buffer */

    double calc_val_tmp;

    double scl_up_time;
    double scl_down_time;
    volatile uint8_t cnt;

    /* before Hs-mode communication BPS check */
    if ( (false == first_bps_set_flag) && (kbps > RIICHS_FASTPLUS_SPPED_MAX) )
    { /* Cannot set bps */
        return RIICHS_ERR_OTHER;
    }

    pclk_val = riichs_mcu_check_freq(); /* Store pclk frequency */

    if (first_bps_set_flag)
    {
        /* Set Rise up time and down time */
        scl_up_time = p_riichs_info->scl_up_time;
        scl_down_time = p_riichs_info->scl_down_time;
    }
    else
    {
        /* set Rise up time and down time before Hs-mode communication */
        scl_up_time = p_riichs_info->fs_scl_up_time;
        scl_down_time = p_riichs_info->fs_scl_down_time;
    }

    /* Calculation for ICHBR and ICFBR registers value */

    /* Calculation L width time */
    L_time = (1 / (2 * bps)); /* Half period of frequency */
    H_time = L_time;

    if (kbps > RIICHS_FASTPLUS_SPPED_MAX)
    /* Check I2C mode of Speed */
    {
        L_time = (2 / (3 * bps)); /* period of frequency high:low = 1:2 */
        H_time = (1 / (3 * bps));
        /* 1000kbps over */
        /* Check L width */
        if (L_time < 160E-9)
        {
            /* Wnen L width less than 0.5us */
            /* Subtract Rise up and down time for SCL from H/L width */
            L_time = 160E-9;
            bps_tmp = bps;
            H_time = (((1 / bps_tmp) - L_time) - scl_up_time) - scl_down_time;
        }
        else
        {
            /* Subtract Rise up and down time for SCL from H/L width */
            L_time -= scl_down_time;
            H_time -= scl_up_time;
        }
    }
    else if (kbps > RIICHS_FAST_SPPED_MAX)
    {
        /* 400kbps over */
        /* Check L width */
        if (L_time < 0.5E-6)
        {
            /* Wnen L width less than 0.5us */
            /* Subtract Rise up and down time for SCL from H/L width */
            L_time = 0.5E-6;
            bps_tmp = bps;
            H_time = (((1 / bps_tmp) - L_time) - scl_up_time) - scl_down_time;
        }
        else
        {
            /* Subtract Rise up and down time for SCL from H/L width */
            L_time -= scl_down_time;
            H_time -= scl_up_time;
        }
    }
    else if (kbps > RIICHS_STAD_SPPED_MAX)
    {
        /* 100kbps over */
        /* Check L width */
        if (L_time < 1.3E-6)
        {
            /* Wnen L width less than 1.3us */
            /* Subtract Rise up and down time for SCL from H/L width */
            L_time = 1.3E-6;
            bps_tmp = bps;
            H_time = (((1 / bps_tmp) - L_time) - scl_up_time) - scl_down_time;
        }
        else
        {
            /* Subtract Rise up and down time for SCL from H/L width */
            L_time -= scl_down_time;
            H_time -= scl_up_time;
        }
    }
    else
    {
        L_time -= scl_down_time;
        H_time -= scl_up_time;
    }
    temp_L_time = L_time;
    temp_H_time = H_time;

    temp_replace = ICHBR_ICFBR_HIGH16_MASK;    /* ICHBR register or ICFBR register bit mask */

    /* if first_bps_set_flag true then ICRCCR.CKS bits set */
    if (first_bps_set_flag)
    {
        /* Calculating until calc_val is less than 256 */
        /* WAIT_LOOP */
        for (calc_val = 0xFFFF, cnt = 0; RIICHS_ICBR_MAX < (calc_val * (temp_L_time/temp_H_time)); cnt++)
        {
            calc_val = H_time; /* Set H width time */

            /* Check the range of divider of CKS */
            if (cnt >= RIICHS_MAX_DIV)
            {
                /* Cannot set bps */
                return RIICHS_ERR_OTHER;
            }

            calc_val_tmp = calc_val;
            calc_val = (calc_val_tmp / (d_cks[cnt] / pclk_val));
            calc_val = calc_val + 0.5; /* round off */
        }

        /* store ICRCCR value to avoid CKS bit. */
        *picrccr_reg = (uint32_t)((cnt - 1) & (BIT0 | BIT1 | BIT2)); /* Set ICRCCR.CKS bits.*/
    }
    else
    {
        /* Set H width to calc_val */
        cnt = ((*picrccr_reg) & 0x00000007) + 1;
        calc_val = H_time; /* Set H width time */
        calc_val_tmp = calc_val;
        calc_val = (calc_val_tmp / (d_cks[cnt - 1] / pclk_val));

        if (RIICHS_ICBR_MAX < (calc_val * (temp_L_time/temp_H_time)))
        {
            temp_replace |= 0x80000000; /* set ICFBR.DSBRPO */
            calc_val = calc_val / 2;
        }

        calc_val = calc_val + 0.5; /* round off */

        if (RIICHS_ICBR_MAX < calc_val)
        {
            calc_val = RIICHS_ICBR_MAX;
        }
    }

    if (first_bps_set_flag)
    {
        /* Apply noise filter stage  */
        if (kbps > RIICHS_FASTPLUS_SPPED_MAX)
        {
            nf_replace = (*picicr_reg & (0x0000000f))/4 + 1;
        }
        else
        {
            nf_replace = (*picicr_reg & (0x0000000f)) + 1;
        }
    }
    else
    {
        nf_replace = (*picicr_reg & (0x0000000f))/4 + 1;
    }

    /* Set value to ICHBR register OR ICFBR register HIGH[7:0] */
    if (3 > calc_val)
    {
        calc_val = 3;
    }
    if ((0 < nf_replace) && (((nf_replace + 1) * 2) > calc_val) )
    {
        calc_val = (nf_replace + 1) * 2;
    }
    temp_replace |= ((uint32_t)(((uint32_t)(calc_val - 1 - nf_replace)) & 0x000000ff) << 8);

    /* Set L width to calc_val */
    calc_val = L_time; /* Set L width */
    calc_val_tmp = calc_val;
    calc_val = (calc_val_tmp / (d_cks[cnt - 1] / pclk_val));

    if (0 < (temp_replace & 0x80000000))
    {
        calc_val = calc_val / 2;
    }

    calc_val = (uint16_t) (calc_val + 0.5); /* round off */

    if (RIICHS_ICBR_MAX < calc_val)
    {
        calc_val = RIICHS_ICBR_MAX;
    }

    /* If the calculated value is less than 1, it rounded up to 1. */

    /* Set value to ICHBR register OR ICFBR register LOW[7:0] */
    if (3 > calc_val)
    {
        calc_val = 3;
    }
    if ((0 < nf_replace) && (((nf_replace + 1) * 2) > calc_val) )
    {
        calc_val = (nf_replace + 1) * 2;
    }
    temp_replace |= ((uint32_t) (((uint32_t) (calc_val - 1 - nf_replace)) & 0x000000ff));

    if (kbps > RIICHS_FASTPLUS_SPPED_MAX)
    {
        /* set Hs-Mode Bit rate Register */
        *pichbr_reg = temp_replace;
    }
    else
    {
        /* set F/S-Mode Bit rate Register */
        *picfbr_reg = temp_replace;
    }
    uctmp = *pichbr_reg; /* dummy read */
    uctmp = *picfbr_reg; /* dummy read */

    return RIICHS_SUCCESS;

} /* End of function riichs_bps_calc() */

#if (RIICHS_CFG_CH0_INCLUDED == 1U)
/***********************************************************************************************************************
 * Function Name: riichs0_eei_sub
 * Description  : Interrupt EEI handler for channel 0.
 *                Types of interrupt requests transfer error or event generation.
 *                The event generations are arbitration-lost, NACK detection, timeout detection, 
 *                start condition detection, and stop condition detection.
 * Arguments    : None
 * Return Value : None
 **********************************************************************************************************************/
void riichs0_eei_sub (void)
{
    riichs_info_t * p_riichs_info;

    /* Checks Time Out. */
    if ((0U != RIICHS0.ICSR2.BIT.TMOF) && (0U != RIICHS0.ICSIER.BIT.TMOIE))
    {
        /* all interrupt disable */
        RIICHS0.ICSIER.BIT.TMOIE = 0U;
        /* WAIT_LOOP */
        while (0U != RIICHS0.ICSIER.BIT.TMOIE)
        {
            /* Do Nothing */
        }
        riichs_api_event[0] = RIICHS_EV_INT_TMO;
    }

    /* Checks Arbitration Lost. */
    if ((0U != RIICHS0.ICSR2.BIT.AL) && (0U != RIICHS0.ICSIER.BIT.ALIE))
    {
        RIICHS0.ICSIER.BIT.ALIE = 0U;
        /* WAIT_LOOP */
        while (0U != RIICHS0.ICSIER.BIT.ALIE)
        {
            /* Do Nothing */
        }
        riichs_api_event[0] = RIICHS_EV_INT_AL;
        if (RIICHS_MODE_S_READY == riichs_api_info[0].B_Mode)
        {
            riichs_set_ch_status(priichs_info_s[0], RIICHS_COMMUNICATION);
        }
    }

    /* Checks stop condition detection. */
    if ((0U != RIICHS0.ICSR2.BIT.STOP) && (0U != RIICHS0.ICSIER.BIT.SPIE))
    {
        if (((RIICHS_MODE_S_READY == riichs_api_info[0].N_Mode) || (RIICHS_MODE_S_SEND == riichs_api_info[0].N_Mode))
                || (RIICHS_MODE_S_RECEIVE == riichs_api_info[0].N_Mode))
        {
            /* Sets the internal status. */
            riichs_api_status_set(priichs_info_s[0], RIICHS_STS_SP_COND_WAIT);
        }

        /* Prohibits STOP interrupt. */
        RIICHS0.ICSIER.BIT.SPIE = 0U;

        RIICHS0.ICCSCR.BIT.WAITAE = 0U; /* Refer to the technical update. */
        RIICHS0.ICACKR.LONG = (( RIICHS0.ICACKR.LONG & RIICHS_ICACKR_ACKBT_CLR) | RIICHS_ICACKR_ACKWP_SET);
        RIICHS0.ICACKR.BIT.ACKWP = 0U; /* Refer to the technical update. */
        /* WAIT_LOOP */
        while ((0U != RIICHS0.ICCSCR.BIT.WAITAE) || (0U != RIICHS0.ICACKR.BIT.ACKBT))
        {
            /* Do Nothing */
        }

        /* Clears each status flag. */
        RIICHS0.ICSR2.BIT.STOP = 0U;
        /* WAIT_LOOP */
        while (0U != RIICHS0.ICSR2.BIT.STOP)
        {
            /* Do Nothing */
        }

        /* Sets event flag. */
        riichs_api_event[0] = RIICHS_EV_INT_STOP;
    }

    /* Checks NACK reception. */
    if ((0U != RIICHS0.ICSR2.BIT.NACKF) && (0U != RIICHS0.ICSIER.BIT.NAKIE))
    {
        /* Prohibits NACK interrupt to generate stop condition. */
        RIICHS0.ICSIER.BIT.NAKIE = 0U;

        /* Prohibits these interrupt. */
        /* After NACK interrupt, these interrupts will occur when they do not stop the following interrupts. */
        RIICHS0.ICSIER.BIT.TEIE = 0U;
        RIICHS0.ICCSIER.BIT.TIE = 0U;
        RIICHS0.ICCSIER.BIT.RIE = 0U;
        /* WAIT_LOOP */
        while (((0U != RIICHS0.ICSIER.BIT.TEIE) || (0U != RIICHS0.ICCSIER.BIT.TIE)) || (0U != RIICHS0.ICCSIER.BIT.RIE))
        {
            /* Do Nothing */
        }

        if (0U == RIICHS0.ICMMR.BIT.TRS)
        {
            riichs_mcu_clear_ir_rxi(0);
        }

        /* Sets event flag. */
        riichs_api_event[0] = RIICHS_EV_INT_NACK;
    }

    /* Checks start condition detection. */
    if ((0U != RIICHS0.ICSR2.BIT.START) && (0U != RIICHS0.ICSIER.BIT.STIE))
    {
        /* Disable start condition detection interrupt. */
        /* Clears status flag. */
        RIICHS0.ICSIER.BIT.STIE = 0U;
        RIICHS0.ICSR2.BIT.START = 0U;

        /* WAIT_LOOP */
        while ((0U != RIICHS0.ICSR2.BIT.START) || (0U != RIICHS0.ICSIER.BIT.STIE))
        {
            /* Do Nothing */
        }

        if (( RIICHS_STS_IDLE == riichs_api_info[0].B_status) && (1000 < g_riichs_bps[0]) )
        {
            /* Sets the internal status. */
            riichs_api_status_set(priichs_info_m[0], RIICHS_STS_MASTER_CODE_WAIT);
            RIICHS0.ICSIER.BIT.STIE = 1U;
        }

        riichs_api_event[0] = RIICHS_EV_INT_START;
    }

    switch (riichs_api_info[0].N_Mode)
    {
        case RIICHS_MODE_M_SEND :
        case RIICHS_MODE_M_RECEIVE :
        case RIICHS_MODE_M_SEND_RECEIVE :

            /* Master mode data */
            p_riichs_info = priichs_info_m[0];
        break;

        case RIICHS_MODE_S_READY :
        case RIICHS_MODE_S_SEND :
        case RIICHS_MODE_S_RECEIVE :

            /* Slave mode data */
            p_riichs_info = priichs_info_s[0];
        break;

        default :

            /* Internal error */
            return;
        break;
    }

    r_riichs_advance(p_riichs_info); /* Calls advance function */
} /* End of function riichs0_eei_sub() */

/***********************************************************************************************************************
 * Function Name: riichs0_txi_sub
 * Description  : Interrupt TXI handler for channel 0.
 *                Types of interrupt requests transmission data empty.
 * Arguments    : None
 * Return Value : None
 **********************************************************************************************************************/
void riichs0_txi_sub (void)
{
    uint32_t tmp;

    /* TXI processing is performed only when the following conditions are all met.                 */
    /*  - The address part of data has been received and the received address matches the RX's     */
    /*    own address.                                                                             */
    /*  - The RX device is in slave mode.                                                          */
    /*  - The RX device is in transmission mode (the R/W bit received is 'R' (slave transmission)) */
    tmp = RIICHS0.ICMMR.LONG;
    if ((RIICHS_ICMMR_MST_SET != (tmp & RIICHS_ICMMR_MST)) && (RIICHS_ICMMR_TRS_SET == (tmp & RIICHS_ICMMR_TRS)))
    {
        /* Processing when the addresses match (slave send specified) */

        /* Determines whether the state is arbitration-lost detected.                           */
        /* When an arbitration-lost is detected, the riichs_after_receive_slvadr function is not  */
        /* executed. Thus the internal status needs to be changed to "RIICHS_STS_SEND_DATA_WAIT". */
        if (RIICHS_STS_AL == riichs_api_info[0].N_status)
        {
            /* Processing when an arbitration-lost is detected. */
            /* Sets the internal status. */
            riichs_api_status_set(priichs_info_s[0], RIICHS_STS_SEND_DATA_WAIT);
        }

        /* Determines if the slave transmission is the first slave transmission.             */
        /* N_mode is updated from "RIICHS_MODE_S_READY" to "RIICHS_MODE_S_SEND" on the first TXI */
        /* interrupt after the slave transmission is determined.                             */
        if (RIICHS_MODE_S_READY == riichs_api_info[0].N_Mode)
        {
            /* Processing for the first slave transmission */
            /* Sets the internal mode. */
            riichs_api_info[0].N_Mode = RIICHS_MODE_S_SEND; /* Set slave transmission mode. */
        }

        /* Sets event. */
        switch (riichs_api_info[0].N_status)
        {
            case RIICHS_STS_IDLE_EN_SLV :

                /* Updates the channel status. */
                riichs_set_ch_status(priichs_info_s[0], RIICHS_COMMUNICATION);

                /* Sets interrupted address sending. */
                riichs_api_event[0] = RIICHS_EV_INT_ADD;

            break;

            case RIICHS_STS_SEND_DATA_WAIT :

                /* Sets interrupted data sending. */
                riichs_api_event[0] = RIICHS_EV_INT_SEND;

            break;

            default :

                /* Does nothing. */
            break;

        }

        r_riichs_advance(priichs_info_s[0]); /* Calls advance function */

    }
} /* End of function riichs0_txi_sub() */

/***********************************************************************************************************************
 * Function Name: riichs0_rxi_sub
 * Description  : Interrupt RXI handler for channel 0.
 *                Types of interrupt requests reception end.
 * Arguments    : None
 * Return Value : None
 **********************************************************************************************************************/
void riichs0_rxi_sub (void)
{
    riichs_info_t * p_riichs_info;

    if (RIICHS_STS_AL == riichs_api_info[0].N_status)
    {
        /* Sets the internal status. */
        riichs_api_status_set(priichs_info_s[0], RIICHS_STS_IDLE_EN_SLV);
    }

    if (RIICHS_MODE_S_READY == riichs_api_info[0].N_Mode)
    {
        /* Sets the internal mode. */
        riichs_api_info[0].N_Mode = RIICHS_MODE_S_RECEIVE;
    }

    if ((RIICHS_STS_IDLE_EN_SLV == riichs_api_info[0].N_status))
    {
        /* Updates the channel status. */
        riichs_set_ch_status(priichs_info_s[0], RIICHS_COMMUNICATION);
    }

    switch (riichs_api_info[0].N_Mode)
    {
        case RIICHS_MODE_M_RECEIVE :
        case RIICHS_MODE_M_SEND_RECEIVE :

            /* Master mode data */
            p_riichs_info = priichs_info_m[0];
        break;

        case RIICHS_MODE_S_RECEIVE :

            /* Slave mode data */
            p_riichs_info = priichs_info_s[0];
        break;

        default :

            /* Internal error */
            return;
        break;
    }

    /* Sets interrupted data receiving. */
    riichs_api_event[0] = RIICHS_EV_INT_RECEIVE;

    r_riichs_advance(p_riichs_info); /* Calls advance function */
} /* End of function riichs0_rxi_sub() */

/***********************************************************************************************************************
 * Function Name: riichs0_tei_sub
 * Description  : Interrupt TEI handler for channel 0.
 *                Types of interrupt requests transmission end.
 * Arguments    : None
 * Return Value : None
 **********************************************************************************************************************/
void riichs0_tei_sub (void)
{
    /* Clears ICSR2.TEND. */
    RIICHS0.ICSR2.BIT.TEND = 0U;
    /* WAIT_LOOP */
    while (0U != RIICHS0.ICSR2.BIT.TEND)
    {
        /* Do Nothing */
    }

    /* Sets event. */
    switch (riichs_api_info[0].N_status)
    {

        case RIICHS_STS_MASTER_CODE_WAIT :
            /* Sets interrupted data sending. */
            riichs_api_status_set(priichs_info_m[0], RIICHS_STS_ST_COND_WAIT);
            riichs_re_start_cond_generate(priichs_info_m[0]);

        break;

        case RIICHS_STS_SEND_SLVADR_W_WAIT :
        case RIICHS_STS_SEND_SLVADR_R_WAIT :

            /* Sets interrupted address sending. */
            riichs_api_event[0] = RIICHS_EV_INT_ADD;

        break;

        case RIICHS_STS_SEND_DATA_WAIT :

            /* Sets interrupted data sending. */
            riichs_api_event[0] = RIICHS_EV_INT_SEND;

        break;

        default :

            /* Does nothing. */
        break;

    }

    r_riichs_advance(priichs_info_m[0]); /* Calls advance function */
} /* End of function riichs0_tei_sub() */
#endif
