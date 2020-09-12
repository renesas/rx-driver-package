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
 * Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
* File Name    : r_scif_rx.c
* Description  : SCI FIFO API functions.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           31.08.2014 1.00    Initial Release
*           16.03.2017 1.10    Modified code order in teif_handler() that allowed a few extra clocks to be sent in
*                               SYNC mode when operating at high speed bit rates due to RE still being enabled.
*                               Verified fixed up to max bit rate of 12 Mbps.
*           04.22.2019 1.22    Delete inline definition of GetVersion function
*           01.04.2019 1.23    Added "WAIT_LOOP" keyword.
*           11.11.2019 2.00    Added support for GNUC and ICCRX.
*                              Fixed coding style.
***********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
/* Access to peripherals and board defines. */
#include <string.h>     /* memset() */
#include "platform.h"
/* Defines for SCIF support */
#include "r_scif_rx_private.h"
#include "r_scif_rx_if.h"
#include "r_scif_rx_config.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#define SCIF_PRV_TX_FIFO_SIZE        (16)
#define SCIF_PRV_TX_FIFO_USED_CNT    (hdl->p_rom->regs->FDR.BIT.T)
#define SCIF_PRV_TX_FIFO_THRESHOLD   (hdl->p_rom->regs->FTCR.BIT.TFTC)

#define SCIF_PRV_RX_FIFO_SIZE        (16)
#define SCIF_PRV_RX_FIFO_USED_CNT    (hdl->p_rom->regs->FDR.BIT.R)
#define SCIF_PRV_RX_FIFO_THRESHOLD   (hdl->p_rom->regs->FTCR.BIT.RFTC)

/* Mask of all active channels */
#define SCIF_PRV_CFG_CH_INCLUDED_MASK  ((SCIF_CFG_CH8_INCLUDED << 8)   | \
                                        (SCIF_CFG_CH9_INCLUDED << 9)   | \
                                        (SCIF_CFG_CH10_INCLUDED << 10) | \
                                        (SCIF_CFG_CH11_INCLUDED << 11))

/* SCIF SCR register masks */
#define SCIF_PRV_SCR_TIE_MASK       (0x80U)     /* TXIF enable */
#define SCIF_PRV_SCR_TEIE_MASK      (0x04U)     /* TEIF enable */
#define SCIF_PRV_SCR_RIE_MASK       (0x40U)     /* RXIF/ERIF/BRIF/DRIF enable */
#define SCIF_PRV_SCR_REIE_MASK      (0x08U)     /* ERIF/BRIF enable */
#define SCIF_PRV_SCR_TE_MASK        (0x20U)     /* transmission enable */
#define SCIF_PRV_SCR_RE_MASK        (0x10U)     /* reception enable */

/* SCIF LSR and FSR register receiver error masks */
#define SCIF_PRV_LSR_ORER_MASK      (0x01U)     /* overflow error */
#define SCIF_PRV_SSR_PER_MASK       (0x04U)     /* parity err; read only */
#define SCIF_PRV_SSR_FER_MASK       (0x08U)     /* framing error; read only */
#define SCIF_PRV_SSR_BREAK_MASK     (0x10U)     /* break rcvd */
#define SCIF_PRV_RCVR_ERR_MASK      (0x80U)     /* framing/parity r/w */

/* Macros to enable and disable ICU interrupts */
#define SCIF_PRV_ENABLE_TEIF_ICU_INT      (ICU.GENAL0.LONG |= hdl->p_rom->teif_ch_mask)
#define SCIF_PRV_DISABLE_TEIF_ICU_INT     (ICU.GENAL0.LONG &= ~hdl->p_rom->teif_ch_mask)
#define SCIF_PRV_ENABLE_ERIF_ICU_INT      (ICU.GENAL0.LONG |= hdl->p_rom->erif_ch_mask)
#define SCIF_PRV_DISABLE_ERIF_ICU_INT     (ICU.GENAL0.LONG &= ~hdl->p_rom->erif_ch_mask)
#define SCIF_PRV_ENABLE_BRIF_ICU_INT      (ICU.GENAL0.LONG |= hdl->p_rom->brif_ch_mask)
#define SCIF_PRV_DISABLE_BRIF_ICU_INT     (ICU.GENAL0.LONG &= ~hdl->p_rom->brif_ch_mask)
#define SCIF_PRV_ENABLE_RXIF_ICU_INT      (*hdl->p_rom->p_ier_rxif |= hdl->p_rom->rxif_en_mask)
#define SCIF_PRV_DISABLE_RXIF_ICU_INT     (*hdl->p_rom->p_ier_rxif &= (uint8_t)~hdl->p_rom->rxif_en_mask)
#define SCIF_PRV_ENABLE_TXIF_ICU_INT      (*hdl->p_rom->p_ier_txif |= hdl->p_rom->txif_en_mask)
#define SCIF_PRV_DISABLE_TXIF_ICU_INT     (*hdl->p_rom->p_ier_txif &= (uint8_t)~hdl->p_rom->txif_en_mask)

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/
typedef struct st_scif_priorities
{
    uint8_t     txif;
    uint8_t     rxif;
    uint8_t     group;      /* teif, erif, brif, drif */
} scif_priority_t;

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
#if TESTING_ON
uint8_t rxi_cnt=0;
uint8_t txi_cnt=0;
uint8_t tei_cnt=0;
#endif

/* Define global variables for accessing the TXIF/RXIF interrupt counts. These can be
 * used to observe the affects of modifying the SCIF_CFG_CHx_SCIF_PRV_TX_FIFO_THRESHOLD &
 * SCIF_CFG_CHx_SCIF_PRV_RX_FIFO_THRESHOLD configuration values in r_scif_r_config.h. */
volatile uint16_t g_txif_interrupt_count = 0;
volatile uint16_t g_rxif_interrupt_count = 0;

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
#if (SCIF_CFG_ASYNC_INCLUDED)
static scif_err_t scif_init_async(scif_hdl_t const hdl, scif_uart_t * const p_cfg, scif_priority_t * const p_priority);
static scif_err_t scif_async_receive(scif_hdl_t const hdl, uint8_t *p_dst, uint16_t const length);
static scif_err_t scif_async_cmds(scif_hdl_t const hdl, scif_cmd_t const cmd, void *p_args);
static void erif_handler(scif_hdl_t const hdl);
static void brif_handler(scif_hdl_t const hdl);
#endif /* ASYNC INCLUDED */

#if (SCIF_CFG_SYNC_INCLUDED)
static scif_err_t scif_init_sync(scif_hdl_t const hdl, scif_sync_t * const p_cfg, scif_priority_t * const p_priority);
static scif_err_t scif_sync_cmds(scif_hdl_t const hdl, scif_cmd_t const cmd, void *p_args);
#endif

static uint32_t s_pclka_speed;    /* saved peripheral clock speed for break generation */

static void power_on(scif_hdl_t const hdl);
static void power_off(scif_hdl_t const hdl);
static int32_t scif_init_bit_rate(scif_hdl_t const hdl, uint32_t const pclk, uint32_t const baud);
static void scif_enable_ints(scif_hdl_t const hdl, scif_priority_t * const priority);
static scif_err_t scif_send_data(scif_hdl_t const hdl, uint8_t *p_src, uint8_t *p_dst, uint16_t const length);
static void txif_handler(scif_hdl_t const hdl);
static void rxif_handler(scif_hdl_t const hdl);
static void scif_reset_tx(scif_hdl_t const hdl);
static void scif_reset_rx(scif_hdl_t const hdl, bool enable_rx);
static void teif_handler(scif_hdl_t const hdl);

R_BSP_PRAGMA_STATIC_INTERRUPT(scif8_txif8_isr, VECT(SCIFA8,TXIF8))
R_BSP_PRAGMA_STATIC_INTERRUPT(scif9_txif9_isr, VECT(SCIFA9,TXIF9))
R_BSP_PRAGMA_STATIC_INTERRUPT(scif10_txif10_isr, VECT(SCIFA10,TXIF10))
R_BSP_PRAGMA_STATIC_INTERRUPT(scif11_txif11_isr, VECT(SCIFA11,TXIF11))
R_BSP_PRAGMA_STATIC_INTERRUPT(scif8_rxif8_isr, VECT(SCIFA8,RXIF8))
R_BSP_PRAGMA_STATIC_INTERRUPT(scif9_rxif9_isr, VECT(SCIFA9,RXIF9))
R_BSP_PRAGMA_STATIC_INTERRUPT(scif10_rxif10_isr, VECT(SCIFA10,RXIF10))
R_BSP_PRAGMA_STATIC_INTERRUPT(scif11_rxif11_isr, VECT(SCIFA11,RXIF11))

/**********************************************************************************************************************
 * Function Name: R_SCIF_Open
 ******************************************************************************************************************//**
 * @brief This function applies power to the SCIF channel, initializes the associated registers, enables interrupts, 
 * and provides the channel handle for use with other API functions.
 * @param[in] chan Channel to initialize; 8-11
 * @param[in] mode Operational mode
 * @param[in] p_cfg Pointer to configuration union, structure elements are specific to mode.
 * @param[out] p_callback Optional pointer to function called from interrupt when a message send/receive completes or 
 * receiver error occurs.
 * @param[in, out] p_hdl Pointer to a handle for channel (value set here)
 * @retval SCIF_SUCCESS Successful; channel initialized
 * @retval SCIF_ERR_BAD_CHAN Channel number is invalid for part
 * @retval SCIF_ERR_OMITTED_CHAN Corresponding SCIF_CHx_INCLUDED is 0
 * @retval SCIF_ERR_CH_NOT_CLOSED Channel currently in operation; Perform R_SCIF_Close() first
 * @retval SCIF_ERR_BAD_MODE Specified mode not currently supported
 * @retval SCIF_ERR_NULL_PTR p_cfg or p_hdl pointer is NULL
 * @retval SCIF_ERR_INVALID_ARG An element of the p_cfg structure contains an invalid value
 * @details Initializes an SCIF channel for a particular mode and provides a handle in *p_hdl for use with other API 
 * functions. All applicable interrupts are enabled.
 * @note The driver uses an algorithm for calculating the optimum values for BRR, MDDR, SEMR.ABCS0, 
 * SEMR.BGDM and SMR.CKS using BSP_PCLKA_HZ as defined in mcu_info.h of the board support package. 
 * This however does not guarantee a low bit error rate for all peripheral clock/baud rate combinations. \n
 * The application must wait one bit-time after calling Open() before sending/receiving to allow the clock to 
 * settle. \n
 * If an external clock is used in Asynchronous mode, the Pin Function Select and port pins must be initialized 
 * first. \n
 * The callback function has a single argument. This is a pointer to a structure which is cast to a void pointer 
 * (provides consistency with other FIT module callback functions). \n
 * The events SCIF_EVT_FRAMING_ERR and SCIF_EVT_PARITY_ERR indicate that the next byte to be read from the FIFO 
 * has an error. This byte is not passed to the callback function but is loaded into the receive buffer. 
 * This is so the bytes read from the FIFO will match the requested count.
 */
scif_err_t R_SCIF_Open(uint8_t chan, scif_mode_t const mode, scif_cfg_t * const p_cfg,
                        void (* const p_callback)(void *p_args), scif_hdl_t * const p_hdl)
{
    scif_err_t      err;
    scif_priority_t priority;

    /* CHECK ARGUMENTS */
    chan -= 8;      /* channels 8-11 map to array 0-3 */

#if (SCIF_CFG_PARAM_CHECKING_ENABLE == 1)
    if (chan > 3)
    {
        return SCIF_ERR_BAD_CHAN;
    }
    if (NULL == g_scif_handles[chan])
    {
        return SCIF_ERR_OMITTED_CHAN;
    }
    if (SCIF_MODE_OFF != g_scif_handles[chan]->mode)
    {
        return SCIF_ERR_CH_NOT_CLOSED;
    }
    if ((SCIF_MODE_OFF == mode) || (SCIF_MODE_END_ENUM <= mode))
    {
        return SCIF_ERR_BAD_MODE;
    }
    if ((NULL == p_cfg) || (NULL == p_hdl) || (FIT_NO_PTR == p_cfg) || (FIT_NO_PTR == p_hdl))
    {
        return SCIF_ERR_NULL_PTR;
    }
#endif

    /* apply power to channel */
    power_on(g_scif_handles[chan]);

    /* Initialize mode specific features */
    g_scif_handles[chan]->mode = mode;

    /* Init err to bad mode, which will also be returned if a mode is requested which has not
     * been enabled in the config file. */
    err = SCIF_ERR_BAD_MODE;

    if (SCIF_MODE_ASYNC == mode)
    {
#if (SCIF_CFG_ASYNC_INCLUDED)
        /* Casting is valid because it matches the type to the right side or argument. */
        err = scif_init_async(g_scif_handles[chan], (scif_uart_t *)p_cfg, &priority);
#endif
    }
    else if (SCIF_MODE_SYNC == mode)
    {
#if (SCIF_CFG_SYNC_INCLUDED)
        /* Casting is valid because it matches the type to the right side or argument. */
        err = scif_init_sync(g_scif_handles[chan], (scif_sync_t *)p_cfg, &priority);
#endif
    }
    else
    {

        /* Do nothing */
    }

    if (SCIF_SUCCESS != err)
    {
        g_scif_handles[chan]->mode = SCIF_MODE_OFF;
        return err;
    }
    g_scif_handles[chan]->callback = p_callback;

    /* enable interrupts */
    scif_enable_ints(g_scif_handles[chan], &priority);

    /* finish */
    *p_hdl = g_scif_handles[chan];

    return SCIF_SUCCESS;
}
/**********************************************************************************************************************
 End of function R_SCIF_Open
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: power_on
* Description  : This function provides power to the channel referenced by
*                the handle by taking it out of the module stop state.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
* Return Value : none
 *********************************************************************************************************************/
static void power_on(scif_hdl_t const hdl)
{
    bsp_int_ctrl_t int_ctrl;

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
    SYSTEM.MSTPCRC.LONG &= (~hdl->p_rom->stop_mask);
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    return;
}
/**********************************************************************************************************************
 End of function power_on
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: power_off
* Description  : This function removes power to the channel referenced by
*                handle by putting it into the module stop state.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
* Return Value : none
 *********************************************************************************************************************/
static void power_off(scif_hdl_t const hdl)
{
    bsp_int_ctrl_t int_ctrl;

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
    SYSTEM.MSTPCRC.LONG |= hdl->p_rom->stop_mask;
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    return;
}
/**********************************************************************************************************************
 End of function power_off
 *********************************************************************************************************************/

#if (SCIF_CFG_ASYNC_INCLUDED)
/**********************************************************************************************************************
* Function Name: scif_init_async
* Description  : This function initializes the control block and UART 
*                registers for an SCIF channel.
*
* NOTE: p_cfg is checked to be non-NULL prior to this function.
*       The TE and RE bits in SCR must be 0 prior to calling this function.
*
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
*                p_cfg -
*                    ptr to Uart configuration argument structure
*                p_priority -
*                    pointer to location to load interrupt priority into
* Return Value : SCIF_SUCCESS -
*                    channel initialized successfully
*                SCIF_ERR_INVALID_ARG -
*                    element of p_cfg contains illegal value
 *********************************************************************************************************************/
static scif_err_t scif_init_async(scif_hdl_t const hdl, scif_uart_t * const p_cfg, scif_priority_t * const p_priority)
{
    scif_err_t  err=SCIF_SUCCESS;
    int32_t     bit_err;

    /* Check arguments */    

#if (SCIF_CFG_PARAM_CHECKING_ENABLE == 1)
    if (((SCIF_DATA_7BIT != p_cfg->data_size) && (SCIF_DATA_8BIT != p_cfg->data_size))
        || ((SCIF_STOPBITS_1 != p_cfg->stop_bits) && (SCIF_STOPBITS_2 != p_cfg->stop_bits))
        || ((1 > p_cfg->txif_priority) || (BSP_MCU_IPL_MAX < p_cfg->txif_priority))
        || ((1 > p_cfg->rxif_priority) || (BSP_MCU_IPL_MAX < p_cfg->rxif_priority))
        || ((1 > p_cfg->group_priority) || (BSP_MCU_IPL_MAX < p_cfg->group_priority))
        ||  (p_cfg->group_priority <= p_cfg->rxif_priority))
    {
        return SCIF_ERR_INVALID_ARG;
    }

    if (SCIF_PARITY_ON == p_cfg->parity_en)
    {
        if ((SCIF_EVEN_PARITY != p_cfg->parity_type) && (SCIF_ODD_PARITY != p_cfg->parity_type))
        {
            return SCIF_ERR_INVALID_ARG;
        }
    }
    else if (SCIF_PARITY_OFF != p_cfg->parity_en)
    {
        return SCIF_ERR_INVALID_ARG;
    }
    else
    {

        /* Do nothing */
    }

    if (SCIF_CLK_INT == p_cfg->clk_src)
    {
        if (0 == p_cfg->baud_rate)
        {
            return SCIF_ERR_INVALID_ARG;
        }
    }
    else if ((SCIF_CLK_EXT8X != p_cfg->clk_src) && (SCIF_CLK_EXT16X != p_cfg->clk_src))
    {
        return SCIF_ERR_INVALID_ARG;
    }
    else
    {

        /* Do nothing */
    }
#endif /* (SCIF_CFG_PARAM_CHECKING_ENABLE == 1) */

    /* initialize channel control block and FIFOs */
    scif_init_prologue(hdl);

    /* save ICU interrupt priorities */
    p_priority->txif = p_cfg->txif_priority;
    p_priority->rxif = p_cfg->rxif_priority;
    p_priority->group = p_cfg->group_priority;

    /* configure SMR for Async mode */
    if (SCIF_PARITY_OFF == p_cfg->parity_en)
    {
        p_cfg->parity_type = SCIF_EVEN_PARITY;  /* ensure random value is not ORed into SMR */
    }

    /* Casting is valid because it matches the type to the right side or argument. */
    hdl->p_rom->regs->SMR.WORD = ((uint16_t)(p_cfg->data_size) | (uint16_t)(p_cfg->stop_bits) | 
                                    (uint16_t)(p_cfg->parity_en) | (uint16_t)(p_cfg->parity_type));

    /* setup baud rate clocks */
    s_pclka_speed = BSP_PCLKA_HZ;                 /* save for break generation */
    if (SCIF_CLK_INT == p_cfg->clk_src)
    {

        /* Use internal clock for baud rate */
        bit_err = scif_init_bit_rate(hdl, BSP_PCLKA_HZ, p_cfg->baud_rate);
        if (1000 == bit_err)
        {
            err = SCIF_ERR_INVALID_ARG;         /* impossible baud rate; 100% error */
        }
        else
        {
            hdl->baud_rate = p_cfg->baud_rate;  /* save baud rate for break generation */
        }
    }
    else /* Use external clock for baud rate */
    {
        hdl->p_rom->regs->SCR.BIT.CKE = 0x02;
        hdl->p_rom->regs->SEMR.BIT.ABCS0 = (SCIF_CLK_EXT8X == p_cfg->clk_src) ? 1 : 0;
    }

    /* set options and thresholds */
    scif_init_epilogue(hdl, false);

    return err;
}
/**********************************************************************************************************************
 End of function scif_init_async
 *********************************************************************************************************************/
#endif /* SCIF_CFG_ASYNC_INCLUDED */

/**********************************************************************************************************************
* Function Name: scif_init_prologue
* Description  : This function handles starting initializations common to
*                both async and sync modes.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
* Return Value : none
 *********************************************************************************************************************/
void scif_init_prologue(scif_hdl_t const hdl)
{
    volatile uint16_t    dummy;     /* volatile to remove "not used" warning */

    /* Initialize channel control block */
    hdl->tx_idle = true;
    hdl->tx_break = false;
    memset(&hdl->queue[0], 0, sizeof(scif_fifo_ctrl_t));
    memset(&hdl->queue[1], 0, sizeof(scif_fifo_ctrl_t));

    /* suspend FIFOs */
    hdl->p_rom->regs->SCR.WORD = 0;
    hdl->p_rom->regs->FCR.BIT.TFRST = 1;
    hdl->p_rom->regs->FCR.BIT.RFRST = 1;

    /* clear peripheral error flags */
    dummy = hdl->p_rom->regs->FSR.WORD;
    hdl->p_rom->regs->FSR.WORD = 0;
    dummy = hdl->p_rom->regs->LSR.WORD;
    hdl->p_rom->regs->LSR.BIT.ORER = 0;
}
/**********************************************************************************************************************
 End of function scif_init_prologue
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif_init_epilogue
* Description  : This function handles ending initializations common to
*                both async and sync modes.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                msb_first -
*                    true if MSB should be transmitted first
* Return Value : none
 *********************************************************************************************************************/
void scif_init_epilogue(scif_hdl_t const hdl, bool msb_first)
{

    /* init options */
    hdl->p_rom->regs->SEMR.BIT.NFEN = 0;          /* disable noise filter */
    hdl->p_rom->regs->FCR.BIT.MCE = 0;            /* disable flow control */
    hdl->p_rom->regs->SEMR.BIT.DIR = (uint8_t)((true == msb_first) ? 1 : 0);

    /* use FTCR (not FCR) for threshold levels */
    hdl->p_rom->regs->FTCR.BIT.TTRGS = 1;
    hdl->p_rom->regs->FTCR.BIT.RTRGS = 1;

    /* set threshold levels */
    SCIF_PRV_TX_FIFO_THRESHOLD = hdl->p_rom->tx_dflt_thresh;
    SCIF_PRV_RX_FIFO_THRESHOLD = 1;

    /* reset suspended FIFOs */
    hdl->p_rom->regs->FCR.BIT.TFRST = 0;
    hdl->p_rom->regs->FCR.BIT.RFRST = 0;

    /* set tx data line hi for idle (async; should not matter for sync) */
    if (SCIF_MODE_ASYNC == hdl->mode)
    {
        hdl->p_rom->regs->SPTR.BIT.SPB2IO = 1;
        hdl->p_rom->regs->SPTR.BIT.SPB2DT = 1;        /* level */
    }
}
/**********************************************************************************************************************
 End of function scif_init_epilogue
 *********************************************************************************************************************/

#if (SCIF_CFG_SYNC_INCLUDED)
/**********************************************************************************************************************
* Function Name: scif_init_sync
* Description  : This function initializes the control block and SYNC
*                registers for an SCIF channel.
*
* NOTE: p_cfg is checked to be non-NULL prior to this function.
*       The TE and RE bits in SCR must be 0 prior to calling this function.
*
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                p_cfg -
*                    ptr to SSPI configuration argument structure
*                p_priority -
*                    pointer to location to load interrupt priority into
* Return Value : SCIF_SUCCESS -
*                    channel initialized successfully
*                SCIF_ERR_INVALID_ARG -
*                    element of p_cfg contains illegal value
 *********************************************************************************************************************/
static scif_err_t scif_init_sync(scif_hdl_t const hdl, scif_sync_t * const p_cfg, scif_priority_t * const p_priority)
{
    scif_err_t  err=SCIF_SUCCESS;
    int32_t     bit_err;

    /* Check arguments */

#if (SCIF_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((p_cfg->bit_rate == 0) || (p_cfg->int_priority == 0) || (p_cfg->int_priority > BSP_MCU_IPL_MAX))
    {
        return SCIF_ERR_INVALID_ARG;
    }
#endif

    /* Initialize channel control block and FIFOs */
    scif_init_prologue(hdl);

    /* save ICU interrupt priorities */
    p_priority->txif = p_cfg->int_priority;
    p_priority->rxif = p_cfg->int_priority;
    p_priority->group = p_cfg->int_priority;

    /* configure SMR for Sync mode */
    hdl->p_rom->regs->SMR.WORD = 0x0080;

    /* set up bit rate clock */
    s_pclka_speed = BSP_PCLKA_HZ;                 /* save for break generation */
    bit_err = scif_init_bit_rate(hdl, BSP_PCLKA_HZ, p_cfg->bit_rate);
    if (bit_err == 1000)
    {
        err = SCIF_ERR_INVALID_ARG;          /* impossible baud rate; 100% error */
    }

    /* set options and thresholds */
    scif_init_epilogue(hdl, p_cfg->msb_first);

    return err;
}
/**********************************************************************************************************************
 End of function scif_init_sync
 *********************************************************************************************************************/
#endif /* (SCIF_CFG_SYNC_INCLUDED) */

/**********************************************************************************************************************
* Function Name: scif_init_bit_rate
* Description  : This function determines the best possible settings for the
*                baud rate registers for the specified peripheral clock speed
*                and baud rate. Note that this does not guarantee a low bit 
*                error rate, just the best possible one. The bit rate error is
*                returned in .1% increments. If the hardware cannot support
*                the specified combination, a value of 1000 (100% error) is
*                returned.
*
* NOTE: The transmitter and receiver (TE and RE bits in SCR) must be disabled 
*       prior to calling this function.
*
*       The application must pause for 1 bit time after the BRR register
*       is loaded before transmitting/receiving to allow time for the clock
*       to settle. 
*
* Arguments    : hdl -
*                    Handle for channel (ptr to chan control block)
*                    NOTE: mode element must be already set
*                pclk -
*                    Peripheral clock speed; e.g. 24000000 for 24MHz
*                baud -
*                    Baud rate; 19200, 57600, 115200, etc.
* Return Value : bit error in .1% increments; e.g. 16 = 1.6% bit rate error
*                a value of 1000 denotes 100% error; no registers set
 *********************************************************************************************************************/
static int32_t scif_init_bit_rate(scif_hdl_t const hdl, uint32_t const pclk, uint32_t const baud)
{
    uint32_t i;
    uint32_t num_divisors=0;
    uint32_t ratio;
    uint32_t tmp;
    uint32_t divisor;
    uint32_t int_m;
    baud_divisor_t const *p_baud_info=NULL;
    float float_m;
    float error;
    float abs_error;

#if (SCIF_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((0 == pclk) || (0 == baud))
    {
        return 1000;
    }
#endif

    /* SELECT PROPER TABLE BASED UPON MODE */

    if (SCIF_MODE_ASYNC == hdl->mode)
    {
#if (SCIF_CFG_ASYNC_INCLUDED)
        p_baud_info = g_scif_async_baud;
        num_divisors = NUM_DIVISORS_ASYNC;
#endif
    }
    else /* SYNC or SSPI */
    {
#if (SCIF_CFG_SYNC_INCLUDED)
        p_baud_info = g_scif_sync_baud;
        num_divisors = NUM_DIVISORS_SYNC;
#endif
    }

    /* FIND DIVISOR; table has associated ABCS0, BGDM and CKS values */

    /* BRR must be 255 or less */
    /* the "- 1" is ignored in some steps for approximations */
    /* BRR = (PCLK/(divisor * baud)) - 1 */
    /* BRR = (ratio / divisor) - 1 */

    ratio = pclk/baud;

    /* WAIT_LOOP */
    for(i=0; i < num_divisors; i++)
    {

        /* Casting is valid because it matches the type to the right side or argument. */
        if (ratio < (uint32_t)(p_baud_info[i].divisor * 256))
        {
            break;
        }
    }

    /* RETURN IF BRR WILL BE >255 OR LESS THAN 0 */

    if (i == num_divisors)
    {
        return(1000);           /* impossible baud rate requested; return 100% error */
    }

    /* Casting is valid because it matches the type to the right side or argument. */
    divisor = (uint32_t)p_baud_info[i].divisor;
    tmp = ratio/(divisor);      /* tmp = PCLK/(baud * divisor) = BRR+1 = N+1 */
    if(0 == tmp)
    {
        return(1000);            /* illegal value; return 100% error */
    }

    /* SET BRR, ABCS0, BDGM, and CKS */

    hdl->p_rom->regs->SEMR.BIT.MDDRS = 0;         /* "bank switch" to BRR */

    tmp = ratio / (divisor/2);  /* divide by half the divisor */

    /* if odd, "round up" by ignoring -1; divide by 2 again for rest of divisor */
    hdl->p_rom->regs->BRR = (uint8_t)((tmp & 0x01) ? (tmp/2) : ((tmp/2)-1));

    hdl->p_rom->regs->SEMR.BIT.ABCS0 = p_baud_info[i].abcs;
    hdl->p_rom->regs->SEMR.BIT.BGDM = p_baud_info[i].bgdm;
    hdl->p_rom->regs->SMR.BIT.CKS = p_baud_info[i].cks;

    /* CALCULATE BIT RATE ERROR.
     * RETURN IF ERROR LESS THAN 1% OR IF IN SYNCHRONOUS/SSPI MODE.
     */
    tmp = ratio/(divisor);      /* tmp = PCLK/(baud * divisor) = BRR+1 = N+1 */
    error = ( ((float)pclk / (baud * divisor * tmp)) - 1) * 100;
    abs_error  = (error < 0) ? (-error) : (error);

    if ((1.0 >= abs_error) || (SCIF_MODE_ASYNC != hdl->mode))
    {
        hdl->p_rom->regs->SEMR.BIT.BRME = 0;          /* disable MDDR */
        return (int32_t)(error*10);
    }

    /* CALCULATE M ASSUMING A 0% ERROR then WRITE REGISTER */

    /* Casting is valid because it matches the type to the right side or argument. */
    hdl->p_rom->regs->BRR = (uint8_t)(tmp-1);

    /* Casting is valid because it matches the type to the right side or argument. */
    float_m = ((float)baud * divisor * 256 * tmp) / pclk;
    float_m *= 2;

    /* Casting is valid because it matches the type to the right side or argument. */
    int_m = (uint32_t)float_m;
    int_m = (int_m & 0x01) ? ((int_m/2) + 1) : (int_m/2);

    hdl->p_rom->regs->SEMR.BIT.MDDRS = 1;         /* "bank switch" from BRR to MDDR */
    hdl->p_rom->regs->MDDR = (uint8_t)int_m;      /* write M */
    hdl->p_rom->regs->SEMR.BIT.BRME = 1;          /* enable MDDR */

    /* Casting is valid because it matches the type to the right side or argument. */
    error = (( (float)(pclk) / (divisor * tmp * baud * ((float)(256)/int_m)) ) - 1) * 100;

    return (int32_t)(error*10);
}
/**********************************************************************************************************************
 End of function scif_init_bit_rate
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif_enable_ints
* Description  : This function sets priority, clears flags, and enables 
*                interrupts in both the ICU and SCI peripheral. These include 
*                RXIF/TXIF and GROUPAL0 interrupts TEIF/ERIF/BRIF/DRIF
*                interrupts.
*
* NOTE: DRIF interrupts will not be supported because they only work for
*       messages less than threshold bytes in length. So if a delay occurs
*       immediately after threshold bytes are received, it will never be
*       detected and feature becomes useless for messages longer than
*       threshold number of bytes.
*
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
*                priority -
*                    pointer to priority structure for interrupts
* Return Value : none
 *********************************************************************************************************************/
static void scif_enable_ints(scif_hdl_t const hdl, scif_priority_t * const priority)
{
    bsp_int_ctrl_t int_ctrl;

    /* SET PRIORITY FOR INTERRUPTS */

    *hdl->p_rom->p_ipr_rxif = priority->rxif;
    *hdl->p_rom->p_ipr_txif = priority->txif;
    if (priority->group > IPR(ICU, GROUPAL0))   /* includes teif, erif, brif, drif */
    {
        IPR(ICU, GROUPAL0) = priority->group;
    }

    /* CLEAR INTERRUPT FLAGS */

    *hdl->p_rom->p_ir_rxif = 0;
    *hdl->p_rom->p_ir_txif = 0;
    ICU.GRPAL0.LONG &= (~hdl->p_rom->teif_ch_mask);
#if SCIF_CFG_ASYNC_INCLUDED
    if (SCIF_MODE_ASYNC == hdl->mode)
    {
        ICU.GRPAL0.LONG &= (~hdl->p_rom->erif_ch_mask);
        ICU.GRPAL0.LONG &= (~hdl->p_rom->brif_ch_mask);
    }
#endif

    /* REGISTER GROUP INTERRUPTS WITH BSP */

    R_BSP_InterruptWrite(hdl->p_rom->teif_vector, hdl->p_rom->teif_isr);
#if SCIF_CFG_ASYNC_INCLUDED
    if (SCIF_MODE_ASYNC == hdl->mode)
    {
        R_BSP_InterruptWrite(hdl->p_rom->erif_vector, hdl->p_rom->erif_isr);
        R_BSP_InterruptWrite(hdl->p_rom->brif_vector, hdl->p_rom->brif_isr);
    }
#endif

    /* ENABLE INTERRUPTS IN ICU */

    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
    SCIF_PRV_ENABLE_TXIF_ICU_INT;                /* enable TXIF interrupt */
    SCIF_PRV_ENABLE_RXIF_ICU_INT;                /* enable RXIF interrupt */
    IEN(ICU,GROUPAL0) = 1;                       /* enable group interrupt */
    SCIF_PRV_ENABLE_TEIF_ICU_INT;                /* enable TEIF sub-group interrupt */
#if SCIF_CFG_ASYNC_INCLUDED
    if (SCIF_MODE_ASYNC == hdl->mode)
    {
        SCIF_PRV_ENABLE_ERIF_ICU_INT;                /*   enable ERIF sub-group interrupt */
        SCIF_PRV_ENABLE_BRIF_ICU_INT;                /*   enable BRIF sub-group interrupt */
    }
#endif
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);

    /* ENABLE PERIPHERAL RECEIVE INTERRUPTS (xmt done in Send()*/

    /* Do not enable receiver for Sync. It clocks in one byte using internal clock even
     * though no external clock line is being toggled. Also, cannot read that data nor
     * clear it by resetting the fifo and get repeated interrupts because byte not read.
     */
#if (SCIF_CFG_ASYNC_INCLUDED)
    if (SCIF_MODE_ASYNC == hdl->mode)
    {
        hdl->p_rom->regs->SCR.BIT.RE = 1;     /* do not combine RE and RIE into word OR */
        hdl->p_rom->regs->SCR.BIT.RIE = 1;
    }
#endif
    return;    
}
/**********************************************************************************************************************
 End of function scif_enable_ints
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCIF_Send
 ******************************************************************************************************************//**
 * @brief Queues up to two requests. Begins transmission if transmitter is not already in use.
 * @param[in] hdl Handle for channel
 * @param[in] p_src Pointer to data to transmit
 * @param[in] length Number of bytes to send
 * @retval SCIF_SUCCESS Message queued for sending; transmission started if transmitter is idle
 * @retval SCIF_ERR_NULL_PTR hdl or p_src is NULL
 * @retval SCIF_ERR_BAD_MODE Channel mode not currently supported
 * @retval SCIF_ERR_INVALID_ARG length is 0
 * @retval SCIF_ERR_BUSY Cannot process request. 2+ requests already placed
 * @details If the driver can process the request, SCIF_SUCCESS is returned. If there are already two requests 
 * outstanding, SCIF_ERR_BUSY is returned. If a message is longer than the FIFO size, the driver will 
 * automatically reload the FIFO at the interrupt level each time the threshold level (set in config.h) is reached. \n
 * When no more data remains to be transmitted, an SCIF_EVT_TX_DONE (Async) or SCIF_EVT_XCV_DONE (Sync) event is 
 * passed to the callback function if specified in Open(). If no callback function was provided, the application 
 * must poll for completion using a Control() command. \n
 * If it is desired to know when each message has completed transmission, do not have more than one Send() 
 * request outstanding at a time. This driver is optimized for streaming data and the "done" event is used to 
 * indicate transmit completion of all data.
 * @note In synchronous mode, the peripheral drives the clock for Send(), Receive(), and SendReceive() messages. 
 * In this mode, at most two transfer requests of any kind can ever be outstanding at a time. Therefore a 
 * SCIF_ERR_BUSY may be returned even when no Send() message was previously issued. \n
 * Do not re-use the same buffer pointed to by p_src until it is known that the previous message the buffer was 
 * used for has completed transmission. Doing so could corrupt the data of the message currently being sent. 
 * This behavior is different than the standard SCI driver which copies the original buffer into a queue where it 
 * waited until it could be transmitted. For high throughput, this driver does not copy data into an intermediate 
 * queue and the hardware FIFO is the only temporary storage mechanism.
 */
scif_err_t R_SCIF_Send(scif_hdl_t const hdl, uint8_t *p_src, uint16_t const length)
{
    scif_err_t   err;

    /* Check arguments */

#if (SCIF_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((NULL == hdl) || (NULL == p_src))
    {
        return SCIF_ERR_NULL_PTR;
    }
    if ((SCIF_MODE_OFF == hdl->mode) || (SCIF_MODE_END_ENUM <= hdl->mode))
    {
        return SCIF_ERR_BAD_MODE;
    }
    if (0 == length)
    {
        return SCIF_ERR_INVALID_ARG;
    }
#endif

    /* transmit p_src data, don't save receive data */
    err = scif_send_data(hdl, p_src, NULL, length);

    return err;
}
/**********************************************************************************************************************
 End of function R_SCIF_Send
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif_send_data
* Description  : This function determines if the channel referenced by the
*                handle is not busy, and begins the data transfer process.
*                This function is used for Async Send() and Sync Send(),
*                Receive(), and SendReceive().
*
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                p_src -
*                    ptr to data to transmit; NULL means transmit dummy byte
*                p_dst -
*                    ptr to storage buffer; NULL means don't store
*                length -
*                    number of bytes to send and possibly receive
* Return Value : SCIF_SUCCESS -
*                    data transfer started
*                SCIF_ERR_BUSY -
*                    channel currently busy
 *********************************************************************************************************************/
static scif_err_t scif_send_data(scif_hdl_t const hdl, uint8_t *p_src, uint8_t *p_dst, uint16_t const length)
{
    bsp_int_ctrl_t int_ctrl;

#if (SCIF_CFG_SYNC_INCLUDED)
    scif_fifo_ctrl_t *p_rctrl;
#endif
    scif_fifo_ctrl_t *p_tctrl;
    scif_err_t        err;

    /* If two requests outstanding or Async break in progress, return busy */
    p_tctrl = &hdl->queue[hdl->qindex_app_tx];
    if ((0 != p_tctrl->tx_cnt) || (true == hdl->tx_break))
    {
        err = SCIF_ERR_BUSY;
    }
    else /* else process current request */
    {

        /* Do NOT attempt to load fifo at application level.
         * Set up arguments and enable transmitter to kick off transmit. */
#if (SCIF_CFG_SYNC_INCLUDED)
        /* Set up rx control. Data ignored when p_dst is NULL. */
        if (hdl->mode == SCIF_MODE_SYNC)
        {
            p_rctrl = &hdl->queue[hdl->qindex_app_rx];
            p_rctrl->p_rx_buf = p_dst;
            p_rctrl->rx_cnt = length;
        }
#endif
        p_tctrl->p_tx_buf = p_src;              /* dummy byte sent when NULL (Sync) */
        p_tctrl->tx_cnt = length;             /* length must be set after buf ptr */

        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
        SCIF_PRV_DISABLE_TXIF_ICU_INT;               /* disable interrupt in icu */
        SCIF_PRV_DISABLE_RXIF_ICU_INT;               /* disable interrupt in icu */
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
        hdl->tx_idle = false;

        /* TE and TIE may already be enabled if another request currently sending */
        /* RE and RIE already enabled if in Async mode; must enable here for Sync */
        hdl->p_rom->regs->SCR.WORD |= (SCIF_PRV_SCR_TE_MASK | SCIF_PRV_SCR_RE_MASK);
        hdl->p_rom->regs->SCR.WORD |= (SCIF_PRV_SCR_TIE_MASK | SCIF_PRV_SCR_RIE_MASK);
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
        SCIF_PRV_ENABLE_RXIF_ICU_INT;
        SCIF_PRV_ENABLE_TXIF_ICU_INT;
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);

        hdl->qindex_app_tx ^= 1;            /* adjust request queue index */
#if (SCIF_CFG_SYNC_INCLUDED)
        if (hdl->mode == SCIF_MODE_SYNC)
        {
            hdl->qindex_app_rx ^= 1;        /* adjust request queue index */
        }
#endif
        err = SCIF_SUCCESS;
    }

    return err;
}
/**********************************************************************************************************************
 End of function scif_send_data
 *********************************************************************************************************************/

#if (SCIF_CFG_SYNC_INCLUDED)
/**********************************************************************************************************************
 * Function Name: R_SCIF_SendReceive
 ******************************************************************************************************************//**
 * @brief For Synchronous mode only. Transmits and receives data simultaneously.
 * @param[in] hdl Handle for channel
 * @param[in] *p_src Pointer to data to transmit
 * @param[out] *p_dst Pointer to buffer to load data into
 * @param[in] length Number of bytes to send
 * @retval SCIF_SUCCESS Data transfer queued and initiated if transceiver idle
 * @retval SCIF_ERR_NULL_PTR hdl value is NULL
 * @retval SCIF_ERR_BAD_MODE Channel mode not Synchronous
 * @retval SCIF_ERR_INVALID_ARG length is 0
 * @retval SCIF_ERR_BUSY Cannot process request. 2+ requests already placed
 * @details This function transmits and receives data simultaneously if the transceiver is not in use. If the driver 
 * can process the request, SCIF_SUCCESS is returned. If there are already two requests outstanding, 
 * SCIF_ERR_BUSY is returned. If a message is longer than the FIFO size, the driver will automatically 
 * process the FIFO at the interrupt level each time the threshold level (set in config.h) is reached. \n
 * When no more data remains to be transmitted and received, an SCIF_EVT_XCV_DONE event is passed to the callback 
 * function if specified in Open(). If no callback function was provided, the application must poll for completion 
 * using a Control() command. \n
 * If it is desired to know when each message has completed transmission/reception, do not have more than one 
 * SendReceive() request outstanding at a time. This driver is optimized for streaming data and the "done" 
 * event is used to indicate transmit/receive completion of all data.
 * @note In synchronous mode, the peripheral drives the clock for Send(), Receive(), and SendReceive() messages. 
 * In this mode, at most two transfer requests of any kind can ever be outstanding at a time. Therefore a 
 * SCIF_ERR_BUSY may be returned even when no SendReceive() message was previously issued.  \n
 * Do not re-use the same buffers pointed to by p_dst and p_dst until it is known that the previous message the 
 * buffer was used for has been processed. Doing so could corrupt the data of the message previously received.
 */
scif_err_t R_SCIF_SendReceive(scif_hdl_t const hdl, uint8_t *p_src, uint8_t *p_dst, uint16_t const length)
{
    scif_err_t   err;

    /* Check arguments */

#if (SCIF_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((hdl == NULL) || (p_src == NULL) || (p_dst == NULL))
    {
        return SCIF_ERR_NULL_PTR;
    }
    if (hdl->mode != SCIF_MODE_SYNC)
    {
        return SCIF_ERR_BAD_MODE;
    }
    if (length == 0)
    {
        return SCIF_ERR_INVALID_ARG;
    }
#endif

    /* transmit p_src data, save receive data */
    err = scif_send_data(hdl, p_src, p_dst, length);

    return err;
}
/**********************************************************************************************************************
 End of function R_SCIF_SendReceive
 *********************************************************************************************************************/
#endif /* (SCIF_CFG_SYNC_INCLUDED) */


/**********************************************************************************************************************
* Function Name: scif8_txif8_isr
* Description  : TXIF interrupt routines for every SCIF channel
* Arguments    : none
* Return Value : none
 *********************************************************************************************************************/
#if SCIF_CFG_CH8_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void scif8_txif8_isr(void)
{
    txif_handler(&g_scif_ch8_ctrl);
}
#endif
/**********************************************************************************************************************
 End of function scif8_txif8_isr
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif9_txif9_isr
* Description  : TXIF interrupt routines for every SCIF channel
* Arguments    : none
* Return Value : none
 *********************************************************************************************************************/
#if SCIF_CFG_CH9_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void scif9_txif9_isr(void)
{
    txif_handler(&g_scif_ch9_ctrl);
}
#endif
/**********************************************************************************************************************
 End of function scif9_txif9_isr
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif10_txif10_isr
* Description  : TXIF interrupt routines for every SCIF channel
* Arguments    : none
* Return Value : none
 *********************************************************************************************************************/
#if SCIF_CFG_CH10_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void scif10_txif10_isr(void)
{
    txif_handler(&g_scif_ch10_ctrl);
}
#endif
/**********************************************************************************************************************
 End of function scif10_txif10_isr
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif11_txif11_isr
* Description  : TXIF interrupt routines for every SCIF channel
* Arguments    : none
* Return Value : none
 *********************************************************************************************************************/
#if SCIF_CFG_CH11_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void scif11_txif11_isr(void)
{
    txif_handler(&g_scif_ch11_ctrl);
}
#endif
/**********************************************************************************************************************
 End of function scif11_txif11_isr
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: txif_handler
* Description  : TXIF interrupt handler for SCIF
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
* Return Value : none
 *********************************************************************************************************************/
static void txif_handler(scif_hdl_t const hdl)
{
    volatile scif_fifo_ctrl_t    *p_ctrl;
#if TESTING_ON
    txi_cnt++;
#endif

    g_txif_interrupt_count++; /* Increment interrupt count */

    /* If space in fifo and data waiting to be sent, load fifo. */
    p_ctrl = &hdl->queue[hdl->qindex_int_tx];
    if (0 != p_ctrl->tx_cnt)
    {

        /* loop until no more data to send or fifo becomes full */
        /* WAIT_LOOP */
        while ((0 != p_ctrl->tx_cnt) && (SCIF_PRV_TX_FIFO_SIZE != SCIF_PRV_TX_FIFO_USED_CNT))
        {
            if (NULL == p_ctrl->p_tx_buf)   /* Sync only */
            {
                #if TESTING_ON
                hdl->p_rom->regs->FTDR = txi_cnt;
                #else
                hdl->p_rom->regs->FTDR = SCIF_CFG_DUMMY_TX_BYTE;
                #endif /* (TESTING_ON) */
            }
            else
            {
                hdl->p_rom->regs->FTDR = *p_ctrl->p_tx_buf;
                p_ctrl->p_tx_buf++;
            }

            p_ctrl->tx_cnt--;

            /* if all of current message loaded, set index to next message */
            if (0 == p_ctrl->tx_cnt)
            {
                hdl->qindex_int_tx ^= 1;
                p_ctrl = &hdl->queue[hdl->qindex_int_tx];
            }
        }
    }

    /* If at threshold level, clear bit (re-arm) so can get another TXIF interrupt. */
    if (1 == hdl->p_rom->regs->FSR.BIT.TDFE)
    {
        hdl->p_rom->regs->FSR.BIT.TDFE = 0;
    }

    /* If no more data to load, just wait for TEIF interrupt
     * instead of adjusting threshold level to 0. */
    if (0 == p_ctrl->tx_cnt)
    {
        hdl->p_rom->regs->SCR.BIT.TIE = 0;        /* disable TXIF */
        hdl->p_rom->regs->SCR.BIT.TEIE = 1;       /* enable TEIF */
    }

}
/**********************************************************************************************************************
 End of function txif_handler
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif8_teif8_isr
* Description  : TEIF interrupt routines for every SCIF channel.
*                BSP gets main group interrupt, then vectors to/calls these "interrupts"/callbacks.
* Arguments    : none
* Return Value : none
 *********************************************************************************************************************/
#if SCIF_CFG_CH8_INCLUDED
void scif8_teif8_isr(void *cb_args)
{
    teif_handler(&g_scif_ch8_ctrl);
}
#endif
/**********************************************************************************************************************
 End of function scif8_teif8_isr
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif9_teif9_isr
* Description  : TEIF interrupt routines for every SCIF channel.
*                BSP gets main group interrupt, then vectors to/calls these "interrupts"/callbacks.
* Arguments    : none
* Return Value : none
 *********************************************************************************************************************/
#if SCIF_CFG_CH9_INCLUDED
void scif9_teif9_isr(void *cb_args)
{
    teif_handler(&g_scif_ch9_ctrl);
}
#endif
/**********************************************************************************************************************
 End of function scif9_teif9_isr
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif10_teif10_isr
* Description  : TEIF interrupt routines for every SCIF channel.
*                BSP gets main group interrupt, then vectors to/calls these "interrupts"/callbacks.
* Arguments    : none
* Return Value : none
 *********************************************************************************************************************/
#if SCIF_CFG_CH10_INCLUDED
void scif10_teif10_isr(void *cb_args)
{
    teif_handler(&g_scif_ch10_ctrl);
}
#endif
/**********************************************************************************************************************
 End of function scif10_teif10_isr
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif11_teif11_isr
* Description  : TEIF interrupt routines for every SCIF channel.
*                BSP gets main group interrupt, then vectors to/calls these "interrupts"/callbacks.
* Arguments    : none
* Return Value : none
 *********************************************************************************************************************/
#if SCIF_CFG_CH11_INCLUDED
void scif11_teif11_isr(void *cb_args)
{
    teif_handler(&g_scif_ch11_ctrl);
}
#endif
/**********************************************************************************************************************
 End of function scif11_teif11_isr
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: teif_handler
* Description  : TEIF interrupt handler for SCIF
* Arguments    : hdl - 
*                    handle for channel (ptr to channel control block)
* Return Value : none
 *********************************************************************************************************************/
static void teif_handler(scif_hdl_t const hdl)
{
scif_cb_args_t   args;
#if TESTING_ON
tei_cnt++;
#endif

    /* While the receiver is enabled it continues to clock in data even
     * when no clock line is driving it. Disable the the receiver here
     * or will get a bit to several bytes (depends on the length of
     * time to execute this routine, the callback, and the beginning of
     * the rxif routine) of data added to the receive FIFO with no RXIF,
     * TXIF, or TEIF interrupt.
     *
     * NOTE: Technically, nothing should be until after TEND is verified as 1.
     * However, that slight delay of a few assembly instructions is sufficient
     * to leave the receiver enabled long enough to transmit a few false clocks.
     * Therefore, RE=0 must always be the first instruction.
     */
#if (SCIF_CFG_SYNC_INCLUDED)
    if (hdl->mode == SCIF_MODE_SYNC)
    {
        hdl->p_rom->regs->SCR.BIT.RE = 0;
        hdl->p_rom->regs->SCR.BIT.RIE = 0;
        SCIF_PRV_RX_FIFO_THRESHOLD = 1;
    }
#endif

    /* Show transmission done for polled applications */
    hdl->tx_idle = true;

    /* Cleanup and prepare for next transmission sequence */
    if (1 == hdl->p_rom->regs->FSR.BIT.TEND)
    {

        /* disable transmitter and TEIF interrupts (TXIF already disabled) */
        hdl->p_rom->regs->SCR.BIT.TE = 0;
        hdl->p_rom->regs->SCR.BIT.TEIE = 0;

        /* make data line idle high so "burp" pulse does not occur when re-enable transmitter */
#if (SCIF_CFG_ASYNC_INCLUDED)
        if (SCIF_MODE_ASYNC == hdl->mode)
        {
            hdl->p_rom->regs->SPTR.BIT.SPB2IO = 1;
            hdl->p_rom->regs->SPTR.BIT.SPB2DT = 1;        /* 1,1 = inactive level high */
        }
#endif
    }

    /* Do callback if one available */
    if ((NULL != hdl->callback) && (FIT_NO_FUNC != hdl->callback))
    {
        args.hdl = hdl;
        args.event = (SCIF_MODE_ASYNC == hdl->mode) ? SCIF_EVT_TX_DONE : SCIF_EVT_XCV_DONE;

        /* Casting is valid because it matches the type to the right side or argument. */
        hdl->callback((void *)&args);
    }

#if (SCIF_CFG_ASYNC_INCLUDED)
    if (true == hdl->tx_break)
    {

        /* restore original baud rate */
        scif_init_bit_rate(hdl, s_pclka_speed, hdl->baud_rate);
        scif_reset_rx(hdl, true);

        hdl->tx_break = false;  /* MUST COME AFTER RESET RX!!! */
    }
#endif

}
/**********************************************************************************************************************
 End of function teif_handler
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCIF_Receive
 ******************************************************************************************************************//**
 * @brief Queues up to two requests. Fetches data from the hardware FIFO. In Synchronous mode, initiates clocking 
 * of data if not already in use.
 * @param[in] hdl Handle for channel
 * @param[out] *p_dst Pointer to buffer to load data into
 * @param[in] length Number of bytes to read
 * @retval SCIF_SUCCESS Request queued. Clocking begins (Sync) if transceiver idle
 * @retval SCIF_ERR_NULL_PTR hdl value is NULL
 * @retval SCIF_ERR_BAD_MODE Channel mode not currently supported
 * @retval SCIF_ERR_INVALID_ARG length is 0
 * @retval SCIF_ERR_BUSY Cannot process request. 2+ requests already placed
 * @details If the driver can process the request, SCIF_SUCCESS is returned. If there are already two requests 
 * outstanding, SCIF_ERR_BUSY is returned. If a message is longer than the FIFO size, the driver will automatically 
 * read from the FIFO at the interrupt level each time the threshold level (set in config.h) is reached. 
 * If there is less than the threshold level bytes remaining the driver automatically adjusts the threshold level. \n
 * When no more data remains to be received, an SCIF_EVT_RX_DONE (Async) or SCIF_EVT_XCV_DONE (Sync) event is 
 * passed to the callback function if specified in Open(). If no callback function was provided, the application 
 * must poll for completion using a Control() command. Note that errors which occurred during reception are only 
 * reported via the callback function. \n
 * If it is desired to know when each message has completed reception, do not have more than one Receive() request 
 * outstanding at a time. This driver is optimized for streaming data and the "done" event is used to indicate 
 * receive completion of all requested data.
 * @note In synchronous mode, the peripheral drives the clock for Send(), Receive(), and SendReceive() messages. 
 * In this mode, at most two transfer requests of any kind can ever be outstanding at a time. Therefore a 
 * SCIF_ERR_BUSY may be returned even when no Receive() message was previously issued. \n
 * Do not re-use the same buffer pointed to by p_dst until it is known that the previous message the buffer was 
 * used for has been processed. Doing so could corrupt the data of the message previously received.
 */
scif_err_t R_SCIF_Receive(scif_hdl_t const hdl, uint8_t *p_dst, uint16_t const length)
{
    scif_err_t   err=SCIF_SUCCESS;

    /* Check arguments */

#if (SCIF_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((NULL == hdl) || (NULL == p_dst))
    {
        return SCIF_ERR_NULL_PTR;
    }
    if ((SCIF_MODE_OFF == hdl->mode) || (SCIF_MODE_END_ENUM <= hdl->mode))
    {
        return SCIF_ERR_BAD_MODE;
    }
    if (0 == length)
    {
        return SCIF_ERR_INVALID_ARG;
    }
#endif

    if (SCIF_MODE_ASYNC == hdl->mode)
    {
#if (SCIF_CFG_ASYNC_INCLUDED)
        err = scif_async_receive(hdl, p_dst, length);
#endif
    }
    else /* mode is SYNC */
    {
#if (SCIF_CFG_SYNC_INCLUDED)
        /* transmit dummy data, save receive data */
        err = scif_send_data(hdl, NULL, p_dst, length);
#endif
    }

    return err;
}
/**********************************************************************************************************************
 End of function R_SCIF_Receive
 *********************************************************************************************************************/

#if (SCIF_CFG_ASYNC_INCLUDED)
/**********************************************************************************************************************
* Function Name: scif_async_receive
* Description  : Posts request if queue space available. Returns SUCCESS if can
*                otherwise returns BUSY. If sufficient data is already in FIFO
*                the data is read here at the application level and an
*                RX DONE event is passed to the callback function. Otherwise
*                all data is read at the interrupt level.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                p_dst -
*                    ptr to buffer to load data into
*                length -
*                    number of bytes to read
* Return Value : SCIF_SUCCESS -
*                    requested number of byte loaded into p_dst
*                SCIF_ERR_BUSY
*                    cannot accept any more requests (max 2)
 *********************************************************************************************************************/
static scif_err_t scif_async_receive(scif_hdl_t const hdl, uint8_t *p_dst, uint16_t const length)
{
    scif_fifo_ctrl_t *p_ctrl;
    uint16_t          i;
    scif_cb_args_t    args;
    scif_err_t        err=SCIF_SUCCESS;

    /* If two requests already outstanding, return busy */
    p_ctrl = &hdl->queue[hdl->qindex_app_rx];
    if ((0 != p_ctrl->rx_cnt) || (true == hdl->tx_break))
    {
        err = SCIF_ERR_BUSY;
    }
    else /* else process current request */
    {
        if ((0 == hdl->queue[0].rx_cnt) && (0 == hdl->queue[1].rx_cnt) && (SCIF_PRV_RX_FIFO_USED_CNT >= length))
        {

            /* sufficient data already in FIFO */
            /* WAIT_LOOP */
            for (i=0; i < length; i++)
            {
                *p_dst++ = hdl->p_rom->regs->FRDR;
            }

            if ((NULL != hdl->callback) && (FIT_NO_FUNC != hdl->callback))
            {
                args.hdl = hdl;
                args.event = SCIF_EVT_RX_DONE;
                hdl->callback(&args);
            }
        }
        else
        {
            p_ctrl->p_rx_buf = p_dst;
            p_ctrl->rx_cnt = length;              /* length must be set after buf ptr */
            if (1 == hdl->p_rom->regs->FSR.BIT.RDF)
            {
                hdl->p_rom->regs->FSR.BIT.RDF = 0;    /* re-arm threshold (RXIF) interrupt */
            }

            hdl->qindex_app_rx ^= 1;            /* adjust request queue index */
        }
    }

    return err;
}
/**********************************************************************************************************************
 End of function scif_async_receive
 *********************************************************************************************************************/
#endif /* (SCIF_CFG_ASYNC_INCLUDED) */


/**********************************************************************************************************************
* Function Name: scif8_rxif8_isr
* Description  : RXIF interrupt routines for every SCIF channel
* Arguments    : none
* Return Value : none
 *********************************************************************************************************************/
#if SCIF_CFG_CH8_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void scif8_rxif8_isr(void)
{
    rxif_handler(&g_scif_ch8_ctrl);
}
#endif
/**********************************************************************************************************************
 End of function scif8_rxif8_isr
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif9_rxif9_isr
* Description  : RXIF interrupt routines for every SCIF channel
* Arguments    : none
* Return Value : none
 *********************************************************************************************************************/
#if SCIF_CFG_CH9_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void scif9_rxif9_isr(void)
{
    rxif_handler(&g_scif_ch9_ctrl);
}
#endif
/**********************************************************************************************************************
 End of function scif9_rxif9_isr
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif10_rxif10_isr
* Description  : RXIF interrupt routines for every SCIF channel
* Arguments    : none
* Return Value : none
 *********************************************************************************************************************/
#if SCIF_CFG_CH10_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void scif10_rxif10_isr(void)
{
    rxif_handler(&g_scif_ch10_ctrl);
}
#endif
/**********************************************************************************************************************
 End of function scif10_rxif10_isr
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif11_rxif11_isr
* Description  : RXIF interrupt routines for every SCIF channel
* Arguments    : none
* Return Value : none
 *********************************************************************************************************************/
#if SCIF_CFG_CH11_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void scif11_rxif11_isr(void)
{
    rxif_handler(&g_scif_ch11_ctrl);
}
#endif
/**********************************************************************************************************************
 End of function scif11_rxif11_isr
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: rxif_handler
* Description  : RXIF interrupt handler for SCIF
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
* Return Value : none
 *********************************************************************************************************************/
static void rxif_handler(scif_hdl_t const hdl)
{
    volatile scif_fifo_ctrl_t  *p_ctrl;
    volatile uint8_t            byte;   /* volatile to remove "not used" warning */
    scif_cb_args_t              args;
    uint8_t                     tmp_reg_frdr;

#if TESTING_ON
    rxi_cnt++;
    g_rxif_interrupt_count++; /* Increment interrupt count */
#endif

    /* WARNING: When stepping through any code, the FIFO will be reduced by
     * 1 char with each step because the FIFO is read for display in the
     * I/O registers window. Similarly, if a breakpoint is hit, one character
     * is also "lost". Look at the FRDR for the character read out.
     */

    /* If data in fifo and receive request outstanding, read fifo. */
    p_ctrl = &hdl->queue[hdl->qindex_int_rx];
    if (0 != SCIF_PRV_RX_FIFO_USED_CNT)
    {

        /* loop until request completed or no more data in FIFO */
        /* WAIT_LOOP */
        while ((0 != SCIF_PRV_RX_FIFO_USED_CNT) && (0 != p_ctrl->rx_cnt))
        {
            if (NULL == p_ctrl->p_rx_buf)   /* ignore data for Sync Send() */
            {
                byte = hdl->p_rom->regs->FRDR;
            }
            else
            {
                tmp_reg_frdr = hdl->p_rom->regs->FRDR;   /* read byte */
                *p_ctrl->p_rx_buf = tmp_reg_frdr;   /* read byte */
                p_ctrl->p_rx_buf++;
            }
            p_ctrl->rx_cnt--;     /* decrement number of bytes yet to read */

            /* if all of current message read, set index to next message */
            if (0 == p_ctrl->rx_cnt)
            {
                hdl->qindex_int_rx ^= 1;
                p_ctrl = &hdl->queue[hdl->qindex_int_rx];
            }
        }
    }

    if (0 == p_ctrl->rx_cnt)
    {

        /* done with receive requests */

        /* receiver disabled and callback made for sync in TEIF routine */

#if (SCIF_CFG_ASYNC_INCLUDED)
        if (SCIF_MODE_ASYNC == hdl->mode)
        {

            /* call callback function if available */
            if ((NULL != hdl->callback) && (FIT_NO_FUNC != hdl->callback))
            {
                args.hdl = hdl;
                args.event = SCIF_EVT_RX_DONE;
                hdl->callback(&args);
            }

            SCIF_PRV_RX_FIFO_THRESHOLD = 1;
        }
#endif
    }

    /* If remaining data to read is less than threshold, adjust threshold */
    else
    {
        if (p_ctrl->rx_cnt < hdl->p_rom->rx_dflt_thresh)
        {
            SCIF_PRV_RX_FIFO_THRESHOLD = p_ctrl->rx_cnt;
        }
        else
        {
            SCIF_PRV_RX_FIFO_THRESHOLD = hdl->p_rom->rx_dflt_thresh;
        }
    }

    /* If at threshold level, clear bit so can get another RXIF interrupt.
     * Do not re-arm if Receive() request not outstanding (Async)
     */
    if ((1 == hdl->p_rom->regs->FSR.BIT.RDF) && (0 != p_ctrl->rx_cnt))
    {
        hdl->p_rom->regs->FSR.BIT.RDF = 0;
    }

}
/**********************************************************************************************************************
 End of function rxif_handler
 *********************************************************************************************************************/

#if (SCIF_CFG_ASYNC_INCLUDED)
/**********************************************************************************************************************
* Function Name: scif8_erif8_isr
* Description  : ERIF interrupt routines for every SCIF channel.
*                BSP gets main group interrupt, then vectors to/calls these "interrupts"/callbacks.
* Arguments    : none
* Return Value : none
 *********************************************************************************************************************/
#if SCIF_CFG_CH8_INCLUDED
void scif8_erif8_isr(void *cb_args)
{
    erif_handler(&g_scif_ch8_ctrl);
}
#endif
/**********************************************************************************************************************
 End of function scif8_erif8_isr
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif9_erif9_isr
* Description  : ERIF interrupt routines for every SCIF channel.
*                BSP gets main group interrupt, then vectors to/calls these "interrupts"/callbacks.
* Arguments    : none
* Return Value : none
 *********************************************************************************************************************/
#if SCIF_CFG_CH9_INCLUDED
void scif9_erif9_isr(void *cb_args)
{
    erif_handler(&g_scif_ch9_ctrl);
}
#endif
/**********************************************************************************************************************
 End of function scif9_erif9_isr
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif10_erif10_isr
* Description  : ERIF interrupt routines for every SCIF channel.
*                BSP gets main group interrupt, then vectors to/calls these "interrupts"/callbacks.
* Arguments    : none
* Return Value : none
 *********************************************************************************************************************/
#if SCIF_CFG_CH10_INCLUDED
void scif10_erif10_isr(void *cb_args)
{
    erif_handler(&g_scif_ch10_ctrl);
}
#endif
/**********************************************************************************************************************
 End of function scif10_erif10_isr
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif11_erif11_isr
* Description  : ERIF interrupt routines for every SCIF channel.
*                BSP gets main group interrupt, then vectors to/calls these "interrupts"/callbacks.
* Arguments    : none
* Return Value : none
 *********************************************************************************************************************/
#if SCIF_CFG_CH11_INCLUDED
void scif11_erif11_isr(void *cb_args)
{
    erif_handler(&g_scif_ch11_ctrl);
}
#endif
/**********************************************************************************************************************
 End of function scif11_erif11_isr
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: erif_handler
* Description  : ERIF interrupt handler for SCIF UART mode
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
* Return Value : none
 *********************************************************************************************************************/
static void erif_handler(scif_hdl_t const hdl)
{
    scif_cb_args_t  args;
    uint16_t        status;

    /* NOTE: Manual does not mention that This interrupt occurs in addition to
     * the RXIF interrupt. With non-FIFO SCI peripherals, the ERI interrupt
     * occurs instead of an RXI interrupt. This interrupt occurs before the RXIF
     * interrupt even when the threshold is set to 1.
     */

    /* reading status clears FER/PER (not mentioned in manual) */
    status = hdl->p_rom->regs->FSR.WORD;

    if (1 == hdl->p_rom->regs->FSR.BIT.ER)
    {

        /* NOTE: The byte is read in RXIF */

        /* Do callback for error */
        if ((NULL != hdl->callback) && (FIT_NO_FUNC != hdl->callback))
        {
            args.hdl = hdl;
            if (status & SCIF_PRV_SSR_PER_MASK)
            {
                args.event = SCIF_EVT_RX_PARITY_ERR;
            }
            else if (status & SCIF_PRV_SSR_FER_MASK)
            {
                args.event = SCIF_EVT_RX_FRAMING_ERR;
            }
            else
            {

                /* Do nothing */
            }

            /* Casting is valid because it matches the type to the right side or argument. */
            hdl->callback((void *)&args);
        }

        hdl->p_rom->regs->FSR.BIT.ER = 0;     /* clear receive error flag */
    }
}
/**********************************************************************************************************************
 End of function erif_handler
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif8_brif8_isr
* Description  : BRIF interrupt routines for every SCIF channel.
*                BSP gets main group interrupt, then vectors to/calls these "interrupts"/callbacks.
* Arguments    : none
* Return Value : none
 *********************************************************************************************************************/
#if SCIF_CFG_CH8_INCLUDED
void scif8_brif8_isr(void *cb_args)
{
    brif_handler(&g_scif_ch8_ctrl);
}
#endif
/**********************************************************************************************************************
 End of function scif8_brif8_isr
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif9_brif9_isr
* Description  : BRIF interrupt routines for every SCIF channel.
*                BSP gets main group interrupt, then vectors to/calls these "interrupts"/callbacks.
* Arguments    : none
* Return Value : none
 *********************************************************************************************************************/
#if SCIF_CFG_CH9_INCLUDED
void scif9_brif9_isr(void *cb_args)
{
    brif_handler(&g_scif_ch9_ctrl);
}
#endif
/**********************************************************************************************************************
 End of function scif9_brif9_isr
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif10_brif10_isr
* Description  : BRIF interrupt routines for every SCIF channel.
*                BSP gets main group interrupt, then vectors to/calls these "interrupts"/callbacks.
* Arguments    : none
* Return Value : none
 *********************************************************************************************************************/
#if SCIF_CFG_CH10_INCLUDED
void scif10_brif10_isr(void *cb_args)
{
    brif_handler(&g_scif_ch10_ctrl);
}
#endif
/**********************************************************************************************************************
 End of function scif10_brif10_isr
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif11_brif11_isr
* Description  : BRIF interrupt routines for every SCIF channel.
*                BSP gets main group interrupt, then vectors to/calls these "interrupts"/callbacks.
* Arguments    : none
* Return Value : none
 *********************************************************************************************************************/
#if SCIF_CFG_CH11_INCLUDED
void scif11_brif11_isr(void *cb_args)
{
    brif_handler(&g_scif_ch11_ctrl);
}
#endif
/**********************************************************************************************************************
 End of function scif11_brif11_isr
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: brif_handler
* Description  : BRIF interrupt handler for SCIF UART mode
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
* Return Value : none
 *********************************************************************************************************************/
static void brif_handler(scif_hdl_t const hdl)
{
    scif_cb_args_t      args;
    volatile uint8_t    byte;       /* volatile to remove compiler warning */

    args.hdl = hdl;

    if (1 == hdl->p_rom->regs->LSR.BIT.ORER)
    {
        if ((NULL != hdl->callback) && (FIT_NO_FUNC != hdl->callback))
        {
            args.event = SCIF_EVT_RX_OVERFLOW;
            hdl->callback(&args);
        }

        /* clear overrun error status flag; set flag for polling apps */
        hdl->p_rom->regs->LSR.BIT.ORER = 0;
    }

    if (1 == hdl->p_rom->regs->FSR.BIT.BRK)
    {
        byte = hdl->p_rom->regs->FRDR;    /* remove 0 from FIFO */

        /* This interrupt occurs after a '0' is placed in the FIFO */
        if ((NULL != hdl->callback) && (FIT_NO_FUNC != hdl->callback))
        {
            args.event = SCIF_EVT_RX_BREAK;
            hdl->callback(&args);
        }

        /* clear break detect status flag; set flag for polling apps */
        hdl->p_rom->regs->FSR.BIT.BRK = 0;
    }

}
/**********************************************************************************************************************
 End of function brif_handler
 *********************************************************************************************************************/
#endif  /* (SCIF_CFG_ASYNC_INCLUDED) */

/**********************************************************************************************************************
 * Function Name: R_SCIF_Control
 ******************************************************************************************************************//**
 * @brief This function handles special hardware and software operations for the SCIF channel.
 * @param[in] hdl Handle for channel
 * @param[in] cmd Command to run (see enumeration below)
 * @param[in, out] *p_args Pointer to arguments (see below) specific to command, casted to void * \n
 * Most of the commands do not require arguments and take NULL or FIT_NO_PTR for p_args. \n
 * Note that SCIF_CMD_CHANGE_BAUD command may not be used for Asynchronous mode when using an external clock. \n
 * The commands SCIF_CMD_CHECK_TX_DONE, SCIF_CMD_CHECK_RX_DONE, and SCIF_CMD_CHECK_XCV_DONE return SCIF_SUCCESS when 
 * all requests have been transmitted. Otherwise SCIF_ERR_IN_PROGRESS is returned. \n
 * \n
 * Note: For SCIF_CMD_RESET_TX, if a message transmission is in progress it will be abort immediately. It will not 
 * wait until the current byte completes transmission. In this case, it is recommended to wait 1 byte-time before 
 * sending again to allow receiver to process likely framing error from last [partial] byte sent.
 * @retval SCIF_SUCCESS Successful; channel initialized
 * @retval SCIF_ERR_NULL_PTR hdl or p_args pointer is NULL (when required)
 * @retval SCIF_ERR_BAD_MODE Channel mode not currently supported
 * @retval SCIF_ERR_INVALID_ARG The cmd value or an element of p_args contains an invalid value.
 * @details This function is used for configuring "non-standard" hardware features, changing driver configuration, 
 * and obtaining driver status.
 * @note Do not use the value loaded by SCIF_CMD_TX_BYTES_REMAINING to determine if a message is sent. There still  
 * may be bits in the shift register when this commands return a "0". Use SCIF_CMD_TX_DONE for this purpose. \n
 * Wait one bit-time after performing a SCIF_CMD_CHANGE_BAUD for the clock to settle at the new speed. 
 * The bit time should be measured in terms of the slower bit rate. \n
 * Wait two bit-times after performing a SCIF_CMD_GENERATE_BREAK before resuming communications. 
 * Any Send() or Receive() calls made during this will get a SCIF_ERR_BUSY until the break completes. 
 * A break condition lasts 1.5 to 2.0 byte times. \n
 * The driver uses an algorithm for calculating the optimum values for BRR, MDDR, SEMR.ABCS0, SEMR.BGDM, SEMR.BRME, 
 * SEMR.MDDRS and SMR.CKS. This however does not guarantee a low bit error rate for all peripheral clock/baud rate 
 * combinations. \n
 * If the command SCIF_CMD_EN_FLOW_CTRL is to be used, the Pin Function Select and port pins must be configured first.
 */
scif_err_t R_SCIF_Control(scif_hdl_t const hdl, scif_cmd_t const cmd, void *p_args)
{
    scif_err_t  err=SCIF_SUCCESS;
    scif_baud_t *p_baud;
    int32_t     bit_err;

#if (SCIF_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((NULL == hdl) || ((NULL == p_args) && (SCIF_CMD_CHANGE_BAUD == cmd)))
    {
        return SCIF_ERR_NULL_PTR;
    }

    /* cannot set baud rate when using external clock */
    if ((SCIF_CMD_CHANGE_BAUD == cmd) && (0x02 == hdl->p_rom->regs->SCR.BIT.CKE))
    {
        return SCIF_ERR_INVALID_ARG;
    }
#endif
    
    /* COMMANDS COMMON TO ALL MODES */

    if (SCIF_CMD_CHANGE_BAUD == cmd)
    {

        /* disable tx and rx */
        p_baud = (scif_baud_t *)p_args;
        hdl->p_rom->regs->SCR.BIT.RE = 0;
        hdl->p_rom->regs->SCR.BIT.RIE = 0;
        scif_reset_tx(hdl);

        /* change baud rate and reset rx */
        bit_err = scif_init_bit_rate(hdl, p_baud->pclk, p_baud->rate);
        if (SCIF_MODE_ASYNC == hdl->mode)
        {
            scif_reset_rx(hdl, true);       /* leave receiver enabled Async */
        }
        else
        {
            scif_reset_rx(hdl, false);      /* leave receiver disabled Sync */
        }
        if (1000 == bit_err)
        {
            err = SCIF_ERR_INVALID_ARG;     /* impossible baud rate; 100% error */
        }
#if (SCIF_CFG_ASYNC_INCLUDED)
        else /* Async save for break generation */
        {
            s_pclka_speed = p_baud->pclk;
            hdl->baud_rate = p_baud->rate;
        }
#endif
    }

    /* ASYNC-SPECIFIC COMMANDS */
    else if (SCIF_MODE_ASYNC == hdl->mode)
    {
#if (SCIF_CFG_ASYNC_INCLUDED)
        err = scif_async_cmds(hdl, cmd, p_args);
#endif
    }

    /* SYNC-SPECIFIC COMMANDS */
    else
    {
#if (SCIF_CFG_SYNC_INCLUDED)
        err = scif_sync_cmds(hdl, cmd, p_args);
#endif
    }

    return err;
}
/**********************************************************************************************************************
 End of function R_SCIF_Control
 *********************************************************************************************************************/

#if (SCIF_CFG_ASYNC_INCLUDED)
/**********************************************************************************************************************
* Function Name: scif_async_cmds
* Description  : This function configures non-standard UART hardware and
*                performs special software operations.
*
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                cmd -
*                    command to run
*                p_args -
*                    pointer argument(s) specific to command
* Return Value : SCIF_SUCCESS -
*                    Command completed successfully.
*                SCIF_ERR_NULL_PTR -
*                    p_args is NULL when required for cmd
*                SCIF_ERR_INVALID_ARG -
*                    The cmd value or p_args contains an invalid value.
 *********************************************************************************************************************/
static scif_err_t scif_async_cmds(scif_hdl_t const  hdl, scif_cmd_t const cmd, void *p_args)
{
    static uint8_t s_zero=0;
    scif_err_t  err=SCIF_SUCCESS;
    int32_t     bit_err;
    uint16_t    tmp;
    bsp_int_ctrl_t int_ctrl;

#if (SCIF_CFG_PARAM_CHECKING_ENABLE == 1)
    if (((NULL == p_args) || (FIT_NO_PTR == p_args)) 
    && ((SCIF_CMD_TX_BYTES_REMAINING == cmd) || (SCIF_CMD_RX_BYTES_PENDING == cmd)))
    {
        return SCIF_ERR_NULL_PTR;
    }
#endif

    switch(cmd)
    {
        case (SCIF_CMD_EN_NOISE_CANCEL):

            /* NOTE: This temporarily stops and restarts the base clock */
            hdl->p_rom->regs->SEMR.BIT.NFEN = 1;      /* enable noise filter */
            break;

        case (SCIF_CMD_EN_MSB_FIRST):
            if (1 == hdl->p_rom->regs->SMR.BIT.CHR)
            {
                err = SCIF_ERR_INVALID_ARG;         /* cannot have MSB first with 7-bit data */
            }
            else
            {
                hdl->p_rom->regs->SEMR.BIT.DIR = 1;
            }
            break;


        case (SCIF_CMD_EN_FLOW_CTRL):

            /* PFS & port pins must be configured for CTS prior to calling this */
            hdl->p_rom->regs->FCR.BIT.MCE = 1;        /* enable flow control */
            break;

        case (SCIF_CMD_RESET_TX):
            scif_reset_tx(hdl);
            break;

        case (SCIF_CMD_RESET_RX):
            scif_reset_rx(hdl, true);
            break;

        case (SCIF_CMD_TX_BYTES_REMAINING):
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
            SCIF_PRV_DISABLE_TXIF_ICU_INT;
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);

            /* Casting is valid because it matches the type to the right side or argument. */
            *(uint16_t *)p_args = hdl->queue[0].tx_cnt + hdl->queue[1].tx_cnt + SCIF_PRV_TX_FIFO_USED_CNT;
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
            SCIF_PRV_ENABLE_TXIF_ICU_INT;
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
            break;

        case (SCIF_CMD_RX_BYTES_PENDING):
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
            SCIF_PRV_DISABLE_RXIF_ICU_INT;
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
            tmp = hdl->queue[0].rx_cnt + hdl->queue[1].rx_cnt;

            /* there may be more bytes in the FIFO than requested (Async) */
            if (tmp < SCIF_PRV_RX_FIFO_USED_CNT)
            {
                tmp = 0;        /* not waiting to receive any more bytes */
            }
            else
            {
                tmp -= SCIF_PRV_RX_FIFO_USED_CNT;
            }

            /* Casting is valid because it matches the type to the right side or argument. */
            *(uint16_t *)p_args = tmp;
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
            SCIF_PRV_ENABLE_RXIF_ICU_INT;
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
            break;

        case (SCIF_CMD_CHECK_TX_DONE):
            if (false == hdl->tx_idle)
            {
                err = SCIF_ERR_IN_PROGRESS;
            }
            break;

        case (SCIF_CMD_CHECK_RX_DONE):
            if ((0 != hdl->queue[0].rx_cnt) || (0 != hdl->queue[1].rx_cnt))
            {
                err = SCIF_ERR_IN_PROGRESS;
            }
            break;

        case (SCIF_CMD_GENERATE_BREAK):

            /* disable rx and tx */
            hdl->p_rom->regs->SCR.BIT.RE = 0;
            hdl->p_rom->regs->SCR.BIT.RIE = 0;
            scif_reset_tx(hdl);

            /* A break is a "0" level on the mcu side for at least 1.5 byte times.
             * Supposed to wait 1-bit time after changing baud rate for clock to settle.
             * To avoid having a 1 bit-time timer and to reduce the likelihood of an
             * unsupported baud rate (1.5x slower), a baud rate of 2x slower is used to
             * delay 2 byte times instead. */
            bit_err = scif_init_bit_rate(hdl, s_pclka_speed, hdl->baud_rate >> 1);
            if (1000 == bit_err)
            {
                scif_reset_rx(hdl, true);
                err = SCIF_ERR_INVALID_ARG;
            }
            else
            {
                hdl->tx_break = true;       /* don't accept requests while break in progress */
                hdl->tx_idle = false;


                hdl->queue[0].p_tx_buf = &s_zero;       /* load 0 break value */
                hdl->queue[0].tx_cnt = 1;

                R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
                SCIF_PRV_DISABLE_TXIF_ICU_INT;               /* disable interrupt in icu */
                R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
                hdl->p_rom->regs->SCR.BIT.TE = 1;     /* enable transmitter to trigger txif */
                hdl->p_rom->regs->SCR.BIT.TIE = 1;    /*  and send value */
                R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
                SCIF_PRV_ENABLE_TXIF_ICU_INT;
                R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
                hdl->qindex_app_tx ^= 1;            /* adjust request queue index */

                /* original baud rate will be restored and rx reset in TEIF interrupt */
            }
            break;

        default:
            err = SCIF_ERR_INVALID_ARG;
            break;
    }

    return err;
}
/**********************************************************************************************************************
 End of function scif_async_cmds
 *********************************************************************************************************************/
#endif  /* (SCIF_CFG_ASYNC_INCLUDED) */

/**********************************************************************************************************************
* Function Name: scif_reset_tx
* Description  : Resets the tx fifo and interrupt flags. Leaves transmitter
*                disabled
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
* Return Value : none
 *********************************************************************************************************************/
static void scif_reset_tx(scif_hdl_t const hdl)
{
    bsp_int_ctrl_t int_ctrl;

    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
    SCIF_PRV_DISABLE_TXIF_ICU_INT;
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
    *hdl->p_rom->p_ir_txif = 0;             /* clear ICU flag */
    hdl->p_rom->regs->SCR.WORD &= (~(SCIF_PRV_SCR_TE_MASK | SCIF_PRV_SCR_TIE_MASK | SCIF_PRV_SCR_TEIE_MASK));
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
    SCIF_PRV_ENABLE_TXIF_ICU_INT;
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);

    /* reset FIFO */
    hdl->p_rom->regs->FCR.BIT.TFRST = 1;
    hdl->p_rom->regs->FCR.BIT.TFRST = 0;

    hdl->tx_idle = true;
    hdl->queue[0].tx_cnt = 0;
    hdl->queue[1].tx_cnt = 0;
    hdl->qindex_app_tx = 0;
    hdl->qindex_int_tx = 0;
    SCIF_PRV_TX_FIFO_THRESHOLD = hdl->p_rom->tx_dflt_thresh;

    return;
}
/**********************************************************************************************************************
 End of function scif_reset_tx
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: scif_reset_rx
* Description  : Resets the rx fifo and interrupt flags. Leaves receiver
*                enabled based upon enable_rx value.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                enable_rx -
*                    enables rx after resetting when TRUE
* Return Value : none
 *********************************************************************************************************************/
static void scif_reset_rx(scif_hdl_t const hdl, bool enable_rx)
{
    volatile uint16_t   word;       /* made volatile to eliminate unused warning */
    bsp_int_ctrl_t int_ctrl;

    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
    SCIF_PRV_DISABLE_RXIF_ICU_INT;
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
    *hdl->p_rom->p_ir_rxif = 0;             /* clear ICU flag */
    hdl->p_rom->regs->SCR.WORD &= (~(SCIF_PRV_SCR_RE_MASK | SCIF_PRV_SCR_RIE_MASK));
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
    SCIF_PRV_ENABLE_RXIF_ICU_INT;
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);

    /* reset FIFO */
    hdl->p_rom->regs->FCR.BIT.RFRST = 1;
    hdl->p_rom->regs->FCR.BIT.RFRST = 0;

    /* reset status */
    word = hdl->p_rom->regs->LSR.WORD;
    hdl->p_rom->regs->LSR.WORD = 0;
    word = hdl->p_rom->regs->FSR.WORD;
    hdl->p_rom->regs->FSR.WORD &= 0x0060;     /* reset all except 2 transmit bits */

    /* reset queues */
    hdl->queue[0].rx_cnt = 0;
    hdl->queue[1].rx_cnt = 0;
    hdl->qindex_app_rx = 0;
    hdl->qindex_int_rx = 0;
    SCIF_PRV_RX_FIFO_THRESHOLD = 1;

    if (true == enable_rx)
    {
        hdl->p_rom->regs->SCR.BIT.RE = 1;     /* do not combine into word OR */
        hdl->p_rom->regs->SCR.BIT.RIE = 1;
    }

    return;
}
/**********************************************************************************************************************
 End of function scif_reset_rx
 *********************************************************************************************************************/

#if (SCIF_CFG_SYNC_INCLUDED)
/**********************************************************************************************************************
* Function Name: scif_sync_cmds
* Description  : This function performs special software operations specific
*                to the SSPI and SYNC protocols.
*
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                cmd -
*                    command to run
*                p_args -
*                    pointer argument(s) specific to command
* Return Value : SCIF_SUCCESS -
*                    Command completed successfully.
*                SCIF_ERR_NULL_PTR -
*                    p_args is NULL when required for cmd
*                SCIF_ERR_INVALID_ARG -
*                    The cmd value or p_args contains an invalid value.
*                    May be due to mode channel is operating in.
 *********************************************************************************************************************/
static scif_err_t scif_sync_cmds(scif_hdl_t const hdl, scif_cmd_t const cmd, void *p_args)
{
scif_cb_args_t   args;
scif_err_t       err=SCIF_SUCCESS;


    switch (cmd)
    {
    case (SCIF_CMD_CHECK_XCV_DONE):
        if (hdl->tx_idle == false)
        {
            err = SCIF_ERR_IN_PROGRESS;
        }
    break;


    case (SCIF_CMD_RESET_XCV):
        scif_reset_tx(hdl);
        scif_reset_rx(hdl, false);

        /* Do callback if available */
        if ((hdl->callback != NULL) && (hdl->callback != FIT_NO_FUNC))
        {
            args.hdl = hdl;
            args.event = SCIF_EVT_XCV_ABORTED;
            hdl->callback((void *)&args);
        }
    break;


    default:
        err = SCIF_ERR_INVALID_ARG;
    break;
    }

    return err;
}
/**********************************************************************************************************************
 End of function scif_sync_cmds
 *********************************************************************************************************************/
#endif /* (SCIF_CFG_SYNC_INCLUDED) */

/**********************************************************************************************************************
 * Function Name: R_SCIF_Close
 ******************************************************************************************************************//**
 * @brief This function removes power to the SCIF channel and disables the associated interrupts.
 * @param[in] hdl Handle for channel
 * @retval SCIF_SUCCESS Successful; channel closed
 * @retval SCIF_ERR_NULL_PTR hdl is NULL
 * @details Disables the SCIF channel designated by the handle. Does not free any resources but saves power and 
 * allows the corresponding channel to be re-opened later, potentially with a different configuration.
 * @note This function will abort any transmission or reception that may be in progress.
 */
scif_err_t R_SCIF_Close(scif_hdl_t const hdl)
{
    bsp_int_ctrl_t int_ctrl;

#if (SCIF_CFG_PARAM_CHECKING_ENABLE == 1)
    if (NULL == hdl)
    {
        return SCIF_ERR_NULL_PTR;
    }
#endif

    /* disable ICU interrupts */
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
    SCIF_PRV_DISABLE_RXIF_ICU_INT;
    SCIF_PRV_DISABLE_TXIF_ICU_INT;

    /* do not disable GROUPAL0 interrupts; may be active for another peripheral */
    SCIF_PRV_DISABLE_TEIF_ICU_INT;
#if SCIF_CFG_ASYNC_INCLUDED
    SCIF_PRV_DISABLE_ERIF_ICU_INT;
    SCIF_PRV_DISABLE_BRIF_ICU_INT;
#endif
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);

    /* disable peripheral interrupts and xcvr (TE and RE) */
    hdl->p_rom->regs->SCR.WORD = 0;
        

    /* mark the channel as not in use and power down */
    hdl->mode = SCIF_MODE_OFF;
    power_off(hdl);

    return SCIF_SUCCESS;
}
/**********************************************************************************************************************
 End of function R_SCIF_Close
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCIF_GetVersion
 ******************************************************************************************************************//**
 * @brief This function returns the driver version number at runtime.
 * @retval Version number.
 * @details Returns the version of this module. The version number is encoded such that the top two bytes are the 
 * major version number and the bottom two bytes are the minor version number.
 */
uint32_t  R_SCIF_GetVersion(void)
{
    uint32_t const version = (SCIF_VERSION_MAJOR << 16) | SCIF_VERSION_MINOR;

    return version;
}
/**********************************************************************************************************************
 End of function R_SCIF_GetVersion
 *********************************************************************************************************************/
