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
* File Name    : r_rspia_rx.c
* Device(s)    : RX Family
* Tool-Chain   : Renesas RX
*                GCC for Renesas RX
*                IAR C/C++ Compiler for Renesas RX
* OS           : None
* H/W Platform :
* Description  : Functions for using RSPIA on RX devices.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 31.03.2021 1.00     First Release
*                               Supported for RX671.
*         : 15.08.2022 1.30     Added RX26T support.
*                               Fixed to comply GSCE coding standard revision 6.5.0.
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Defines for RSPIA support */
#include "platform.h"
#include "r_rspia_rx_if.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define RSPIA_POWER_ON  (0)
#define RSPIA_POWER_OFF (1)

#if RSPIA_CFG_USE_CH0
#else
    #error "ERROR in r_rspia_rx configuration. Must enable at least 1 channel for use."
#endif

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Values will be used as bit flags.*/
typedef enum
{
    RSPIA_DO_TX    = 0x1,
    RSPIA_DO_RX    = 0x2,
    RSPIA_DO_TX_RX = 0x3
} rspia_operation_t;

typedef struct rspia_tcb_s
{
    void            * p_src;
    void            * p_dst;
    uint16_t          tx_count;
    uint16_t          rx_count;
    uint16_t          xfr_length;
    uint8_t           bytes_per_transfer;      /* Source buffer bytes per transfer: 1, 2, or 4. */
    uint8_t           tx_dflt_thresh;          /* TX FIFO threshold(default) */
    uint8_t           rx_dflt_thresh;          /* RX FIFO threshold(default) */
    uint8_t           tx_curr_thresh;          /* TX FIFO threshold(current) */
    uint8_t           rx_curr_thresh;          /* RX FIFO threshold(current) */
    bool              tx_dummy;                /* State flag for using dummy data in receive only. */
    bool              idle;                    /* State flag for transmit data empty. */
    rspia_operation_t transfer_mode; /* Transmit only, receive only, or transmit-receive. */
} rspia_tcb_t;

/* Driver internal shadow copy of register settings. */
typedef struct rspia_ctrl_reg_vals_s
{
    uint32_t spcr_val;    /* RSPIA Control Register (SPCR). */
    uint8_t  sslp_val;    /* RSPIA Slave Select Polarity Register (SSLP) */
    uint8_t  sppcr_val;   /* RSPIA Pin Control Register (SPPCR) */
    uint8_t  spscr_val;   /* RSPIA Sequence Control Register (SPSCR) */
    uint16_t spdcr_val;   /* RSPIA Data Control Register (SPDCR) */
    uint8_t  spckd_val;   /* RSPIA Clock Delay Register (SPCKD) */
    uint8_t  sslnd_val;   /* RSPIA Slave Select Negation Delay Register (SSLND)*/
    uint8_t  spnd_val;    /* RSPIA Next-Access Delay Register (SPND) */
} rspia_ctrl_reg_values_t;

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
/* Used to prevent having duplicate code for each channel. This only works if the channels are identical (just at
   different locations in memory). This is easy to tell by looking in iodefine.h and seeing if the same structure
   was used for all channels. */
volatile struct st_rspia R_BSP_EVENACCESS_SFR * g_rspia_channels[RSPIA_NUM_CH] =
{
    /* Initialize the array for up to x channels. Add more as needed. */
#if RSPIA_CFG_USE_CH0
    /* Pointer to RSPIA0 structure address */
    &RSPIA0,
#else
    NULL,
#endif
};

/* Array of channel handles. One for each physical RSPIA channel on the device. */
static struct rspia_cfg_block_s g_rspia_handles[RSPIA_NUM_CH];

/* Allocate transfer control blocks for all channels. */
static struct rspia_tcb_s g_rspia_tcb[RSPIA_NUM_CH] = {0};

/* Allocate transaction result code storage for all channels. */
static rspia_callback_data_t g_rspia_cb_data[RSPIA_NUM_CH] = {0};

/* Allocate register settings structure for all channels and initialize to defaults. */
static rspia_ctrl_reg_values_t g_ctrl_reg_values[] =
{
    RSPIA_SPCR_DEF,     /* Control Register (SPCR) */
    RSPIA_SSLP_DEF,     /* Slave Select Polarity Register (SSLP) */
    RSPIA_SPPCR_DEF,    /* Pin Control Register (SPPCR) */
    RSPIA_SPSCR_DEF,    /* Sequence Control Register (SPSCR) */
    RSPIA_SPDCR_DEF,    /* Data Control Register (SPDCR) */
    RSPIA_SPCKD_DEF,    /* Clock Delay Register (SPCKD) */
    RSPIA_SSLND_DEF,    /* Slave Select Negation Delay Register (SSLND) */
    RSPIA_SPND_DEF,     /* Next-Access Delay Register (SPND) */
};

/**********************************************************************************************************************
Private function declarations
 *********************************************************************************************************************/
/* Initializes the associated registers. */
static void rspia_regs_init (uint8_t chan);

/* Set the associated registers. */
static rspia_err_t rspia_regs_set  (uint8_t                   chan,
                                    rspia_chan_settings_t   * p_cfg,
                                    rspia_command_word_t      p_cmd,
                                    rspia_ctrl_reg_values_t * p_setreg);

#if RSPIA_CFG_PARAM_CHECKING_ENABLE
/* Check valid channel functions. */
static rspia_err_t rspia_chan_check (uint8_t chan);
#endif

/* Common routine used by RSPIA API write or read functions. */
static rspia_err_t rspia_write_read_common (rspia_hdl_t          hdl,
                                            rspia_command_word_t p_cmd,
                                            void               * p_src,
                                            void               * p_dst,
                                            uint16_t             length,
                                            rspia_operation_t    tx_rx_mode);

/* Sets the baud rate registers for a given frequency. */
static uint32_t rspia_baud_set (uint8_t chan, uint32_t baud_target);

/* Gets the threshold values of RSPIA channel used. */
static void rspia_get_thresh_vals (uint8_t chan);

/* Determines the primitive data type required for accessing a given RSPIA data frame bit length. */
static uint8_t rspia_get_data_type (rspia_command_word_t p_cmd_frame);

/* Common RSPIA channel power-on-off utility. */
static void power_on_off (uint8_t chan, uint8_t on_or_off);

/* Set RSPIA interrupt priorities. */
static void rspia_ir_priority_set (uint8_t chan, uint8_t priority);

/* Clear any pending RSPIA interrupts. */
static void rspia_interrupts_clear (uint8_t chan);

/* Disable or enable RSPIA interrupts. */
static void rspia_interrupts_set (uint8_t chan, bool enabled);

/* RSPIA transmit data to TX FIFO buffer. */
static void rspia_transmit (uint8_t chan);

/* RSPIA receive data from RX FIFO buffer. */
static void rspia_receive (uint8_t chan);

/* Error interrupt handler function. */
void rspia_spei_grp_isr (void * pdata);

/* Common interrupt handler for SPRI */
void rspia_rx_common (uint8_t chan);

/* Common interrupt handler for SPTI */
void rspia_tx_common (uint8_t chan);

/**********************************************************************************************************************
 * Function Name: R_RSPIA_Open
 *****************************************************************************************************************/ /**
 * @brief     This function applies power to the RSPIA channel, initializes the associated registers,
 *            enables interrupts, and provides the channel handle for use with other API functions.
 * @param[in] chan
 *            Number of the RSPIA channel to be initialized.
 * @param[in] *p_cfg
 *            Pointer to RSPIA channel configuration data structure.
 * @param[in] p_cmd
 *            SPCMD command data structure.
 * @param[in] (*p_callback)(void*p_cbdat)
 *            Pointer to user defined function called from interrupt.
 * @param[in] *p_hdl
 *            Pointer to a handle for channel. Handle value will be set by this function.
 * @retval    RSPIA_SUCCESS
 *             Successful; channel initialized
 * @retval    RSPIA_ERR_BAD_CHAN
 *             Channel number is not available
 * @retval    RSPIA_ERR_OMITTED_CHAN
 *             RSPIA_USE_CHx is 0 in config.h
 * @retval    RSPIA_ERR_CH_NOT_CLOSED
 *             Channel currently in operation; Perform R_RSPIA_Close() first
 * @retval    RSPIA_ERR_NULL_PTR
 *             *p_cfg pointer or *p_hdl pointer is NULL
 * @retval    RSPIA_ERR_ARG_RANGE
 *             An element of the *p_cfg structure contains an invalid value.
 * @retval    RSPIA_ERR_INVALID_ARG
 *             Argument is not valid for parameter.
 * @retval    RSPIA_ERR_LOCK
 *             The lock could not be acquired.
 * @details   The Open function is responsible for preparing an RSPIA channel for operation. This function must be called
 *            once prior to calling any other RSPIA API functions (except R_RSPIA_GetVersion). Once successfully completed,
 *            the status of the selected RSPIA will be set to "open". After that, this function should not be called
 *            again for the same RSPIA channel without first performing a "close" by calling R_RSPIA_Close().
 * @note      None.
 */
rspia_err_t    R_RSPIA_Open(uint8_t                     chan,
                            rspia_chan_settings_t     * p_cfg,
                            rspia_command_word_t        p_cmd,
                            void   (*p_callback)(void * p_cbdat),
                            rspia_hdl_t               * p_hdl)
{
    rspia_err_t err = RSPIA_SUCCESS;

    rspia_ctrl_reg_values_t * p_setregs = &(g_ctrl_reg_values[chan]);

    #if RSPIA_CFG_REQUIRE_LOCK
    bool        lock_result = false;
    #endif

    #if RSPIA_CFG_PARAM_CHECKING_ENABLE
    /* Check channel number. */
    err = rspia_chan_check(chan);
    if (RSPIA_SUCCESS != err)
    {
        return err;
    }

    /*Check argument */
    if ((NULL == p_cfg) || (NULL == p_hdl))
    {
        return RSPIA_ERR_NULL_PTR;
    }

    /* Check to see if the peripheral has already been initialized. */
    if (true == g_rspia_handles[chan].rspia_chan_opened)
    {
        return RSPIA_ERR_CH_NOT_CLOSED; /* This channel has already been initialized. */
    }
    #endif

    #if RSPIA_CFG_REQUIRE_LOCK
    /* Attempt to acquire lock for this RSPI channel. Prevents reentrancy conflict. */
    lock_result = R_BSP_HardwareLock((mcu_lock_t)(BSP_LOCK_RSPIA0 + chan));

    if (false == lock_result)
    {
        return RSPIA_ERR_LOCK; /* The open function is currently locked. */
    }
    #endif

    power_on_off(chan, RSPIA_POWER_ON);

    if (RSPIA_CH0 == chan)
    {
        rspia_ir_priority_set(chan, RSPIA_CFG_CH0_IR_PRIORITY);
    }
    else
    {
        /* Nothing else. */
        R_BSP_NOP();
    }

    /* Disable interrupts in ICU. */
    rspia_interrupts_set(chan, false);

    /* Initializes the associated registers of RSPIA */
    rspia_regs_init(chan);

    /* Set the base bit rate. Modifies the SPBR register setting with requested baud rate.*/
    if (0 == rspia_baud_set(chan, p_cfg->bps_target))
    {
        /* Failed */
        #if RSPIA_CFG_REQUIRE_LOCK
        /* Casting to mcu_lock_t type is valid */
        R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_RSPIA0 + chan));
        #endif
        return RSPIA_ERR_ARG_RANGE; /* Could not calculate settings for the requested baud rate. */
    }

    /* Get the threshold values of RSPIA channel used */
    rspia_get_thresh_vals(chan);

    /* Set the associated registers of RSPIA */
    err = rspia_regs_set(chan, p_cfg, p_cmd, p_setregs);
    if (RSPIA_SUCCESS != err)
    {
        /* Failed */
        #if RSPIA_CFG_REQUIRE_LOCK
        /* Casting to mcu_lock_t type is valid */
        R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_RSPIA0 + chan));
        #endif
        return err;
    }

    /* Peripheral Initialized */
    g_rspia_handles[chan].current_slave     = (uint8_t)p_cmd.ssl_assert;
    g_rspia_handles[chan].rspia_chan_opened = true;
    g_rspia_handles[chan].p_callback        = p_callback;
    g_rspia_handles[chan].chan              = chan;

    *p_hdl = &(g_rspia_handles[chan]);     /* Return a pointer to the channel handle structure. */

    #if RSPIA_CFG_REQUIRE_LOCK
    /* Release lock for this channel. */
    R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_RSPIA0 + chan));
    #endif

    return err;
}
/**********************************************************************************************************************
 End of function R_RSPIA_Open
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: rspia_regs_init
 * Description  : This function initializes the associated registers.
 * Arguments    : chan
 *                  Number of the RSPIA channel to be initialized.
 * Return Value : None
***********************************************************************************************************************/
static void rspia_regs_init(uint8_t chan)
{

    /* SPCKD register initialize */
    (*g_rspia_channels[chan]).SPCKD.BYTE = 0x0U;

    /* SSLND register initialize */
    (*g_rspia_channels[chan]).SSLND.BYTE = 0x0U;

    /* SPND register initialize */
    (*g_rspia_channels[chan]).SPND.BYTE = 0x0U;

    /* SPPCR register initialize */
    (*g_rspia_channels[chan]).SPPCR.BYTE = 0x0U;

    /* SSLP register initialize. */
    (*g_rspia_channels[chan]).SSLP.BYTE = 0x0U;

    /* SPBR register initialize. */
    (*g_rspia_channels[chan]).SPBR = 0xFF;

    /* SPSCR register initialize. */
    (*g_rspia_channels[chan]).SPSCR.BYTE = 0x0U;

    /* SPDCR register initialize */
    (*g_rspia_channels[chan]).SPDCR.WORD = 0x0U;

    /* SPFCR register initialize */
    (*g_rspia_channels[chan]).SPFCR.BIT.TTRG = 0;
    (*g_rspia_channels[chan]).SPFCR.BIT.RTRG = 0;

    /* SPCMD0 command register initialize. */
    (*g_rspia_channels[chan]).SPCMD0.LONG = 0x0U;

    /* SPCR register initialize */
    (*g_rspia_channels[chan]).SPCR.LONG = 0x0U;

}
/**********************************************************************************************************************
 End of function rspia_regs_init
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: rspia_regs_set
 * Description  : This function set the associated registers.
 * Arguments    : chan
 *                  Number of the RSPIA channel to be initialized.
 *                *p_cfg
 *                  Pointer to RSPIA channel configuration data structure.
 *                p_cmd
 *                  SPCMD command data structure.
 *                *p_setregs
 *                  Pointer to allocate register settings values for channels and initialize to defaults.
 * Return Value : None
 *                RSPIA_SUCCESS
 *                  Successful; channel initialized
 *                RSPIA_ERR_ARG_RANGE
 *                  An element of the *p_cfg and p_cmd structure contains an invalid value.
***********************************************************************************************************************/
static  rspia_err_t  rspia_regs_set(uint8_t                  chan,
                                    rspia_chan_settings_t  * p_cfg,
                                    rspia_command_word_t     p_cmd,
                                    rspia_ctrl_reg_values_t *p_setregs)
{
    rspia_err_t err = RSPIA_SUCCESS;

    /* Check arguments. */

    #if RSPIA_CFG_PARAM_CHECKING_ENABLE
    if (((RSPIA_MS_MODE_MASTER != p_cfg->master_slave_mode) && (RSPIA_MS_MODE_SLAVE != p_cfg->master_slave_mode))
          || ((RSPIA_IF_MODE_4WIRE != p_cfg->gpio_ssl) && (RSPIA_IF_MODE_3WIRE != p_cfg->gpio_ssl)))
    {
        return RSPIA_ERR_ARG_RANGE;
    }

    if (RSPIA_IF_MODE_4WIRE == p_cfg->gpio_ssl)
    {

        /* Check Frame format */
        if ((RSPIA_IF_FRAME_MOTOROLA_SPI != p_cfg->frame_mode) && (RSPIA_IF_FRAME_TI_SSP != p_cfg->frame_mode))
        {
            return RSPIA_ERR_ARG_RANGE;
        }

        /* In TI SSP mode, setting the SPCMDm.CPHA bit to 0 is not effective. */
        if ((RSPIA_IF_FRAME_TI_SSP == p_cfg->frame_mode) && (RSPIA_SPCMD_CPHA_SAMPLE_ODD == p_cmd.cpha))
        {
                return RSPIA_ERR_ARG_RANGE;
        }

        /* Check slave select to be asserted */
        if (RSPIA_SPCMD_ASSERT_SSL3 < (uint8_t)p_cmd.ssl_assert)
        {
            return RSPIA_ERR_ARG_RANGE;
        }
    }
    else
    {
        /* If CPHA bit is set to 0 for clock synchronous operation in slave mode (SPCR.MSTR = 0),
         * subsequent operation is not guaranteed. */
        if ((RSPIA_MS_MODE_SLAVE == p_cfg->master_slave_mode) && (RSPIA_SPCMD_CPHA_SAMPLE_ODD == p_cmd.cpha))
        {
            return RSPIA_ERR_ARG_RANGE;
        }
    }

    /* Check frame data length. */
    if ((RSPIA_SPCMD_BIT_LENGTH_4 > (uint8_t)p_cmd.bit_length) || (RSPIA_SPCMD_BIT_LENGTH_32 < (uint8_t)p_cmd.bit_length))
    {
        return RSPIA_ERR_ARG_RANGE;
    }
    #endif /* RSPIA_CFG_PARAM_CHECKING_ENABLE */

    /* Set pin control register (SPPCR) */
    (*g_rspia_channels[chan]).SPPCR.BYTE = (uint8_t)(p_setregs->sppcr_val & RSPIA_SPPCR_MASK);

    /* Set slave select polarity register (SSLP). */
    (*g_rspia_channels[chan]).SSLP.BYTE = (uint8_t)(p_setregs->sslp_val & RSPIA_SSLP_MASK);

    /* Set RSPIA data control register (SPDCR) */
    (*g_rspia_channels[chan]).SPDCR.WORD = (uint16_t)(p_setregs->spdcr_val & RSPIA_SPDCR_MASK);

    /* Set RSPIA clock delay registers (SPCKD) */
    (*g_rspia_channels[chan]).SPCKD.BYTE = (uint8_t)(p_setregs->spckd_val & RSPIA_SPCKD_MASK);

    /* Set RSPIA slave select negation delay register (SSLND) */
    (*g_rspia_channels[chan]).SSLND.BYTE = (uint8_t)(p_setregs->sslnd_val & RSPIA_SSLND_MASK);

    /* Set RSPIA next-access delay register (SPND) */
    (*g_rspia_channels[chan]).SPND.BYTE = (uint8_t)(p_setregs->spnd_val & RSPIA_SPND_MASK);

    /* Set RSPIA Sequence Control Register (SPSCR) */
    (*g_rspia_channels[chan]).SPSCR.BYTE = (uint8_t)(p_setregs->spscr_val & RSPIA_SPSCR_MASK);

    /* Set TX/RX FIFO threshold initial value (SPFCR) */
    (*g_rspia_channels[chan]).SPFCR.BIT.TTRG = g_rspia_tcb[chan].tx_dflt_thresh;
    (*g_rspia_channels[chan]).SPFCR.BIT.RTRG = g_rspia_tcb[chan].rx_dflt_thresh;

    /* Update the SPCMD0 command register with the settings for this transfer. */
    /* Re-assign BRDV value to data structure */
    p_cmd.br_div = (rspia_spcmd_br_div_t)(*g_rspia_channels[chan]).SPCMD0.BIT.BRDV;

    (*g_rspia_channels[chan]).SPCMD0.LONG = p_cmd.word[0];

    /* Determine master/slave mode setting based on channel settings argument.
     * Overrides prior state for this bit . */
    if (RSPIA_MS_MODE_MASTER == p_cfg->master_slave_mode)
    {
        p_setregs->spcr_val |= RSPIA_MS_MODE_MASTER;               /* Set the master mode bit */
    }
    else
    {
        p_setregs->spcr_val &= RSPIA_MS_MODE_SLAVE;                /* Clear the master mode bit */
    }

    /* Determine RSPIA slave select mode setting based on channel settings argument.
     * Overrides prior state for this bit . */
    if (RSPIA_IF_MODE_4WIRE == p_cfg->gpio_ssl)
    {
        p_setregs->spcr_val &= RSPIA_IF_MODE_4WIRE;                /* Clear the SPMS bit */

        if (RSPIA_IF_FRAME_TI_SSP == p_cfg->frame_mode)
        {
            p_setregs->spcr_val |= RSPIA_IF_FRAME_TI_SSP;          /* Set the TI SSP frame mode bit */
        }
        else
        {
            p_setregs->spcr_val &= RSPIA_IF_FRAME_MOTOROLA_SPI;    /* Set the Motorola SPI frame mode bit */
        }
    }
    else
    {
        p_setregs->spcr_val |= RSPIA_IF_MODE_3WIRE;                /* Set the SPMS bit */
    }

    /* Set RSPIA control register (SPCR) */
    (*g_rspia_channels[chan]).SPCR.LONG = p_setregs->spcr_val;

    return err;
}
/**********************************************************************************************************************
 End of function rspia_regs_set
 *********************************************************************************************************************/


#if RSPIA_CFG_PARAM_CHECKING_ENABLE
/*******************************************************************************
* Function Name: rspia_chan_check
* Description  : Checks whether channel is valid or not.
* Arguments    : chan -
*                    Which channel to use
* Return Value : RSPIA_SUCCESS -
*                    The channel is valid.
*                RSPIA_ERR_OMITTED_CHAN -
*                    The channel is not enabled in config.h
*******************************************************************************/
static rspia_err_t rspia_chan_check(uint8_t chan)
{
    if (chan >= RSPIA_NUM_CH)
    {
        /* Invalid channel. */
        return RSPIA_ERR_BAD_CHAN;
    }

    switch (chan)
    {
        case RSPIA_CH0:
        {
#if RSPIA_CFG_USE_CH0
            /* Do nothing */
#else
            /* RSPIA channel 0 is not enabled. */
            return RSPIA_ERR_OMITTED_CHAN;
#endif /* #if RSPIA_CFG_USE_CH0 */
            break;
        }

        default:
            break;
    }
    return RSPIA_SUCCESS;
}
/******************************************************************************
 End of function rspia_chan_check
 *****************************************************************************/
#endif /* RSPIA_CFG_PARAM_CHECKING_ENABLE */

/***********************************************************************************************************************
* Function Name: rspia_baud_set
* Description  : Determines the RSPIA channel SPBR register setting for the requested bit rate.
*                Returns the actual bit rate that the setting will achieve which may differ from requested.
*                If the requested bit rate cannot be exactly achieved, the next lower bit rate setting will be applied.
*                If successful, applies the calculated setting to the SPBR register.
* Arguments    : chan -
*                    Which channel to use
*                bsp_target -
*                    Target bit rate
* Return Value : bps_calc -
                     The actual bit rate is calculated base on requested bit rate
* Limitations   : Does not track dynamically changing PCLK. Relies on constant BSP_PCLKA_HZ
***********************************************************************************************************************/
static uint32_t rspia_baud_set(uint8_t chan, uint32_t bps_target)
{
    uint8_t     br_dv;           // N term in equation
    uint32_t    bps_calc = 0;
    uint32_t    desired_divider;
    int32_t     f;               // Frequency
    int32_t     n;               // n term in equation

    /* Generates the peripheral module clock (PCLKA) to be supplied to the RSPIA. */
    f = BSP_PCLKA_HZ;

    /* desired_divider = Smallest integer greater than or equal to SPI_CLK / bit-rate. */
    desired_divider = (f / bps_target);

    /* Get the register settings for requested baud rate. */
    if ((RSPIA_CLK_MIN_DIV > desired_divider) || (RSPIA_CLK_MAX_DIV < desired_divider))
    {
        return 0;   // baud_bps_target too high or low for the PCLK.
    }

    /* From Hardware manual: Bit rate = f / (2 * (n + 1) * (2 ^ N))
     * where:
     *      f = PCLK, n = SPBR setting, N = BRDV bits
     * Solving for n:
     *      n = (((f / (2 ^ N)) / 2) / bps) - 1
     */

    /* Calculate for BRDV value of SPCMD command data structure to get SPBR setting for the board PCLK.
     * Possible SPI_CLK dividers for values of N:
     *   N = 0; div = [2,4,6,..,512]
     *   N = 1; div = [4,8,12,..,1024]
     *   N = 2; div = [8,16,32,..,2048]
     *   N = 3; div = [16,32,64,..,4096]
     */
    for (br_dv = 0; br_dv < 4; br_dv++)
    {
        /* Select the smallest value for N possible.
         * div <= 512;        N = br_dv = 0
         * 512 < div <= 1024; N = br_dv = 1
         * ...
        */
        if (desired_divider <= (RSPIA_CLK_DIV_BASE << br_dv))
        {
            break;
        }
    }

    /* Assign br_dv value to BRDV to guarantee actual bit rate */
    (*g_rspia_channels[chan]).SPCMD0.BIT.BRDV = br_dv & 0x03U; // Make sure value of N is in rage of BRDV[1:0]

    /* Casting uint32_t type to int32_t type is valid */
    n = ((f >> (br_dv + 1)) / (int32_t)bps_target) - 1;   // Solve for SPBR setting.

    if ((n >= 0) && (n <= 0xff))   /* Must be <= SPBR register max value. Must not be negative */
    {
        /* Now plug n back into the formula for BPS and check it. */
        bps_calc = (uint32_t)(f / (2 *((n + 1) << br_dv)));

        if (bps_calc > bps_target)
        {
            n += 1;
            if (n > 0xff)
            {
                return 0;   /* result out of range for the PCLK. */
            }
        }

        /* Apply the SPBR register value. */
        (*g_rspia_channels[chan]).SPBR = (uint8_t)n; // Casting to uint8_t is valid.
    }
    else
    {
        bps_calc = 0;   // Result out of range for the PCLK.
    }

    return bps_calc;    // Return the actual BPS rate achieved.
}
/******************************************************************************
 End of function rspia_baud_set
 *****************************************************************************/


/***********************************************************************************************************************
* Function Name: rspia_get_thresh_vals
* Description  : Get the threshold value of RSPIA channel.
* Arguments    : chan -
*                  Which channel to use.
* Return Value : None
***********************************************************************************************************************/
static void rspia_get_thresh_vals(uint8_t chan)
{
    switch (chan)
    {
#if RSPIA_CFG_USE_CH0
        case RSPIA_CH0:
        {
            /* Casting to uint8_t type is valid */
            g_rspia_tcb[chan].tx_dflt_thresh = (uint8_t)RSPIA_CFG_CH0_TX_FIFO_THRESH;

            /* Casting to uint8_t type is valid */
            g_rspia_tcb[chan].rx_dflt_thresh = (uint8_t)RSPIA_CFG_CH0_RX_FIFO_THRESH;

            /* Casting to uint8_t type is valid */
            g_rspia_tcb[chan].tx_curr_thresh = (uint8_t)RSPIA_CFG_CH0_TX_FIFO_THRESH;

            /* Casting to uint8_t type is valid */
            g_rspia_tcb[chan].rx_curr_thresh = (uint8_t)RSPIA_CFG_CH0_RX_FIFO_THRESH;
            break;
        }
#endif /* #if RSPIA_CFG_USE_CH0 */

        default:
        {
            break;
        }
    }
}
/******************************************************************************
 End of function rspia_get_thresh_vals
 *****************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_RSPIA_Control()
 *****************************************************************************************************************/ /**
 * @brief     The Control function is responsible for handling special hardware or software operations for the RSPI
 *            channel.
 * @param[in] hdl
 *            Handle for the channel
 * @param[in] cmd
 *            Enumerated command code.\n
 *            Available command codes:\n
 *             RSPIA_CMD_SET_BAUD : Change the base bit rate setting without reinitializing the RSPIA channel.\n
 *             RSPIA_CMD_ABORT : Stop the current read or write operation immediately.\n
 *             RSPIA_CMD_SET_REGS : Set all supported RSPIA regs in one operation. (Expert use only)\n
 *             RSPIA_CMD_CHANGE_TX_FIFO_THRESH : Change Frame TX FIFO threshold
 *             RSPIA_CMD_CHANGE_RX_FIFO_THRESH : Change Frame RX FIFO threshold
 * @param[in] *pcmd_data
 *            Pointer to the command-data structure parameter of type void that is used to reference the location of
 *            any data specific to the command needed for its completion. Commands that do not require supporting
 *            data must use the FIT_NO_PTR.
 * @retval    RSPIA_SUCCESS
 *             Command successfully completed.
 * @retval    RSPIA_ERR_CH_NOT_OPENED
 *             The channel has not been opened. Perform R_RSPIA_Open() first
 * @retval    RSPIA_ERR_UNKNOWN_CMD
 *             Control command is not recognized.
 * @retval    RSPIA_ERR_NULL_PTR
 *             *pcmd_data pointer or *p_hdl pointer is NULL.
 * @retval    RSPIA_ERR_ARG_RANGE
 *             An element of the *pcmd_data structure contains an invalid value.
 * @retval    RSPIA_ERR_LOCK
 *             The lock could not be acquired.
 * @details   This function is responsible for handling special hardware or software operations for the RSPIA channel.
 *            It takes an RSPIA handle to identify the selected RSPIA, an enumerated command value to select the
 *            operation to be performed, and a void pointer to a location that contains information or data required
 *            to complete the operation. This pointer must point to storage that has been type-cast by the caller for
 *            the particular command using the appropriate type provided in "r_rspia_rx_if.h".
 * @note      None.
 */
rspia_err_t R_RSPIA_Control(rspia_hdl_t     hdl,
                            rspia_cmd_t     cmd,
                            void          * pcmd_data)
{
    /* Command function data structure definitions. One for each command in rspia_cmd_t. */
    rspia_cmd_baud_t      * p_baud_struct;
    rspia_cmd_setregs_t   * p_setregs_struct;
    uint32_t    reg_temp = 0;
    uint8_t     chan     = hdl->chan;

#if RSPIA_CFG_REQUIRE_LOCK
    bool        lock_result = false;
#endif

#if RSPIA_CFG_PARAM_CHECKING_ENABLE
    /* Check argument */
    if ((NULL == hdl) || ((NULL == pcmd_data) && ((void *)FIT_NO_PTR != pcmd_data)))
    {
        return RSPIA_ERR_NULL_PTR;
    }
    if (!g_rspia_handles[chan].rspia_chan_opened)
    {
        return RSPIA_ERR_CH_NOT_OPENED;
    }
    if ((RSPIA_CMD_CHANGE_TX_FIFO_THRESH == cmd) || (RSPIA_CMD_CHANGE_RX_FIFO_THRESH == cmd))
    {
        /* Casting void* type is valid */
        if (3 < (*(uint8_t *)pcmd_data))
        {
            return RSPIA_ERR_ARG_RANGE;
        }
    }
#endif

#if RSPIA_CFG_REQUIRE_LOCK
    /* Attempt to acquire lock for this RSPI channel. Prevents re-entrancy conflict. */
    lock_result = R_BSP_HardwareLock((mcu_lock_t)(BSP_LOCK_RSPIA0 + chan));

    /* Only abort cmd is allowed while transfer in progress. */
    if ((false == lock_result) && (RSPIA_CMD_ABORT != cmd))
    {
        return RSPIA_ERR_LOCK;  /* The control function is currently locked. */
    }
#endif

    switch (cmd)
    {
        case RSPIA_CMD_SET_BAUD:
        {
            /* Casting to rspia_cmd_baud_t type is valid */
            p_baud_struct = (rspia_cmd_baud_t *)pcmd_data;

            reg_temp = (*g_rspia_channels[chan]).SPCR.LONG; // Temporarily save state of the SPCR register.

            /* Temporarily disable the RSPIA operation. */
            /* SPE, SPTIE and MSTR should be cleared simultaneously. */
            (*g_rspia_channels[chan]).SPCR.LONG = (uint32_t)(reg_temp & (~((RSPIA_SPCR_SPTIE | RSPIA_SPCR_SPE) | RSPIA_SPCR_MSTR)));
            if (0 == (*g_rspia_channels[chan]).SPCR.LONG)
            {
                R_BSP_NOP();
            }

            /* Update the baud rate. */
            /* Get the register settings for requested baud rate. */
            if (0 == rspia_baud_set(chan, p_baud_struct->bps_target))
            {
#if RSPIA_CFG_REQUIRE_LOCK
                /* Release lock for this channel. */
                R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_RSPIA0 + chan));
#endif
                return RSPIA_ERR_ARG_RANGE; /* Could not calculate settings for the requested baud rate. */
            }

            /* Re-enable the RSPIA operation. */
            (*g_rspia_channels[chan]).SPCR.LONG = reg_temp;

            break;
        }

        case RSPIA_CMD_ABORT:
        {
            /* Perform immediate abort of the active RSPIA transfer on this channel.
             * Does not close the channel. */
            rspia_interrupts_set(chan, false);           // Disable interrupts in ICU.
            (*g_rspia_channels[chan]).SPCR.LONG &= (~RSPIA_SPCR_SPE);  // Disable RSPIA operation. Forces soft reset.
            if (0 == ((*g_rspia_channels[chan]).SPCR.LONG & RSPIA_SPCR_SPE))
            {
                R_BSP_NOP();
            }

            /* Transfer aborted. Call the user callback function passing pointer to the result structure. */
            if ((FIT_NO_FUNC != g_rspia_handles[chan].p_callback) && (NULL != g_rspia_handles[chan].p_callback))
            {
                g_rspia_cb_data[chan].hdl = &(g_rspia_handles[chan]);
                g_rspia_cb_data[chan].event = RSPIA_EVT_TRANSFER_ABORTED;

                /* Casting to void* type is valid */
                g_rspia_handles[chan].p_callback((void*)&(g_rspia_cb_data[chan]));
            }
            break;
        }

        case RSPIA_CMD_SET_REGS:   /* Expert use only! Set all user supported RSPIA regs in one operation. */
        {
            /* Overrides driver default settings. */
            /* Copies user-specified register settings into driver's shadow copy. */
            /* Settings do not take effect until the channel is closed and then reopened. */
            p_setregs_struct = (rspia_cmd_setregs_t *)pcmd_data;

            reg_temp = (*g_rspia_channels[chan]).SPCR.LONG; // Temporarily save state of the SPCR register.

            /* Temporarily disable the RSPIA operation. */
            /* SPE, SPTIE and MSTR should be cleared simultaneously. */
            (*g_rspia_channels[chan]).SPCR.LONG = (uint32_t)(reg_temp & (~((RSPIA_SPCR_SPTIE | RSPIA_SPCR_SPE) | RSPIA_SPCR_MSTR)));
            if (0 == (*g_rspia_channels[chan]).SPCR.LONG)
            {
                R_BSP_NOP();
            }

            /* RSPIA Data Control Register (SPDCR) */
            (*g_rspia_channels[chan]).SPDCR.WORD = (uint16_t)(p_setregs_struct->spdcr_val & RSPIA_SPDCR_MASK);

            /* Set RSPIA clock delay registers (SPCKD) */
            (*g_rspia_channels[chan]).SPCKD.BYTE = (uint8_t)(p_setregs_struct->spckd_val & RSPIA_SPCKD_MASK);

            /* Set RSPIA next-access delay register (SPND) */
            (*g_rspia_channels[chan]).SPND.BYTE = (uint8_t)(p_setregs_struct->spnd_val & RSPIA_SPND_MASK);

            /* Set pin control register (SPPCR) */
            (*g_rspia_channels[chan]).SPPCR.BYTE = (uint8_t)(p_setregs_struct->sppcr_val & RSPIA_SPPCR_MASK);

            /* Set RSPIA slave select negation delay register (SSLND) */
            (*g_rspia_channels[chan]).SSLND.BYTE = (uint8_t)(p_setregs_struct->sslnd_val & RSPIA_SSLND_MASK);

            /* Set slave select polarity register (SSLP). */
            (*g_rspia_channels[chan]).SSLP.BYTE = (uint8_t)(p_setregs_struct->sslp_val & RSPIA_SSLP_MASK);

            /* Re-enable the RSPIA operation. */
            (*g_rspia_channels[chan]).SPCR.LONG = reg_temp;

            break;
        }

        case RSPIA_CMD_CHANGE_TX_FIFO_THRESH:
        {
            /* Save current TX FIFO threshold */
            g_rspia_tcb[chan].tx_curr_thresh = (*(uint8_t *)pcmd_data);

            reg_temp = (*g_rspia_channels[chan]).SPCR.LONG; // Temporarily save state of the SPCR register.

            /* Temporarily disable the RSPIA operation. */
            /* SPE, SPTIE and MSTR should be cleared simultaneously. */
            (*g_rspia_channels[chan]).SPCR.LONG = (uint32_t)(reg_temp & (~((RSPIA_SPCR_SPTIE | RSPIA_SPCR_SPE) | RSPIA_SPCR_MSTR)));
            if (0 == (*g_rspia_channels[chan]).SPCR.LONG)
            {
                R_BSP_NOP();
            }

            /* Change TX FIFO threshold */
            (*g_rspia_channels[chan]).SPFCR.BIT.TTRG = (*(uint8_t *)pcmd_data); /* Casting void* type is valid */

            /* Re-enable the RSPIA operation. */
            (*g_rspia_channels[chan]).SPCR.LONG = reg_temp;

            break;
        }

        case RSPIA_CMD_CHANGE_RX_FIFO_THRESH:
        {
            /* Save current RX FIFO threshold */
            g_rspia_tcb[chan].rx_curr_thresh = (*(uint8_t *)pcmd_data);

            reg_temp = (*g_rspia_channels[chan]).SPCR.LONG; // Temporarily save state of the SPCR register.

            /* Temporarily disable the RSPIA operation. */
            /* SPE, SPTIE and MSTR should be cleared simultaneously. */
            (*g_rspia_channels[chan]).SPCR.LONG = (uint32_t)(reg_temp & (~((RSPIA_SPCR_SPTIE | RSPIA_SPCR_SPE) | RSPIA_SPCR_MSTR)));
            if (0 == (*g_rspia_channels[chan]).SPCR.LONG)
            {
                R_BSP_NOP();
            }

            /* Change RX FIFO threshold */
            (*g_rspia_channels[chan]).SPFCR.BIT.RTRG = (*(uint8_t *)pcmd_data); /* Casting void* type is valid */

            /* Re-enable the RSPIA operation. */
            (*g_rspia_channels[chan]).SPCR.LONG = reg_temp;

            break;
        }

        default:
        {
#if RSPIA_CFG_REQUIRE_LOCK
            /* Release lock for this channel. */
            R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_RSPIA0 + chan));
#endif
            /* Error, command not recognized. */
            return RSPIA_ERR_UNKNOWN_CMD;
            break;
        }
    }

#if RSPIA_CFG_REQUIRE_LOCK
    /* Release lock for this channel. */
    R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_RSPIA0 + chan));
#endif

    return RSPIA_SUCCESS;
}
/**********************************************************************************************************************
 End of function R_RSPIA_Control
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: R_RSPIA_Read()
 *****************************************************************************************************************/ /**
 * @brief     The Read function receives data from the selected SPI device.
 * @param[in] hdl
 *            Handle for the channel
 * @param[in] p_cmd
 *            Bit field data consisting of all the RSPIA command register settings for SPCMD for this operation.\n
 *            See 2.14  in application note.
 * @param[out] *p_dst
 *            Void type pointer to a destination buffer into which data will be copied that has been received from a SPI
 *            device. It is the responsibility of the caller to ensure that adequate space is available to hold the
 *            requested data count. The argument must not be NULL. Based on the data frame bit-length specified in the
 *            p_cmd.bit_length, the *p_dst pointer will be type cast to the corresponding data type during
 *            the transfer. So, for example, if the bit-length is set to 16-bits, then the data will be stored in the
 *            destination buffer as a 16-bit value, and so on for each bit-length setting. Bit-length settings that are
 *            not 8, 16 or 32 will use the smallest data type that they can be contained within. For example, 24-bit
 *            frames will be stored in 32-bit storage, 11-bit frames will be stored in 16-bit storage, etc.
 * @param[in] length
 *            Transfer length variable to indicate the number of data frames to be transferred. The size of the data
 *            word is determined from settings in the p_cmd.bit_length argument. Be sure that the length
 *            argument matches the storage type of the source data; this is a count of the number of frames, not the
 *            number of bytes.
 * @retval    RSPIA_SUCCESS
 *             Read operation successfully completed.
 * @retval    RSPIA_ERR_CH_NOT_OPENED
 *             The channel has not been opened. Perform R_RSPIA_Open() first.
 * @retval    RSPIA_ERR_NULL_PTR
 *             A required pointer argument is NULL.
 * @retval    RSPIA_ERR_LOCK
 *             The lock could not be acquired. The channel is busy.
 * @retval    RSPIA_ERR_INVALID_ARG
 *             Argument is not valid for parameter.
 * @details   Starts reception of data from a SPI device. The function returns immediately after the operation begins,
 *            and data will continue to be received in the background under interrupt control until the requested length
 *            has been received. Received data is stored in the destination buffer. When the transfer is complete the
 *            user-defined callback function is called.\n
 *            Operation differs slightly depending on whether the RSPIA is operating as Master or Slave. If the RSPIA is
 *            configured as slave, then data will only transfer when clocks are received from the Master. While
 *            receiving data, the RSPIA will also transmit the user definable Dummy data pattern defined in the
 *            configuration file.
 * @note      None.
 */
rspia_err_t    R_RSPIA_Read(rspia_hdl_t           hdl,
                            rspia_command_word_t  p_cmd,
                            void                * p_dst,
                            uint16_t              length)
{
    rspia_err_t  ret;

#if RSPIA_CFG_PARAM_CHECKING_ENABLE
    if ((NULL == hdl) || (NULL == p_dst))
    {
        return RSPIA_ERR_NULL_PTR;
    }
    if (0 == length)
    {
        return RSPIA_ERR_INVALID_ARG;
    }
#endif

    ret = rspia_write_read_common(hdl, p_cmd, NULL, p_dst, length, RSPIA_DO_RX);

    return ret;
}
/**********************************************************************************************************************
 End of function R_RSPIA_Read
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: R_RSPIA_Write()
 *****************************************************************************************************************/ /**
 * @brief     The Write function transmits data to the selected SPIA device
 * @param[in] hdl
 *            Handle for the channel
 * @param[in] p_cmd
 *            Bit field data consisting of all the RSPIA command register settings for SPCMD for this operation.\n
 *            See 2.14 in application note.
 * @param[in] *p_src
 *            Void type pointer to a source data buffer from which data will be transmitted to a SPI device. Based on
 *            the data frame bit-length specified in the p_cmd.bit_length, the *p_src pointer will be type
 *            cast to the corresponding data type during the transfer. So, for example, if the bit-length is set to
 *            16-bits, then the source buffer data will be accessed as a block of 16-bit data, and so on for each
 *            bit-length setting. Bit-length settings that are not 8, 16 or 32 will use the data type that they can be
 *            contained within.  For example, 24-bit frames will be stored in 32-bit storage, 11-bit frames will be
 *            stored in 16-bit storage, etc.
 * @param[in] length
 *            Transfer length variable to indicate the number of data frames to be transferred. The size of the data
 *            word is determined from settings in the p_cmd.bit_length argument. Be sure that the length
 *            argument matches the storage type of the source data; this is a count of the number of frames, not the
 *            number of bytes.
 * @retval    RSPIA_SUCCESS
 *             Write operation successfully completed.
 * @retval    RSPIA_ERR_CH_NOT_OPENED
 *             The channel has not been opened. Perform R_RSPIA_Open() first.
 * @retval    RSPIA_ERR_NULL_PTR
 *             A required pointer argument is NULL.
 * @retval    RSPIA_ERR_LOCK
 *             The lock could not be acquired. The channel is busy.
 * @retval    RSPIA_ERR_INVALID_ARG
 *             Argument is not valid for parameter.
 * @details   Starts transmission of data to a SPI device. The function returns immediately after the transmit operation
 *            begins, and data will continue to be transmitted in the background under interrupt control until the
 *            requested length has been transmitted. When the transmission is complete the user-defined callback
 *            function is called. The callback function should be used to notify the user application that the transfer
 *            has completed.\n
 *            Operation differs slightly depending on whether the RSPIA is operating as Master or Slave. If the RSPIA is
 *            configured as slave, then data will only transfer when clocks are received from the Master. Data received
 *            by the RSPIA peripheral will be discarded.
 * @note      None.
 */
rspia_err_t   R_RSPIA_Write(rspia_hdl_t           hdl,
                            rspia_command_word_t  p_cmd,
                            void                * p_src,
                            uint16_t              length)
{
    rspia_err_t  ret;

#if RSPIA_CFG_PARAM_CHECKING_ENABLE
    if ((NULL == hdl) || (NULL == p_src))
    {
        return RSPIA_ERR_NULL_PTR;
    }
    if (0 == length)
    {
        return RSPIA_ERR_INVALID_ARG;
    }
#endif

    ret = rspia_write_read_common(hdl, p_cmd, p_src, NULL, length, RSPIA_DO_TX);

    return ret;
}
/**********************************************************************************************************************
 End of function R_RSPIA_Write
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: R_RSPIA_WriteRead()
 *****************************************************************************************************************/ /**
 * @brief     The Write Read function simultaneously transmits data to a SPI device while receiving data from a SPI
 *            device.
 * @param[in] hdl
 *            Handle for the channel
 * @param[in] p_cmd
 *            Bit field data consisting of all the RSPIA settings for the command register (SPCMD) for this operation.\n
 *            See 2.14 in application note.
 * @param[in] *p_src
 *            Void type pointer to a source data buffer from which data will be transmitted to a SPI device. Based on
 *            the data frame bit-length specified in the spcmd_command_word.bit_length, the *psrc pointer will be type
 *            cast to the corresponding data type during the transfer. So, for example, if the bit-length is set to
 *            16-bits, then the source buffer data will be accessed as a block of 16-bit data, and so on for each
 *            bit-length setting. Bit-length settings that are not 8, 16 or 32 will use the data type that they can be
 *            contained within. For example, 24-bit frames will be stored in 32-bit storage, 11-bit frames will be
 *            stored in 16-bit storage, etc.
 * @param[out] *p_dst
 *            Void type pointer to a destination buffer into which data will be copied that has been received from a SPI
 *            device. It is the responsibility of the caller to ensure that adequate space is available to hold the
 *            requested data count. The argument must not be NULL. Based on the data frame bit-length specified in the
 *            spcmd_command_word.bit_length, the *pdest pointer will be type cast to the corresponding data type during
 *            the transfer. So, for example, if the bit-length is set to 16-bits, then the data will be stored in the
 *            destination buffer as a 16-bit value, and so on for each bit-length setting. Bit-length settings that are
 *            not 8, 16 or 32 will use the smallest data type that they can be contained within. For example, 24-bit
 *            frames will be stored in 32-bit storage, 11-bit frames will be stored in 16-bit storage, etc.
 * @param[in] length
 *            Transfer length variable to indicate the number of data frames to be transferred. The size of the data
 *            word is determined from settings in the spcmd_command_word.bit_length argument. Be sure that the length
 *            argument matches the storage type of the source data; this is a count of the number of frames, not the
 *            number of bytes. The same number of frames will be both written and read.
 * @retval    RSPIA_SUCCESS
 *             Read operation successfully completed.
 * @retval    RSPIA_ERR_CH_NOT_OPENED
 *             The channel has not been opened. Perform R_RSPIA_Open() first.
 * @retval    RSPIA_ERR_NULL_PTR
 *             A required pointer argument is NULL.
 * @retval    RSPIA_ERR_LOCK
 *             The lock could not be acquired. The channel is busy.
 * @retval    RSPIA_ERR_INVALID_ARG
 *             Argument is not valid for parameter.
 * @details   Starts full-duplex transmission and reception of data to and from a SPI device. The function returns
 *            immediately after the transfer operation begins, and data transfer will continue in the background under
 *            interrupt control until the requested length has been transferred. When the operation is complete the
 *            user-defined callback function is called. The callback function should be used to notify the user
 *            application that the transfer has completed.\n
 *            Operation differs slightly depending on whether the RSPIA is operating as Master or Slave. If the RSPIA is
 *            configured as slave, then data will only transfer when clocks are received from the Master. Data to be
 *            transmitted is obtained from the source buffer, while received data is stored in the destination buffer.
 * @note      none.
 */
rspia_err_t   R_RSPIA_WriteRead(rspia_hdl_t           hdl,
                                rspia_command_word_t  p_cmd,
                                void                * p_src,
                                void                * p_dst,
                                uint16_t              length)
{
    rspia_err_t  ret;

#if RSPIA_CFG_PARAM_CHECKING_ENABLE
    if (((NULL == hdl) || (NULL == p_src)) || (NULL == p_dst))
    {
        return RSPIA_ERR_NULL_PTR;
    }
    if (0 == length)
    {
        return RSPIA_ERR_INVALID_ARG;
    }
#endif

    ret = rspia_write_read_common(hdl, p_cmd, p_src, p_dst, length, RSPIA_DO_TX_RX);

    return ret;
}
/**********************************************************************************************************************
 End of function R_RSPIA_WriteRead
 *********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: rspia_write_read_common
* Description  : Initiates write or read process. Common routine used by RSPIA API write or read functions.
* Arguments    : hdl-
*                   Handle for the channel
*                p_cmd-
*                   bit field data consisting of all the RSPIA command register settings for SPCMD for this operation.
*                   This value will be placed directly into the SPCMD0 register by the function. Caller is required to
*                   provide correctly formatted data.
*                p_src-
*                   For write operations, pointer to the source buffer of the data to be sent.
*                p_dst-
*                   For read operations, pointer to destination buffer into which received data will be copied.
*                length-
*                   The number of data words to be transferred.
* Return Value : RSPIA_SUCCESS
*                RSPIA_ERR_CH_NOT_OPENED-
*                   The channel has not been opened.  Perform R_RSPIA_Open() first
*                RSPIA_ERR_INVALID_ARG
*                   Argument is not valid for parameter.
*                RSPIA_ERR_LOCK-
*                   The lock could not be acquired. The channel is busy.
* Note         : None.
***********************************************************************************************************************/
static rspia_err_t  rspia_write_read_common(rspia_hdl_t            hdl,
                                            rspia_command_word_t   p_cmd,
                                            void         * p_src,
                                            void         * p_dst,
                                            uint16_t       length,
                                            rspia_operation_t   tx_rx_mode)
{
    uint8_t  chan = hdl->chan;

#if RSPIA_CFG_REQUIRE_LOCK
    bool     lock_result = false;
#endif

    if (!g_rspia_handles[chan].rspia_chan_opened)
    {
        return RSPIA_ERR_CH_NOT_OPENED;
    }

#if RSPIA_CFG_REQUIRE_LOCK
    /* Attempt to acquire lock for this RSPIA channel. Prevents re-entrancy conflict. */
    lock_result = R_BSP_HardwareLock((mcu_lock_t)(BSP_LOCK_RSPIA0 + chan));

    if (false == lock_result)
    {
        return RSPIA_ERR_LOCK;  // The control function is currently locked.
    }
#endif

    /* Disable interrupts in ICU.*/
    rspia_interrupts_set(chan, false);

    g_rspia_tcb[chan].xfr_length    = length;
    g_rspia_tcb[chan].tx_count      = 0;
    g_rspia_tcb[chan].rx_count      = 0;
    g_rspia_tcb[chan].p_src         = p_src;
    g_rspia_tcb[chan].p_dst         = p_dst;
    g_rspia_tcb[chan].transfer_mode = tx_rx_mode;
    g_rspia_tcb[chan].idle          = false;
    g_rspia_tcb[chan].tx_dummy      = false;

    /* Get data type of frame length */
    if (0 == rspia_get_data_type(p_cmd))
    {
        /* Failed */
#if RSPIA_CFG_REQUIRE_LOCK
        /* Casting to mcu_lock_t type is valid */
        R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_RSPIA0 + chan));
#endif
        return RSPIA_ERR_INVALID_ARG;
    }
    else
    {
        g_rspia_tcb[chan].bytes_per_transfer = rspia_get_data_type(p_cmd);
    }

    /* Check and Clear SPE bit to set communication */
    if ((*g_rspia_channels[chan]).SPCR.LONG & RSPIA_SPCR_SPE)
    {
        (*g_rspia_channels[chan]).SPCR.LONG &= (~RSPIA_SPCR_SPE);
    }

    /* Simultaneously Clear Communication Mode Select Bit and Set default communication mode  */
    /* Bit CMMD[1:0] is set to 00b: Transmit-Receive. */
    (*g_rspia_channels[chan]).SPCR.LONG &= (~RSPIA_SPCR_CMMD_TX_RX_MASK);

    if (tx_rx_mode & RSPIA_DO_TX)
    {
        if (RSPIA_DO_TX == tx_rx_mode)
        {
            /* Bit CMMD[1:0] is set to 01b: Transmit only */
            (*g_rspia_channels[chan]).SPCR.LONG |= RSPIA_SPCR_CMMD_TX_MASK;
        }
    }
    else
    {
        /* Allowed transmit dummy data when only receive operation */
        g_rspia_tcb[chan].tx_dummy = true;
    }

    /* Reassigns the value of RX FIFO threshold */
    if ((RSPIA_DO_TX != tx_rx_mode) && (length <= g_rspia_tcb[chan].rx_curr_thresh))
    {
        (*g_rspia_channels[chan]).SPFCR.BIT.RTRG = (uint8_t)(length - 1);
        g_rspia_tcb[chan].rx_curr_thresh          = (uint8_t)(length - 1);
    }

    /* Wait for channel to be idle before making changes to registers. */
    while ((*g_rspia_channels[chan]).SPSR.WORD & RSPIA_SPSR_IDLNF)
    {
        R_BSP_NOP();
    }

    /* WAIT_LOOP */
    /* Clear error sources: the SPSR.MODF, OVRF, PERF and UDRF flags. */
    while ((*g_rspia_channels[chan]).SPSR.WORD  &  ((((RSPIA_SPSR_OVRF  | RSPIA_SPSR_MODF) | RSPIA_SPSR_PERF) | RSPIA_SPSR_UDRF) | RSPIA_SPSR_RRDYF))
    {
        (*g_rspia_channels[chan]).SPSCLR.WORD |= RSPIA_SPSCLR_MASK;
    }

    /* Initializes the pointer and stored data in the transmit/receive FIFO. */
    (*g_rspia_channels[chan]).SPFCLR.BYTE |= RSPIA_SPFCLR_MASK;

    /* Disable communication end interrupt, Idle Interrupt. */
    (*g_rspia_channels[chan]).SPCR.LONG &= (~(RSPIA_SPCR_SPIIE | RSPIA_SPCR_SPCIE));

    /* Clear flag interrupts in ICU. */
    rspia_interrupts_clear(chan);

    /* Enable interrupts in ICU. */
    rspia_interrupts_set(chan, true);

    /* Enable transmit buffer empty interrupt, Receive buffer full interrupt,
     * and enable RSPIA simultaneously. This will generate an SPTI interrupt,
     * and data transfer will proceed in the ISRs. */
    (*g_rspia_channels[chan]).SPCR.LONG |= (((RSPIA_SPCR_SPTIE | RSPIA_SPCR_SPRIE) | RSPIA_SPCR_SPEIE) | RSPIA_SPCR_SPE);

    return RSPIA_SUCCESS;
}
/******************************************************************************
 End of function rspia_write_read_common
 *****************************************************************************/


/***********************************************************************************************************************
* Function Name: rspia_get_data_type
* Description  : Identifies whether the data must be type-cast as 8-bit, 16-bit, or 32-bit for purposes of accessing the
*                source or destination buffers with the right type and index.
* Arguments    : p_cmd_frame-
*                   16-bit word containing the bits that define the bits per frame in th SPCMD register.
*                   Only the bits corresponding to "SPB[4:0] RSPIA Data Length Setting" of the SPCMDn register are
*                   checked in this argument.
* Return Value : RSPIA_BYTE_DATA-
*                   Data is 8-bit.
*                RSPIA_WORD_DATA-
*                   Data is > 8-bit and <= 16-bit.
*                RSPIA_LONG_DATA-
*                   Data is > 16-bit.
***********************************************************************************************************************/
static uint8_t rspia_get_data_type(rspia_command_word_t p_cmd_frame)
{
    uint8_t data_type;
    uint8_t frame_length_bits;

    /* Casting to uint8_t type is valid */
    frame_length_bits = (uint8_t)((p_cmd_frame.word[0] & RSPIA_SPCMD_SPB) >> 16);

    switch (frame_length_bits)
    {
        case RSPIA_SPCMD_BIT_LENGTH_4:
        case RSPIA_SPCMD_BIT_LENGTH_5:
        case RSPIA_SPCMD_BIT_LENGTH_6:
        case RSPIA_SPCMD_BIT_LENGTH_7:
        case RSPIA_SPCMD_BIT_LENGTH_8:
        {
            data_type = RSPIA_BYTE_DATA;
            break;
        }

        case RSPIA_SPCMD_BIT_LENGTH_9:
        case RSPIA_SPCMD_BIT_LENGTH_10:
        case RSPIA_SPCMD_BIT_LENGTH_11:
        case RSPIA_SPCMD_BIT_LENGTH_12:
        case RSPIA_SPCMD_BIT_LENGTH_13:
        case RSPIA_SPCMD_BIT_LENGTH_14:
        case RSPIA_SPCMD_BIT_LENGTH_15:
        case RSPIA_SPCMD_BIT_LENGTH_16:
        {
            data_type = RSPIA_WORD_DATA;
            break;
        }

        case RSPIA_SPCMD_BIT_LENGTH_17:
        case RSPIA_SPCMD_BIT_LENGTH_18:
        case RSPIA_SPCMD_BIT_LENGTH_19:
        case RSPIA_SPCMD_BIT_LENGTH_20:
        case RSPIA_SPCMD_BIT_LENGTH_21:
        case RSPIA_SPCMD_BIT_LENGTH_22:
        case RSPIA_SPCMD_BIT_LENGTH_23:
        case RSPIA_SPCMD_BIT_LENGTH_24:
        case RSPIA_SPCMD_BIT_LENGTH_25:
        case RSPIA_SPCMD_BIT_LENGTH_26:
        case RSPIA_SPCMD_BIT_LENGTH_27:
        case RSPIA_SPCMD_BIT_LENGTH_28:
        case RSPIA_SPCMD_BIT_LENGTH_29:
        case RSPIA_SPCMD_BIT_LENGTH_30:
        case RSPIA_SPCMD_BIT_LENGTH_31:
        case RSPIA_SPCMD_BIT_LENGTH_32:
        {
            data_type = RSPIA_LONG_DATA;
            break;
        }

        default:
        {
            data_type = 0;
            break;
        }
    }
    return data_type;
}
/******************************************************************************
 End of function rspia_get_data_type
 *****************************************************************************/


/**********************************************************************************************************************
 * Function Name: R_RSPIA_Close()
 *****************************************************************************************************************/ /**
 * @brief     Fully disables the RSPIA channel designated by the handle.
 * @param[in] hdl
 *            RSPIA handle number
 * @retval    RSPIA_SUCCESS
 *             Successful : channel closed
 * @retval    RSPIA_ERR_CH_NOT_OPENED
 *             The channel has not been opened so closing has no effect.
 * @retval    RSPIA_ERR_NULL_PTR
 *             A required pointer argument is NULL
 * @details   This disables the RSPIA channel designated by the handle. The RSPIA handle is modified to indicate that it
 *            is no longer in the 'open' state. The RSPIA channel cannot be used again until it has been reopened with
 *            the R_RSPIA_Open function. If this function is called for an RSPI that is not in the open state then an
 *            error code is returned.
 * @note      None.
 */
rspia_err_t  R_RSPIA_Close(rspia_hdl_t hdl)
{
    uint8_t chan;

#if RSPIA_CFG_PARAM_CHECKING_ENABLE
    if (NULL == hdl)
    {
        return RSPIA_ERR_NULL_PTR;
    }
#endif

    chan = hdl->chan;

    /* Check to see if the channel is currently initialized. */
    if (false == g_rspia_handles[chan].rspia_chan_opened)
    {
        /* This channel is not open so need not be closed. */
        return RSPIA_ERR_CH_NOT_OPENED;
    }

    /* Disable the RSPI operation. */
    /* SPE and SPTIE should be cleared simultaneously. */
    (*g_rspia_channels[chan]).SPCR.LONG &= (uint32_t)(~(RSPIA_SPCR_SPTIE | RSPIA_SPCR_SPE));
    if (0 == (*g_rspia_channels[chan]).SPCR.LONG)
    {
        R_BSP_NOP();
    }

    /* Turn off channel power. */
    power_on_off(chan, RSPIA_POWER_OFF);

    /* Disable interrupts. */
    rspia_interrupts_set(chan, false);

    g_rspia_handles[chan].rspia_chan_opened = false;

    return RSPIA_SUCCESS;
}
/**********************************************************************************************************************
 End of function R_RSPIA_Close
 *********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: power_on_off
* Description : Switches power to an RSPIA channel.  Required by FIT spec.
* Arguments : channel -
*                   Which channel to use.
*             on_or_off -
*                   What it says.
* Return Value : none
***********************************************************************************************************************/
static void power_on_off(uint8_t chan, uint8_t on_or_off)
{
#if RSPIA_CFG_USE_CH0
    #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
    #endif
#endif

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    switch (chan)
    {
#if RSPIA_CFG_USE_CH0
        case RSPIA_CH0:
        {
            #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
            #endif
            /* Enable power of channel */
            MSTP(RSPIA0) = on_or_off;

            #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
            #endif
            /* Waiting bit setting release */
            if (0 == MSTP(RSPIA0))
            {
                R_BSP_NOP();
            }
            break;
        }
#endif /* #if RSPIA_CFG_USE_CH0 */

        default:
            break;
    }

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);
}
/******************************************************************************
 End of function power_on_off
 *****************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_RSPIA_GetVersion()
 *****************************************************************************************************************/ /**
 * @brief     This function returns the driver version number at runtime.
 * @return    Version number with major and minor version digits packed into a single 32-bit value.
 * @details   The function returns the version of this module. The version number is encoded such that the top two bytes
 *            are the major version number and the bottom two bytes are the minor version number.
 * @note      None.
 */
uint32_t R_RSPIA_GetVersion(void)
{
    uint32_t version_number = 0;

    /* Bring in major version number. */
    version_number = ((uint16_t)RSPIA_RX_VERSION_MAJOR) << 16;

    /* Bring in minor version number. */
    version_number |= (uint16_t)RSPIA_RX_VERSION_MINOR;
    return version_number;
}
/**********************************************************************************************************************
 End of function R_RSPIA_GetVersion
 *********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: rspia_ir_priority_set
* Description  : sets the shared interrupt priority level for a channel.
* Arguments : channel -
*                 Which channel to use.
*             priority-
*                 0-15 priority value. 15 = highest priority.
* Return Value : none
***********************************************************************************************************************/
static void rspia_ir_priority_set(uint8_t chan, uint8_t priority)
{
    bsp_int_ctrl_t int_ctrl_data;

    /* Casting to uint32_t type is valid */
    int_ctrl_data.ipl = (uint32_t)priority;

    switch (chan)
    {
#if RSPIA_CFG_USE_CH0
        case RSPIA_CH0:
        {

            /* Set priority register for receive buffer full interrupts. */
            IPR(RSPIA0, SPRI) = priority;

            /* Set priority register for transmit buffer empty interrupts. */
            IPR(RSPIA0, SPTI) = priority;

            /* Enable the group interrupt for the SPEI0 interrupt. (sets priority also). */
            R_BSP_InterruptControl(BSP_INT_SRC_AL0_RSPIA0_SPEI, BSP_INT_CMD_GROUP_INTERRUPT_ENABLE, &int_ctrl_data);

            /* Waiting bit setting release */
            if (0 == IPR(RSPIA0, SPRI))
            {
                R_BSP_NOP();
            }
            break;
        }
#endif /* #if RSPIA_CFG_USE_CH0 */

        default:
        {
            /* Nothing here */
            break;
        }
    }
}
/******************************************************************************
 End of function rspia_ir_priority_set
 *****************************************************************************/

/***********************************************************************************************************************
* Function Name: rspia_interrupts_clear
* Description  : Clear RSPIA interrupts.
* Arguments    : channel -
*                 Which channel to use.
* Return Value : none
***********************************************************************************************************************/
static void rspia_interrupts_clear(uint8_t chan)
{
    switch (chan)
    {
#if RSPIA_CFG_USE_CH0
        case RSPIA_CH0:
        {
            /* Clear any pending receive buffer full interrupts */
            IR(RSPIA0, SPRI) = 0;

            /* Clear any pending transmit buffer empty interrupts */
            IR(RSPIA0, SPTI) = 0;

            /* Waiting bit setting release */
            if (0 == IR(RSPIA0, SPTI))
            {
                R_BSP_NOP();
            }
            break;
        }
#endif /* #if RSPIA_CFG_USE_CH0 */

        default:
        {
            /*Nothing here*/
            break;
        }
    }
}
/******************************************************************************
 End of function rspia_interrupts_clear
 *****************************************************************************/

/***********************************************************************************************************************
* Function Name: rspia_interrupts_set
* Description  : Disable or enable RSPIA interrupts.
* Arguments : channel -
*                 Which channel to use.
*             enabled-
*                 true = enable, false = disable.
* Return Value : none
***********************************************************************************************************************/
static void rspia_interrupts_set(uint8_t chan, bool enabled)
{
#if RSPIA_CFG_USE_CH0
    #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
    #endif
#endif

    switch (chan)
    {
#if RSPIA_CFG_USE_CH0
        case RSPIA_CH0:
        {
            /* Disable or enable receive buffer full interrupt */
            if (enabled)
            {
                R_BSP_InterruptRequestEnable(VECT(RSPIA0, SPRI));
            }
            else
            {
                R_BSP_InterruptRequestDisable(VECT(RSPIA0, SPRI));
            }

            /* Disable  or enable transmit buffer empty interrupt */
            if (enabled)
            {
                R_BSP_InterruptRequestEnable(VECT(RSPIA0, SPTI));
            }
            else
            {
                R_BSP_InterruptRequestDisable(VECT(RSPIA0, SPTI));
            }
            
            /* Waiting bit setting release */
            if (0 == IEN(RSPIA0, SPTI))
            {
                R_BSP_NOP();
            }
            
            if (enabled)
            {
                /* Register the error callback function with the BSP group interrupt handler. */
                R_BSP_InterruptWrite(BSP_INT_SRC_AL0_RSPIA0_SPEI, (bsp_int_cb_t)rspia_spei_grp_isr);

                #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
                R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
                #endif

                /* Enable error interrupt source bit */
                ICU.GENAL0.BIT.EN23 = 1;

                #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
                R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
                #endif
                /* Waiting bit setting release */
                if (0 == ICU.GENAL0.BIT.EN23)
                {
                    R_BSP_NOP();
                }
            }
            else
            {
                /* De-register the error callback function with the BSP group interrupt handler. */
                R_BSP_InterruptWrite(BSP_INT_SRC_AL0_RSPIA0_SPEI, FIT_NO_FUNC);

                /* Disable error interrupt source bit */
                #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
                R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
                #endif

                /* Disable error interrupt source bit */
                ICU.GENAL0.BIT.EN23 = 0;

                #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
                R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
                #endif
                /* Waiting bit setting release */
                if (0 == ICU.GENAL0.BIT.EN23)
                {
                    R_BSP_NOP();
                }
            }
            break;
        }
#endif /* #if RSPIA_CFG_USE_CH0 */

        default:
        {
            /* Nothing here */
            break;
        }
    }
}
/******************************************************************************
 End of function rspia_interrupts_set
 *****************************************************************************/

/******************************************************************************
* Function Name:    rspia_transmit
* Description  :    transmit data to TX FIFO buffer
* Arguments    :    RSPIA channel
* Return Value :    N/A
******************************************************************************/
static void rspia_transmit(uint8_t chan)
{
    void      * p_src     = g_rspia_tcb[chan].p_src;
    uint8_t     data_size = g_rspia_tcb[chan].bytes_per_transfer;
    uint8_t     fifo_frame_tx;
    uint8_t     cnt;

    /* Repeated empty transmit FIFO stages count */
    fifo_frame_tx = (*g_rspia_channels[chan]).SPTFSR.BIT.FREE;

    /* Service the hardware first to keep it busy. */
    /* Feed the TX. */
    for (cnt = 0; cnt < fifo_frame_tx; cnt++)
    {
        /* Don't write transmit buffer more than length. */
        if (g_rspia_tcb[chan].tx_count < g_rspia_tcb[chan].xfr_length)
        {
            if (g_rspia_tcb[chan].tx_dummy)
            {
                /* Must be RX only mode, so transmit dummy data for clocking.*/
                (*g_rspia_channels[chan]).SPDR.LONG = RSPIA_CFG_DUMMY_TX_DATA;
            }
            else
            {
                /* Transmit the data. TX data register accessed in long words. */
                if (RSPIA_BYTE_DATA == data_size)
                {
                    /* Casting void type to uint8_t type is valid */
                    (*g_rspia_channels[chan]).SPDR.LONG = ((uint8_t *)p_src)[g_rspia_tcb[chan].tx_count];
                }
                else if (RSPIA_WORD_DATA == data_size)
                {
                    /* Casting void type to uint16_t type is valid */
                    (*g_rspia_channels[chan]).SPDR.LONG = ((uint16_t *)p_src)[g_rspia_tcb[chan].tx_count];
                }
                else // Must be long data. if (RSPIA_LONG_DATA == data_size)
                {
                    /* Casting void type to uint32_t type is valid */
                    (*g_rspia_channels[chan]).SPDR.LONG = ((uint32_t *)p_src)[g_rspia_tcb[chan].tx_count];
                }
            }
            g_rspia_tcb[chan].tx_count++;
        }

        else
        {
            /* Last data was transmitted. */
            g_rspia_tcb[chan].idle = true; // There is no data to continues transmit.

            /* Disable SPTI interrupt. */
            (*g_rspia_channels[chan]).SPCR.LONG &= (uint32_t)(~RSPIA_SPCR_SPTIE);

            /* Check if transmit only */
            if (RSPIA_DO_TX == g_rspia_tcb[chan].transfer_mode)
            {
#if RSPIA_CFG_REQUIRE_LOCK
                /* Release lock for this channel. */
                R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_RSPIA0 + chan));
#endif

                /* Transfer complete. Call the user callback function passing pointer to the result structure. */
                if ((FIT_NO_FUNC != g_rspia_handles[chan].p_callback) && (NULL != g_rspia_handles[chan].p_callback))
                {
                    g_rspia_cb_data[chan].hdl   = &(g_rspia_handles[chan]);
                    g_rspia_cb_data[chan].event = RSPIA_EVT_TRANSFER_COMPLETE;

                    /* Casting pointer to void* type is valid */
                    g_rspia_handles[chan].p_callback((void*)&(g_rspia_cb_data[chan]));
                }
            }
        }

        /* If the data transfer empty, exit from FIFO transfer loop */
        if (true == g_rspia_tcb[chan].idle)
        {
            break;
        }
    }

    /* When the settings of transmit data are completed, set the SPSR.SPTEF flag to 0. */
    if ((*g_rspia_channels[chan]).SPSR.WORD & RSPIA_SPSR_SPTEF)
    {
        (*g_rspia_channels[chan]).SPSCLR.WORD |= RSPIA_SPSCLR_SPTEFC;
    }
}
/******************************************************************************
 End of function rspia_transmit
 *****************************************************************************/

/******************************************************************************
* Function Name:    rspia_tx_common
* Description  :    common ISR handler for SPTI
* Arguments    :    RSPIA channel
* Return Value :    N/A
******************************************************************************/
void rspia_tx_common(uint8_t chan)
{
    /* RSPIA transmit */
    rspia_transmit(chan);
}
/******************************************************************************
 End of function rspia_tx_common
 *****************************************************************************/

/******************************************************************************
* Function Name:    rspia_receive
* Description  :    Receive data from RX FIFO buffer
* Arguments    :    RSPIA channel
* Return Value :    N/A
******************************************************************************/
static void rspia_receive(uint8_t chan)
{
    void      * p_dst     = g_rspia_tcb[chan].p_dst;
    uint8_t     data_size = g_rspia_tcb[chan].bytes_per_transfer;
    uint32_t    g_rxdata[RSPIA_FIFO_FRAME_SIZE];
    uint16_t    thresh_temp;
    uint8_t     fifo_frame_rx;
    uint8_t     cnt;

    /* Get the number of stored receive FIFO stages */
    fifo_frame_rx = (*g_rspia_channels[chan]).SPRFSR.BIT.FILL;

    /* Don't read receive buffer more than length. */
    if (g_rspia_tcb[chan].rx_count < g_rspia_tcb[chan].xfr_length)
    {
        /* Get data from transmit FIFO buffer */
        for (cnt = 0; cnt < fifo_frame_rx; cnt++)
        {
            g_rxdata[cnt] = (*g_rspia_channels[chan]).SPDR.LONG;
        }

        /* Get data to destination */
        for (cnt = 0; cnt < fifo_frame_rx; cnt++)
        {
            if (RSPIA_BYTE_DATA == data_size)
            {
                /* Casting void type to uint8_t type is valid */
                ((uint8_t *)p_dst)[g_rspia_tcb[chan].rx_count] = (uint8_t)g_rxdata[cnt];
            }
            else if (RSPIA_WORD_DATA == data_size)
            {
                /* Casting void type to uint16_t type is valid */
                ((uint16_t *)p_dst)[g_rspia_tcb[chan].rx_count] = (uint16_t)g_rxdata[cnt];
            }
            else  // Must be long data. if (RSPIA_LONG_DATA == data_size)
            {
                /* Casting void type to uint32_t type is valid */
                ((uint32_t *)p_dst)[g_rspia_tcb[chan].rx_count] = g_rxdata[cnt];
            }
            g_rspia_tcb[chan].rx_count++;
        }

        /* Check the next amount of data to be received and adjust the RX threshold */
        if (g_rspia_tcb[chan].rx_curr_thresh >= (g_rspia_tcb[chan].xfr_length - g_rspia_tcb[chan].rx_count))
        {
            thresh_temp = g_rspia_tcb[chan].xfr_length - g_rspia_tcb[chan].rx_count;
            if (0 == thresh_temp)
            {
                /* Casting to uint8_t is valid */
                (*g_rspia_channels[chan]).SPFCR.BIT.RTRG = (uint8_t)g_rspia_tcb[chan].rx_curr_thresh;
            }
            else
            {
                /* Casting to uint8_t is valid */
                (*g_rspia_channels[chan]).SPFCR.BIT.RTRG = (uint8_t)(thresh_temp - 1);
            }
        }
    }

    /* Check for last data. */
    if (g_rspia_tcb[chan].rx_count == g_rspia_tcb[chan].xfr_length)
    {
        /* Last data was transferred. */
        /* Disable SPRI interrupt and RSPIA operation. */
        (*g_rspia_channels[chan]).SPCR.LONG &= (uint32_t)(~(RSPIA_SPCR_SPRIE | RSPIA_SPCR_SPE));

#if RSPIA_CFG_REQUIRE_LOCK
        /* Release lock for this channel. */
        R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_RSPIA0 + chan));
#endif

        /* Transfer complete. Call the user callback function passing pointer to the result structure. */
        if ((FIT_NO_FUNC != g_rspia_handles[chan].p_callback) && (NULL != g_rspia_handles[chan].p_callback))
        {
            g_rspia_cb_data[chan].hdl   = &(g_rspia_handles[chan]);
            g_rspia_cb_data[chan].event = RSPIA_EVT_TRANSFER_COMPLETE;

            /* Casting pointer to void* type is valid */
            g_rspia_handles[chan].p_callback((void*)&(g_rspia_cb_data[chan]));
        }
    }

    /* When the settings of receive data are completed, set the SPSR.SPRF flag to 0. */
    if ((*g_rspia_channels[chan]).SPSR.WORD & RSPIA_SPSR_SPRF)
    {
        (*g_rspia_channels[chan]).SPSCLR.WORD |= RSPIA_SPSCLR_SPRFC;
    }
}
/******************************************************************************
 End of function rspia_receive
 *****************************************************************************/

/******************************************************************************
* Function Name:    rspia_rx_common
* Description  :    common ISR handler for SPRI
* Arguments    :    RSPIA channel
* Return Value :    N/A
******************************************************************************/
void rspia_rx_common(uint8_t chan)
{
    /* RSPIA receive */
    rspia_receive(chan);
}
/******************************************************************************
 End of function rspia_rx_common
 *****************************************************************************/

#if RSPIA_CFG_USE_CH0
R_BSP_PRAGMA_STATIC_INTERRUPT (rspia0_spri_isr, VECT(RSPIA0, SPRI))
/******************************************************************************
* Function Name:    rspia0_spri_isr
* Description  :    RSPIA SPRI receive buffer full ISR.
*                   Each ISR calls a common function but passes its channel number.
* Arguments    :    N/A
* Return Value :    N/A
******************************************************************************/
R_BSP_ATTRIB_STATIC_INTERRUPT void rspia0_spri_isr(void)
{
    rspia_rx_common(RSPIA_CH0);
}
/******************************************************************************
 End of function rspia0_spri_isr
 *****************************************************************************/
#endif


#if RSPIA_CFG_USE_CH0
R_BSP_PRAGMA_STATIC_INTERRUPT (rspia0_spti_isr, VECT(RSPIA0, SPTI))
/******************************************************************************
* Function Name:    rspia0_spti_isr
* Description  :    RSPIA SPTI transmit buffer empty ISR.
*                   Each ISR calls a common function but passes its channel number.
* Arguments    :    N/A
* Return Value :    N/A
******************************************************************************/
R_BSP_ATTRIB_STATIC_INTERRUPT void rspia0_spti_isr(void)
{
    rspia_tx_common(RSPIA_CH0);
}
/******************************************************************************
 End of function rspia0_spti_isr
 *****************************************************************************/
#endif


/******************************************************************************
* Function Name:    rspia_spei_isr_common
* Description  :    common ISR handler for SPEI RSPI-error
* Arguments    :    RSPI channel
* Return Value :    N/A
******************************************************************************/
static void rspia_spei_isr_common(uint8_t chan)
{
    uint16_t    status_flags = (*g_rspia_channels[chan]).SPSR.WORD;
    rspia_evt_t event        = RSPIA_EVT_ERR_UNDEF;

    /* Identify and clear error condition. */
    if (status_flags & RSPIA_SPSR_OVRF)     /* Overrun error. */
    {
        if (RSPIA_EVT_ERR_UNDEF == event)
        {
            event = RSPIA_EVT_ERR_READ_OVF;
        }

        /* Clear error source: OVRF flag. */
        (*g_rspia_channels[chan]).SPSCLR.BIT.OVRFC = 1U;
    }
    
    if (status_flags & RSPIA_SPSR_MODF)
    {
        if (status_flags & RSPIA_SPSR_UDRF)
        {
            if (RSPIA_EVT_ERR_UNDEF == event)
            {
                event = RSPIA_EVT_ERR_UNDER_RUN;
            }

            /* Clear error source: MODF flag and UDRF. */
            (*g_rspia_channels[chan]).SPSCLR.WORD |= RSPIA_SPSCLR_MODF_UDRF_MASK;
        }
        else
        {
            if (RSPIA_EVT_ERR_UNDEF == event)
            {
                event = RSPIA_EVT_ERR_MODE_FAULT;
            }

            /* Clear error source: MODF flag. */
            (*g_rspia_channels[chan]).SPSCLR.BIT.MODFC = 1U;
        }
    }
    
    if (status_flags & RSPIA_SPSR_PERF)
    {
        if (RSPIA_EVT_ERR_UNDEF == event)
        {
            event = RSPIA_EVT_ERR_PARITY;
        }

        /* Clear error source: PERF flag. */
        (*g_rspia_channels[chan]).SPSCLR.BIT.PERFC = 1U;
    }

    g_rspia_cb_data[chan].event = event;

    /* Disable the RSPIA operation. */
    (*g_rspia_channels[chan]).SPCR.LONG &= (uint32_t)(~((RSPIA_SPCR_SPTIE | RSPIA_SPCR_SPRIE) | RSPIA_SPCR_SPE));
    if (0 == (*g_rspia_channels[chan]).SPCR.LONG)
    {
        R_BSP_NOP();
    }

#if RSPIA_CFG_REQUIRE_LOCK
    /* Release lock for this channel. */
    R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_RSPIA0 + chan));
#endif

    /* Call the user callback function passing pointer to the result structure. */
    if ((FIT_NO_FUNC != g_rspia_handles[chan].p_callback) && (NULL != g_rspia_handles[chan].p_callback))
    {
        g_rspia_cb_data[chan].hdl = &(g_rspia_handles[chan]);

        /* Casting pointer to void* type is valid */
        g_rspia_handles[chan].p_callback((void*)&(g_rspia_cb_data[chan]));
    }
}
/******************************************************************************
 End of function rspia_spei_isr_common
 *****************************************************************************/

/******************************************************************************
* Function Name:    rspia_spei_grp_isr
* Description  :    BSP group interrupt handler for register the error callback function
* Arguments    :    *pdata
* Return Value :    N/A
******************************************************************************/
void rspia_spei_grp_isr(void * pdata)
{
    /* Called from BSP group interrupt handler. */
#if RSPIA_CFG_USE_CH0
    /* RSPIA error */
    if (IS(RSPIA0, SPEI))
    {
        rspia_spei_isr_common(RSPIA_CH0);
    }
#endif
}
/******************************************************************************
 End of function rspia_spei_grp_isr
 *****************************************************************************/
