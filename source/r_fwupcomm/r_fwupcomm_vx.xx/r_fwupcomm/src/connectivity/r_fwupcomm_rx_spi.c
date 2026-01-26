/*
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
 * File Name    : r_fwupcomm_rx_spi.c
 * Description  : Functions for the RX MCU to communicate using RSPI/SCI SPI.
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           20.10.2025 1.00    Initial Release
 *           11.11.2025 1.10    Added SPI communication support.
 ***********************************************************************************************************************/

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include "r_fwupcomm_private.h"

#if ((FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_SCI_SPI)||(FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI))

#define FWUPCOMM_MY_DEVICE_ADDRESS                  (1U)
#define FWUPCOMM_NOT_MY_DEVICE_ADDRESS              (0U)

const r_fwupcomm_ch_api_t r_fwupcomm_ch_api =
{
    .open     = r_fwupcomm_rx_spi_open,
    .close    = r_fwupcomm_rx_spi_close,
    .send     = r_fwupcomm_rx_spi_send,
    .recv     = r_fwupcomm_rx_spi_recv,
    .rx_reset = r_fwupcomm_rx_spi_rx_reset
};


static uint8_t        * sp_send_packet_buffer;  /* Pointer of send buffer */
static uint8_t        * sp_recv_packet_buffer;  /* Pointer of recv buffer */

static bool             s_fwupcomm_spi_ch_open_flag  = false;

volatile static bool    s_fwupcomm_spi_tx_end_flag   = true;
volatile static uint8_t s_fwupcomm_spi_tx_state      = FWUPCOMM_SPI_STATE_RESP_END;
volatile static uint8_t s_fwupcomm_spi_rx_state      = FWUPCOMM_SPI_STATE_RESP_END;

#if (FWUPCOMM_CFG_DEVICE_PRIMARY == 0)
static uint8_t          s_my_device_address_flag     = FWUPCOMM_NOT_MY_DEVICE_ADDRESS;
#endif
static uint8_t          s_err_type                   = 0x00;

#if (FWUPCOMM_CFG_DEVICE_PRIMARY == 1)  /* Primary */
static uint8_t          s_fwupcomm_spi_tx_state_bup  = FWUPCOMM_SPI_STATE_RESP_END;
static uint16_t         s_send_packet_size           = 0x00;  /* transmission packet size */
static uint16_t         s_resp_data_length_info      = 0x00;  /* Number (bytes) of received data length information on Response Packet */
                                                              /* Use for Response dummy clock transmission of Data */
#else  /* Secondary */
static uint8_t          s_fwupcomm_spi_rx_state_bup  = FWUPCOMM_SPI_STATE_RESP_END;
static uint16_t         s_recv_packet_size           = 0x00;  /* Received packet size */
static uint16_t         s_req_data_length_info       = 0x00;  /* Number (bytes) of received data length information for Request packet */
                                                              /* Use for Request packet reception */
                                                              /* Divide by 4 when using RSPI FIT (32-bit data frame) */
#endif

#if (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI)  /* RSPI FIT */
static rspi_command_word_t  s_fwupcomm_rspi_command;
static rspi_handle_t        s_fwupcomm_rspi_handle;
static rspi_chnl_settings_t s_fwupcomm_rspi_setting;
static rspi_evt_t           s_event;  /* Storage for callback result data */
#else  /* SCI FIT */
static sci_cfg_t            s_fwupcomm_sci_sspi_config;
static sci_hdl_t            s_fwupcomm_sci_sspi_handle;
static sci_cb_args_t      * sp_cb_args;
static sci_cb_evt_t         s_event;  /* Storage for callback result data */
#endif

static void                 r_fwupcomm_rx_spi_send_recv_end (uint8_t mode);
static void                 r_fwupcomm_rx_spi_cb (void * p_data);


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_rx_spi_open
 * Description  : Opens a communications channel.
 * Return Value : FWUPCOMM_SUCCESS            : The channel was successfully initialized.
 *              : FWUPCOMM_ERR_CH_ALREADY_OPEN: The communications channel has already been opened.
 *              : FWUPCOMM_ERR_NOT_OPEN       : Initializing the communications channel failed.
 *********************************************************************************************************************/
fwupcomm_err_t r_fwupcomm_rx_spi_open(r_fwupcomm_hdl_t * hdl)
{
    if (true == s_fwupcomm_spi_ch_open_flag)
    {
        return FWUPCOMM_ERR_CH_ALREADY_OPEN;
    }

    /* Store the pointers of send buffer and recv buffer */
    sp_send_packet_buffer   = hdl->send_buf.data;
    sp_recv_packet_buffer   = hdl->recv_buf.data;

    /* Set MISO PDR to Output */
    FWUPCOMM_SPI_MISO_PDR           =  FWUPCOMM_SPI_GPIO_PDR_OUT;  /* Output setting */
    if (FWUPCOMM_SPI_GPIO_PDR_OUT   == FWUPCOMM_SPI_MISO_PDR)
    {
        R_BSP_NOP();
    }

#if (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI)  /* RSPI FIT (Primary/Secondary Common) */
    rspi_err_t      spi_err;

    /* RSPI command structure */
    s_fwupcomm_rspi_command.cpha            = RSPI_SPCMD_CPHA_SAMPLE_EVEN;  /* SPI Mode 3 */
    s_fwupcomm_rspi_command.cpol            = RSPI_SPCMD_CPOL_IDLE_HI;      /* SPI Mode 3 */
    s_fwupcomm_rspi_command.br_div          = RSPI_SPCMD_BR_DIV_1;          /* No divided setting */
    s_fwupcomm_rspi_command.ssl_assert      = RSPI_SPCMD_ASSERT_SSL0;       /* unused */
    s_fwupcomm_rspi_command.ssl_negate      = RSPI_SPCMD_SSL_KEEP;          /* unused */
    s_fwupcomm_rspi_command.bit_length      = RSPI_SPCMD_BIT_LENGTH_32;     /* 32-bit */
    s_fwupcomm_rspi_command.bit_order       = RSPI_SPCMD_ORDER_LSB_FIRST;   /* LSB First */
    s_fwupcomm_rspi_command.next_delay      = RSPI_SPCMD_NEXT_DLY_SSLND;    /* default */
    s_fwupcomm_rspi_command.ssl_neg_delay   = RSPI_SPCMD_SSL_NEG_DLY_SSLND; /* default */
    s_fwupcomm_rspi_command.clock_delay     = RSPI_SPCMD_CLK_DLY_SPCKD;     /* default */
    s_fwupcomm_rspi_command.dummy           = RSPI_SPCMD_DUMMY;             /* unused (for Reception only mode) */

    /* RSPI settings structure */
    s_fwupcomm_rspi_setting.gpio_ssl            = RSPI_IF_MODE_3WIRE;       /* 3-wire interface mode */
    s_fwupcomm_rspi_setting.tran_mode           = RSPI_TRANS_MODE_SW;       /* CPU transfer */

 #if (FWUPCOMM_CFG_DEVICE_PRIMARY == 1)
    s_fwupcomm_rspi_setting.bps_target          = FWUPCOMM_CFG_SPI_BITRATE;
    s_fwupcomm_rspi_setting.master_slave_mode   = RSPI_MS_MODE_MASTER;      /* Master mode */
 #else  /* Secondary */
    s_fwupcomm_rspi_setting.bps_target          = FWUPCOMM_CFG_SPI_BITRATE; /* Secondary, but need to set */
    s_fwupcomm_rspi_setting.master_slave_mode   = RSPI_MS_MODE_SLAVE;       /* Slave mode */
 #endif

    /* Open SPI driver (Primary/Secondary Common) */
    spi_err = R_RSPI_Open(FWUPCOMM_CFG_SPI_CHANNEL,
                          &s_fwupcomm_rspi_setting,
                          s_fwupcomm_rspi_command,
                          &r_fwupcomm_rx_spi_cb,
                          &s_fwupcomm_rspi_handle);
    if (RSPI_SUCCESS != spi_err)  /* bps_target = 0 bps -> return RSPI_ERR_ARG_RANGE */
    {  /* No other errors occurred */
        return FWUPCOMM_ERR_INVALID_ARG;
    }

    /* Pin Setting */
    /* Set RSIP pins to RSPI function.
     * APN R01AN1827: R_RSPI_Open() -> R_RSPI_PinSet_RSPIn() */
 #if (FWUPCOMM_CFG_DEVICE_PRIMARY == 1)  /* Primary */
    /* Set MOSI/MISO/CLK pins to RSPI function. */
    FWUPCOMM_RSPI_PINSET();
 #else  /* Secondary */
    /* For GPIO setting, already set in main().
     * R_RSPI_PinSet_RSPIn() sets MOSI/MISO/CLK pins to RSPI function by setting MPC.xxxPFS and PORTx.PMR.
     * Therefore, set only the PMR of MISO to GPIO again.
     * MISO = H (Open-Drain) may occur in the short duration of time (2-3 us).  */
    FWUPCOMM_RSPI_PINSET();
    FWUPCOMM_SPI_MISO_PMR    = FWUPCOMM_SPI_GPIO_PMR_GPIO;   /* Set MISO to GPIO mode again */

 #endif  /* Primary / Secondary */

#else  /* SCI FIT (Primary Only) */
    sci_err_t           spi_err;

    s_fwupcomm_sci_sspi_config.sspi.spi_mode        = SCI_SPI_MODE_3;           /* SPI Mode 3 */
    s_fwupcomm_sci_sspi_config.sspi.bit_rate        = FWUPCOMM_CFG_SPI_BITRATE;
    s_fwupcomm_sci_sspi_config.sspi.msb_first       = false;                    /* LSB First */
    s_fwupcomm_sci_sspi_config.sspi.invert_data     = false;
    s_fwupcomm_sci_sspi_config.sspi.int_priority    = FWUPCOMM_CFG_SPI_INT_PRIORITY;

    /* Open SPI driver */
 #if (FWUPCOMM_CFG_DEVICE_PRIMARY == 1)
    spi_err = R_SCI_Open(FWUPCOMM_CFG_SPI_CHANNEL,
                         SCI_MODE_SSPI,
                         &s_fwupcomm_sci_sspi_config,
                         r_fwupcomm_rx_spi_cb,
                         &s_fwupcomm_sci_sspi_handle);
 #else  /* Secondary: SCI FIT; Not supported because the slave mode is not supported. */
 #endif
    if (SCI_SUCCESS != spi_err)
    {
        return FWUPCOMM_ERR_INVALID_ARG;
    }

    /* Pin Setting */
    /* Set SCI pins to SCI function
     * APN R01AN1815: R_SCI_Open() -> R_SCI_PinSet_SCIn() */
    FWUPCOMM_SCI_PINSET();
#endif  /* RSPI FIT (Primary/Secondary Common) / SCI FIT Primary  Only) */

    /* Initialize flags and states for transmission/Reception */
    s_fwupcomm_spi_tx_end_flag  = true;

    /* Set an open flag */
    s_fwupcomm_spi_ch_open_flag = true;

    s_fwupcomm_spi_tx_state     = FWUPCOMM_SPI_STATE_RESP_END;
    s_fwupcomm_spi_rx_state     = FWUPCOMM_SPI_STATE_RESP_END;
    s_err_type                  = 0x00;

#if (FWUPCOMM_CFG_DEVICE_PRIMARY == 0)  /* Secondary */
 #if (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI)  /* RSPI FIT */
    /* Setup the request packet reception */
    r_fwupcomm_rx_spi_rx_reset(hdl);
 #else  /* SCI FIT (Primary Only) */
 #endif  /* RSPI FIT / SCI FIT */
#endif  /* Secondary */
    return FWUPCOMM_SUCCESS;
}
/**********************************************************************************************************************
 End of function r_fwupcomm_rx_spi_open
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwupcomm_rx_spi_close
 * Description  : Closes a communications channel.
 * Return Value : None
 *********************************************************************************************************************/
void r_fwupcomm_rx_spi_close(r_fwupcomm_hdl_t * hdl)
{
#if (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI)  /* RSPI FIT (Primary/Secondary Common) */
    R_RSPI_Close(s_fwupcomm_rspi_handle);
#else  /* SCI FIT (Primary Only) */
    R_SCI_Close(s_fwupcomm_sci_sspi_handle);
#endif  /* RSPI FIT / SCI FIT */

    s_fwupcomm_spi_tx_end_flag  = true;
    s_fwupcomm_spi_ch_open_flag = false;

    s_fwupcomm_spi_tx_state     = FWUPCOMM_SPI_STATE_RESP_END;
    s_fwupcomm_spi_rx_state     = FWUPCOMM_SPI_STATE_RESP_END;
}
/**********************************************************************************************************************
 End of function r_fwupcomm_rx_spi_close
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_rx_spi_send
 * Description  : Setups transmission and waits for clock.
 * Arguments    : src : Pointer to the destination for storing data to be sent
 *              : size: Size of data to be sent
 * Return Value : FWUPCOMM_SUCCESS:          The channel was successfully initialized.
 *              : FWUPCOMM_ERR_INVALID_PTR:  The src pointer is NULL.
 *              : FWUPCOMM_ERR_INVALID_ARG:  size is 0.
 *              : FWUPCOMM_ERR_NOT_OPEN:     The communications channel has not been opened.
 *              : FWUPCOMM_ERR_CH_SEND_BUSY: The communications channel was busy so sending of data failed.
 *              : FWUPCOMM_ERR_CH_SEND:      Sending of data in the communications channel failed.
 *********************************************************************************************************************/
fwupcomm_err_t r_fwupcomm_rx_spi_send(r_fwupcomm_hdl_t * hdl, uint8_t * const src, uint16_t size)
{
#if (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI)  /* RSPI FIT (Primary/Secondary Common) */
    rspi_err_t      spi_err;
#else  /* SCI FIT (Primary Only) */
    sci_err_t           spi_err;
#endif  /* RSPI FIT / SCI FIT */
    
#if FWUPCOMM_CFG_PARAM_CHECKING_ENABLE
    if (NULL == src)
    {
        return FWUPCOMM_ERR_INVALID_PTR;
    }
    if (FWUPCOMM_CMD_HEADER_SIZE > size)
    {
        return FWUPCOMM_ERR_INVALID_ARG;
    }
#endif

    if (false == s_fwupcomm_spi_ch_open_flag)
    {
        return FWUPCOMM_ERR_NOT_OPEN;
    }

    if (true != s_fwupcomm_spi_tx_end_flag)  /* Clear to false at Send() calling */
    {
        return FWUPCOMM_ERR_CH_SEND_BUSY;
    }

#if (FWUPCOMM_CFG_DEVICE_PRIMARY == 1)  /* Primary */
    /* Setup to send Request Header */
    s_fwupcomm_spi_tx_end_flag  = false;
    s_fwupcomm_spi_tx_state_bup = s_fwupcomm_spi_tx_state;
    s_fwupcomm_spi_tx_state     = FWUPCOMM_SPI_STATE_REQ_START;  /* Request */
    s_err_type                  = 0x00;

    /* Store the size information */
    s_send_packet_size = size;

    /* Setup to send packet */
    /*  1st: Header transmission
     *  2nd: If Data is available, delay
     *  3rd: Data transmission            */

    /* Send Header */
 #if (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI)  /* RSPI FIT, 32-bit Data Access */
    spi_err = R_RSPI_WriteRead(s_fwupcomm_rspi_handle,
                               s_fwupcomm_rspi_command,
                               (void *)src,
                               (void *)sp_recv_packet_buffer,
                               (uint16_t)(FWUPCOMM_CMD_HEADER_SIZE / 4)); /* Number of Data frames */
    if (RSPI_SUCCESS != spi_err)
    {   /* Restore flag and state */
        s_fwupcomm_spi_tx_end_flag  = true;
        s_fwupcomm_spi_tx_state     = s_fwupcomm_spi_tx_state_bup;
        s_send_packet_size          = 0x00U;

        return FWUPCOMM_ERR_CH_SEND;
    }
 #else  /* SCI FIT, 8-bit Data Access */
    spi_err = R_SCI_SendReceive(s_fwupcomm_sci_sspi_handle,
                                src,
                                sp_recv_packet_buffer,
                                FWUPCOMM_CMD_HEADER_SIZE);
    if (SCI_SUCCESS != spi_err)  /* SCI_ERR_XCVR_BUSY */
    {   /* Restore flag and state */
        s_fwupcomm_spi_tx_end_flag  = true;
        s_fwupcomm_spi_tx_state     = s_fwupcomm_spi_tx_state_bup;
        s_send_packet_size          = 0x00U;

        return FWUPCOMM_ERR_CH_SEND_BUSY;
    }
 #endif  /* RSPI FIT / SCI FIT */

    r_fwupcomm_rx_spi_send_recv_end(FWUPCOMM_SPI_TRANS_END_REQ_HEADER);  /* Wait for transmission/reception end */

    /* If Request packet has Data, send Data */
    if (FWUPCOMM_CMD_HEADER_SIZE < size)  /* With Data */
    {
        /* Interval (Wait for Secondary Reception Setup */
        R_BSP_SoftwareDelay(FWUPCOMM_SPI_PRIMARY_SEND_INTERVAL_US, BSP_DELAY_MICROSECS);

        /* Setup to send Request Data */
        /* Set the flag and the state */
        s_fwupcomm_spi_tx_end_flag  = false;
        s_fwupcomm_spi_tx_state_bup = s_fwupcomm_spi_tx_state;
        s_err_type                  = 0x00;

        /* Send Data */
 #if (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI)  /* RSPI FIT, 32-bit Data Access */
        spi_err = R_RSPI_WriteRead(s_fwupcomm_rspi_handle,
                                   s_fwupcomm_rspi_command,
                                   (void *)(src + FWUPCOMM_CMD_HEADER_SIZE),
                                   (void *)(sp_recv_packet_buffer + FWUPCOMM_CMD_HEADER_SIZE),
                                   (uint16_t)((size / 4) - 2)); /* Number of Data frames, 2: Header frames */
        if (RSPI_SUCCESS != spi_err)
        {   /* Restore flag and state */
            s_fwupcomm_spi_tx_end_flag  = true;
            s_fwupcomm_spi_tx_state     = s_fwupcomm_spi_tx_state_bup;
            s_send_packet_size          = 0x00U;

            return FWUPCOMM_ERR_CH_SEND;
        }
 #else  /* SCI, 8-bit Data Access */
        spi_err = R_SCI_SendReceive(s_fwupcomm_sci_sspi_handle,
                                    src + FWUPCOMM_CMD_HEADER_SIZE,
                                    sp_recv_packet_buffer + FWUPCOMM_CMD_HEADER_SIZE,
                                    size - FWUPCOMM_CMD_HEADER_SIZE);  /* Data size */
        if (SCI_SUCCESS != spi_err)  /* SCI_ERR_XCVR_BUSY */
        {   /* Restore flag and state */
            s_fwupcomm_spi_tx_end_flag  = true;
            s_fwupcomm_spi_tx_state     = s_fwupcomm_spi_tx_state_bup;
            s_send_packet_size          = 0x00U;

            return FWUPCOMM_ERR_CH_SEND_BUSY;
        }
 #endif  /* RSPI FIT / SCI FIT */

        r_fwupcomm_rx_spi_send_recv_end(FWUPCOMM_SPI_TRANS_END_REQ_DATA);

    }  /* (FWUPCOMM_CMD_HEADER_SIZE < size)  With Data */

    /* Wait for MISO = L surely before MISO = H detection start */
    R_BSP_SoftwareDelay(FWUPCOMM_SPI_PRIMARY_SEND_MISO_H_DETECT_START_DELAY_US, BSP_DELAY_MICROSECS);

    /* Wait for all Secondary Ready */
    while (0U == FWUPCOMM_SPI_MISO_PIDR)
    {
        if (0U == FWUPCOMM_SPI_MISO_PIDR)
        {
            R_BSP_NOP();
        }
    }

    /* Delay (MISO = H Hold Time) */
    R_BSP_SoftwareDelay(FWUPCOMM_SPI_PRIMARY_SEND_MISO_H_DETECT_END_DELAY_US, BSP_DELAY_MICROSECS);

#else  /* Secondary */
    /* Setup to send Response */
    /* Set the flag and the state */
    s_fwupcomm_spi_tx_end_flag  = false;
    s_fwupcomm_spi_rx_state_bup = s_fwupcomm_spi_rx_state;
    s_fwupcomm_spi_rx_state     = FWUPCOMM_SPI_STATE_RESP_START;  /* Send: RESP */
    s_err_type                  = 0x00;

    /* Setup to send packet (Header and Data) for Response */
    /*  1st: Header transmission
     *  2nd: Interval Delay (Waiting for clock)
     *       Primary gets the data length information and restart transmission for Data
     *  3rd: Data transmission            */

    /* Setup to send packet and wait for clock */
 #if (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI)  /* RSPI FIT, 32-bit Data Access */
    spi_err = R_RSPI_WriteRead(s_fwupcomm_rspi_handle,
                               s_fwupcomm_rspi_command,
                               (void *)src,
                               (void *)sp_recv_packet_buffer,
                               (uint16_t)(size / 4)); /* Number of Data frames */
    if (RSPI_SUCCESS != spi_err)
    {
        if (RSPI_ERR_INVALID_ARG == spi_err)  /* Hardware error */
        {   /* Restore flag and state */
            s_fwupcomm_spi_tx_end_flag  = true;
            s_fwupcomm_spi_rx_state     = s_fwupcomm_spi_rx_state_bup;

            return FWUPCOMM_ERR_INVALID_ARG;
        }
        else
        {   /* Restore flag and state */
            s_fwupcomm_spi_tx_end_flag  = true;
            s_fwupcomm_spi_rx_state     = s_fwupcomm_spi_rx_state_bup;

            return FWUPCOMM_ERR_CH_SEND_BUSY;
        }
    }
 #else  /* SCI FIT; Not supported because the slave mode is not supported. */
 #endif  /* RSPI FIT / SCI FIT */

    /* Send Header and Data (Secondary: fixed the response packet size) */
    /* Check My Device address */
    if (FWUPCOMM_MY_DEVICE_ADDRESS  == s_my_device_address_flag)  /* My Device Address */
    {
        /* Set MISO to H (Open-Drain) because the response packet is generated already */
        FWUPCOMM_SPI_MISO_PODR          =  FWUPCOMM_SPI_GPIO_PODR_H;  /* H (Open-Drain) setting */
        if (FWUPCOMM_SPI_GPIO_PODR_H    == FWUPCOMM_SPI_MISO_PODR)
        {
            R_BSP_NOP();
        }

        /* Wait for a bus H charge time (Time Constant) of RC circuit */
        R_BSP_SoftwareDelay(FWUPCOMM_SPI_BUS_H_CHARGE_WAIT_US, BSP_DELAY_MICROSECS);

        /* Wait for MISO bus = H (All slaves is in ready state) */
        /* When MISO bus = H,
         * Set MISO to H (CMOS)
         * Set MISO to RSPI function */
        while (0U == FWUPCOMM_SPI_MISO_PIDR)
        {
            R_BSP_NOP();
        }
        /* Delay (MISO = H Hold Time) to ensure Primary H detection */
        if (1U == FWUPCOMM_SPI_MISO_PIDR)
        {
            R_BSP_SoftwareDelay(FWUPCOMM_SPI_MISO_HOLD_US, BSP_DELAY_MICROSECS);
        }

        /* If Device Address = FWUPCOMM_CFG_DEVICE_ADDRESS and command with response request,
         *  Set MISO to Peripheral mode */
        if ((FWUPCOMM_CFG_DEVICE_ADDRESS == (*sp_recv_packet_buffer))
        &&  (0x00 == (*(sp_recv_packet_buffer + FWUPCOMM_CMD_HEADER_CMD_OPTION) & 0x80)))
        {
            /* Change Pin to CMOS Output and SPI mode */
            /* MISO may become L output */
            FWUPCOMM_SPI_MISO_ODR   = FWUPCOMM_SPI_GPIO_ODR_CMOS;  /* Set MISO to CMOS output */
            FWUPCOMM_SPI_MISO_PMR   = FWUPCOMM_SPI_GPIO_PMR_PERI;  /* Set MISO to Peripheral mode */
            if (FWUPCOMM_SPI_GPIO_PMR_PERI  == FWUPCOMM_SPI_MISO_PMR)
            {
                R_BSP_NOP();
            }
        }
    }  /* (FWUPCOMM_MY_DEVICE_ADDRESS == s_my_device_address_flag) */

    r_fwupcomm_rx_spi_send_recv_end(FWUPCOMM_SPI_TRANS_END_RESP_DATA);  /* Wait for transmission/reception end */

#endif  /* Primary / Secondary */

    return FWUPCOMM_SUCCESS;
}
/**********************************************************************************************************************
 End of function r_fwupcomm_rx_spi_send
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_rx_spi_recv
 * Description  : Checks the received data size.
 * Arguments    : dest: When receiving Header, it is a top of Receive Buffer.
 *              :       When receiving Data, it is a top of Receive Buffer of Data.
 *              : size: When receiving Header, it is FWUPCOMM_CMD_HEADER_SIZE.
 *              :       When receiving Data, it is Data size.
 * Return Value : FWUPCOMM_SUCCESS:             The channel was successfully initialized.
 *              : FWUPCOMM_ERR_INVALID_PTR:     The dest pointer is NULL.
 *              : FWUPCOMM_ERR_INVALID_ARG:     size is 0.
 *              : FWUPCOMM_ERR_NOT_OPEN:        The communications channel has not been opened.
 *              : FWUPCOMM_ERR_CH_RECV_NO_DATA: The communications channel does not have enough received data.
 *********************************************************************************************************************/
fwupcomm_err_t r_fwupcomm_rx_spi_recv(r_fwupcomm_hdl_t * hdl, uint8_t * const dest, uint16_t size)
{
#if (FWUPCOMM_CFG_DEVICE_PRIMARY == 1)  /* Primary */
 #if (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI)  /* RSPI FIT (Primary/Secondary Common) */
    rspi_err_t      spi_err;
 #else  /* SCI FIT (Primary Only) */
    sci_err_t       spi_err;
 #endif  /* RSPI FIT / SCI FIT */
#endif  /* Primary */

#if FWUPCOMM_CFG_PARAM_CHECKING_ENABLE
    if (NULL == dest)
    {
        return FWUPCOMM_ERR_INVALID_PTR;
    }
    if (0 == size)
    {
        return FWUPCOMM_ERR_INVALID_ARG;
    }
#endif

    if (false == s_fwupcomm_spi_ch_open_flag)
    {
        return FWUPCOMM_ERR_NOT_OPEN;
    }

#if (FWUPCOMM_CFG_DEVICE_PRIMARY == 1)  /* Primary */
    /* Clear both send buffer and recv buffer */
    memset((void *)hdl->send_buf.data, 0x00, FWUPCOMM_CFG_SEND_PACKET_BUFFER_SIZE);
    memset((void *)hdl->recv_buf.data, 0x00, FWUPCOMM_CFG_RECV_PACKET_BUFFER_SIZE);

    if (0U == s_resp_data_length_info)
    {
        /* Set the flag and state */
        s_fwupcomm_spi_tx_end_flag  = false;
        s_fwupcomm_spi_tx_state_bup = s_fwupcomm_spi_tx_state;
        s_fwupcomm_spi_tx_state     = FWUPCOMM_SPI_STATE_RESP_START;  /* Response */
        s_err_type                  = 0x00;

        /* Send dummy header and Get Data size */
        /* Send dummy Header */
 #if (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI)  /* RSPI FIT, 32-bit Data Access */
        spi_err = R_RSPI_WriteRead(s_fwupcomm_rspi_handle,
                                   s_fwupcomm_rspi_command,
                                   (void *)sp_send_packet_buffer,
                                   (void *)dest,
                                   (uint16_t)FWUPCOMM_CMD_HEADER_SIZE / 4); /* Number of Data frames */
        if (RSPI_SUCCESS != spi_err)
        {   /* Restore flag and state */
            s_fwupcomm_spi_tx_end_flag  = true;
            s_fwupcomm_spi_tx_state     = s_fwupcomm_spi_tx_state_bup;
            s_send_packet_size          = 0x00U;

            return FWUPCOMM_ERR_CH_SEND;
        }
 #else  /* SCI FIT, 8-bit Data Access */
        spi_err = R_SCI_SendReceive(s_fwupcomm_sci_sspi_handle,
                                    sp_send_packet_buffer,
                                    dest,
                                    FWUPCOMM_CMD_HEADER_SIZE);  /* Data size */
        if (SCI_SUCCESS != spi_err)  /* SCI_ERR_XCVR_BUSY */
        {   /* Restore flag and state */
            s_fwupcomm_spi_tx_end_flag  = true;
            s_fwupcomm_spi_tx_state     = s_fwupcomm_spi_tx_state_bup;
            s_send_packet_size          = 0x00U;

            return FWUPCOMM_ERR_CH_SEND_BUSY;
        }
 #endif  /* RSPI FIT / SCI FIT */

        r_fwupcomm_rx_spi_send_recv_end(FWUPCOMM_SPI_TRANS_END_RESP_HEADER);

    }
    else /* With data */
    {
        /* Get the data length information on Response recv packet buffer */
        /* If Response packet has Data, send dummy data of Header size */
        /* No Interval */
        /* Secondary wait for clock */

        /* Setup to send Request dummy Data */
        /* Set the flag and the state */
        s_fwupcomm_spi_tx_end_flag  = false;
        s_fwupcomm_spi_tx_state_bup = s_fwupcomm_spi_tx_state;

        /* STATE_RESP_END is set, because Header is received.
         * so that set to STATE_RESP_HEADER (treat as Header received state) */
        s_fwupcomm_spi_tx_state     = FWUPCOMM_SPI_STATE_RESP_HEADER;

        /* Send Data */
 #if (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI)  /* RSPI FIT, 32-bit Data Access */
        spi_err = R_RSPI_WriteRead(s_fwupcomm_rspi_handle,
                                   s_fwupcomm_rspi_command,
                                   (void *)(sp_send_packet_buffer + FWUPCOMM_CMD_HEADER_SIZE),
                                   (void *)dest,
                                   (uint16_t)(s_resp_data_length_info / 4)); /* Number of Data frames, 2: Header Frames */
        if (RSPI_SUCCESS != spi_err)
        {   /* Restore flag and state */
            s_fwupcomm_spi_tx_end_flag  = true;
            s_fwupcomm_spi_tx_state     = s_fwupcomm_spi_tx_state_bup;
            s_send_packet_size          = 0x00U;

            return FWUPCOMM_ERR_CH_SEND;
        }
 #else  /* SCI FIT, 8-bit Data Access */
        spi_err = R_SCI_SendReceive(s_fwupcomm_sci_sspi_handle,
                                    sp_send_packet_buffer + FWUPCOMM_CMD_HEADER_SIZE,
                                    dest,
                                    s_resp_data_length_info);  /* Data size */
        if (SCI_SUCCESS != spi_err)  /* SCI_ERR_XCVR_BUSY */
        {   /* Restore flag and state */
            s_fwupcomm_spi_tx_end_flag  = true;
            s_fwupcomm_spi_tx_state     = s_fwupcomm_spi_tx_state_bup;
            s_send_packet_size          = 0x00U;

            return FWUPCOMM_ERR_CH_SEND_BUSY;
        }
 #endif  /* RSPI FIT / SCI FIT */

        /* Check the event error */
        if (0U != s_err_type)
        {
            return FWUPCOMM_ERR_CH_RECV;  /* Communication error */
        }

        r_fwupcomm_rx_spi_send_recv_end(FWUPCOMM_SPI_TRANS_END_RESP_DATA);

        s_resp_data_length_info = 0U; /* Clear length_info */

    }  /* (0U == s_resp_data_length_info) */

#else  /* Secondary */
    /* Check multiples of 4 */
    if (0U != (size & 0x03))
    {
        return FWUPCOMM_ERR_INVALID_ARG;
    }

    /* Check that size is FWUPCOMM_CMD_HEADER_SIZE at 1st reception reception */
    if (sp_recv_packet_buffer == dest)  /* for 1st reception reception */
    {
        if (FWUPCOMM_CMD_HEADER_SIZE != size)  /* size: FWUPCOMM_CMD_HEADER_SIZE */
        {
            return FWUPCOMM_ERR_INVALID_ARG;
        }
    }

    /* Check the received size (s_recv_packet_size),
     * so that do not check the Transmission/Reception End */

    /* s_recv_packet_size : 0, 4, 8, (Multiple of 4), ...
     *     0                        : Not received all packet,
     *     FWUPCOMM_CMD_HEADER_SIZE : Received packet for Header only packet
     *     4, 8, ...                : Received packet for header and Data packet */
    if (sp_recv_packet_buffer == dest)  /* for 1st reception */
    {
        if (0x00 == s_recv_packet_size)  /* Not received packet */
        {
            return FWUPCOMM_ERR_CH_RECV_NO_DATA;
        }
    }
    else  /* for 2nd reception (packet with Data) */
    {   /* Data Length information in s_recv_packet_buffer[] */
        if ((size + FWUPCOMM_CMD_HEADER_SIZE)   != s_recv_packet_size)
        {
            return FWUPCOMM_ERR_CH_RECV_NO_DATA;
        }
    }

    /* Check the event error */
    if (0U != s_err_type)
    {
        return FWUPCOMM_ERR_CH_RECV;  /* Communication error */
    }

#endif  /* Primary / Secondary */

    return FWUPCOMM_SUCCESS;
}
/**********************************************************************************************************************
 End of function r_fwupcomm_rx_spi_recv
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_rx_spi_rx_reset
 * Description  : Clears flags, sets MISO to GPIO/Open-Drain output,
 *              :  setups the request packet reception and waits the clock.
 * Return Value : None
 *********************************************************************************************************************/
void r_fwupcomm_rx_spi_rx_reset(r_fwupcomm_hdl_t * hdl)
{
#if (FWUPCOMM_CFG_DEVICE_PRIMARY == 0)  /* Secondary only */

    s_my_device_address_flag    = FWUPCOMM_NOT_MY_DEVICE_ADDRESS;

    /* Set the state */
    s_fwupcomm_spi_tx_end_flag  = false;
    s_fwupcomm_spi_rx_state     = FWUPCOMM_SPI_STATE_REQ_START;  /* Recv: REQ */
    s_err_type                  = 0x00;

    /* Clear the reception packet size */
    s_recv_packet_size          = 0x00;

    /* Set MISO to GPIO/Open-Drain output */
    FWUPCOMM_SPI_MISO_PODR  = FWUPCOMM_SPI_GPIO_PODR_H;    /* Set MISO to H output */
    FWUPCOMM_SPI_MISO_ODR   = FWUPCOMM_SPI_GPIO_ODR_OD;    /* Set MISO to Open-Drain output */
    FWUPCOMM_SPI_MISO_PMR   = FWUPCOMM_SPI_GPIO_PMR_GPIO;  /* Set MISO to GPIO mode */
    if (FWUPCOMM_SPI_GPIO_PMR_GPIO  == FWUPCOMM_SPI_MISO_PMR)
    {
        R_BSP_NOP();
    }

    /* Setup reception and wait for clock */
 #if (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI)  /* RSPI FIT, 32-bit Data Access */
    rspi_err_t      spi_err;

    spi_err = R_RSPI_WriteRead(s_fwupcomm_rspi_handle,
                               s_fwupcomm_rspi_command,
                               (void *)sp_send_packet_buffer,
                               (void *)sp_recv_packet_buffer,
                               (uint16_t)(FWUPCOMM_CMD_HEADER_SIZE / 4)); /* Number of Data frames */
    if (RSPI_SUCCESS != spi_err)
    {
        if (RSPI_ERR_INVALID_ARG == spi_err)  /* Hardware error */
        {
            s_fwupcomm_spi_tx_end_flag  = true;
            s_fwupcomm_spi_rx_state     = FWUPCOMM_SPI_STATE_RESP_END;
            /* return FWUPCOMM_ERR_INVALID_ARG; */
        }
        else
        {   /* Restore the Reception state */
            s_fwupcomm_spi_tx_end_flag  = true;
            s_fwupcomm_spi_rx_state     = FWUPCOMM_SPI_STATE_RESP_END;
            /* return FWUPCOMM_ERR_CH_SEND_BUSY; */
        }
    }
 #else  /* SCI FIT; Not supported because the slave mode is not supported. */
 #endif  /* RSPI FIT / SCI FIT */
#endif  /* Secondary Only */
}
/**********************************************************************************************************************
 End of function r_fwupcomm_rx_spi_rx_reset
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_rx_spi_send_recv_end
 * Description  : Waits for transmission end and Reception end for r_fwupcomm_rx_spi_send()
 * Return Value : None
 *********************************************************************************************************************/
static void r_fwupcomm_rx_spi_send_recv_end(uint8_t mode)
{
#if (FWUPCOMM_CFG_DEVICE_PRIMARY == 1)  /* Primary */
    /* Primary state: Request Transmission -> Response Reception
     *
     * Request Transmission Start   : FWUPCOMM_SPI_STATE_REQ_START
     * Request without Data
     *  Transmission End            : FWUPCOMM_SPI_STATE_REQ_END (Packet Transmitted)
     * Request with Data
     *  Transmitting in progress    : FWUPCOMM_SPI_STATE_REQ_HEADER (Header Transmitted)
     *  Transmission End            : FWUPCOMM_SPI_STATE_REQ_END (Packet Transmitted)
     *
     * Response Reception Start (Dummy Clock Transmission Start for Reception)
     *                              : FWUPCOMM_SPI_STATE_RESP_START
     * Request without Data
     *  Reception End               : FWUPCOMM_SPI_STATE_RESP_END (Packet received)
     * Request with Data
     *  Receiving in progress       : FWUPCOMM_SPI_STATE_RESP_HEADER (Header Received state)
     *  Reception End               : FWUPCOMM_SPI_STATE_RESP_END (Packet Received)
     *  Reception Error End         : FWUPCOMM_SPI_STATE_RESP_DATA_LENGTH_ERROR
     */
    if (FWUPCOMM_SPI_TRANS_END_REQ_HEADER == mode)  /* Request Header Transmission */
    {
        while (FWUPCOMM_SPI_STATE_REQ_START == s_fwupcomm_spi_tx_state)  /* Wait for HEADER or END or ERROR */
        {
            R_BSP_NOP();
        }
    }
    else if (FWUPCOMM_SPI_TRANS_END_REQ_DATA == mode)  /* Request Data Transmission */
    {
        while ((FWUPCOMM_SPI_STATE_REQ_START    == s_fwupcomm_spi_tx_state)  /* Wait for END or ERROR */
            || (FWUPCOMM_SPI_STATE_REQ_HEADER   == s_fwupcomm_spi_tx_state))
        {
            R_BSP_NOP();
        }
    }
    else if (FWUPCOMM_SPI_TRANS_END_RESP_HEADER == mode)  /* Response Header Reception */
    {
        while (FWUPCOMM_SPI_STATE_RESP_START    == s_fwupcomm_spi_tx_state)  /* Wait for HEADER or END or ERROR */
        {
            R_BSP_NOP();
        }
    }
    else if (FWUPCOMM_SPI_TRANS_END_RESP_DATA == mode)  /* Response Data Reception */
    {
        while ((FWUPCOMM_SPI_STATE_RESP_START   == s_fwupcomm_spi_tx_state)  /* Wait for END or ERROR */
            || (FWUPCOMM_SPI_STATE_RESP_HEADER  == s_fwupcomm_spi_tx_state))
        {
            R_BSP_NOP();
        }
    }
    else
    {
         R_BSP_NOP();
    }

#else  /* Secondary */
    /* Secondary state: Request Reception -> Response Transmission
     *
     * Request Reception Start (Clock Wait state)
     *                              : FWUPCOMM_SPI_STATE_REQ_START
     * Request without Data
     *  Reception End               : FWUPCOMM_SPI_STATE_REQ_END (Packet Received)
     * Request with Data
     *  Receiving in progress       : FWUPCOMM_SPI_STATE_REQ_HEADER (Header Received)
     *  Reception End               : FWUPCOMM_SPI_STATE_REQ_END (Packet Received)
     *
     * Response Transmission Start (Clock Wait state)
     *                              : FWUPCOMM_SPI_STATE_RESP_START
     * Request without Data
     *  Transmission End            : FWUPCOMM_SPI_STATE_RESP_END (Packet Transmitted)
     * Request with Data
     *  Transmitting in progress    : FWUPCOMM_SPI_STATE_RESP_HEADER (Header Transmitted)
     *  Transmission End            : FWUPCOMM_SPI_STATE_RESP_END (Packet Transmitted)
     *  Transmission Error End      : FWUPCOMM_SPI_STATE_RESP_DATA_LENGTH_ERROR
     */
    if (FWUPCOMM_SPI_TRANS_END_REQ_HEADER == mode)  /* Request Header Reception */
    {
        while (FWUPCOMM_SPI_STATE_REQ_START == s_fwupcomm_spi_rx_state)  /* Wait for HEADER or END or ERROR */
        {
            R_BSP_NOP();
        }
    }
    else if (FWUPCOMM_SPI_TRANS_END_REQ_DATA == mode)  /* Request Data Reception */
    {
        while ((FWUPCOMM_SPI_STATE_REQ_START    == s_fwupcomm_spi_rx_state)  /* Wait for END or ERROR */
            || (FWUPCOMM_SPI_STATE_REQ_HEADER   == s_fwupcomm_spi_rx_state))
        {
            R_BSP_NOP();
        }
    }
    else if (FWUPCOMM_SPI_TRANS_END_RESP_HEADER == mode)  /* Response Header Reception */
    {
        while (FWUPCOMM_SPI_STATE_RESP_START    == s_fwupcomm_spi_rx_state)  /* Wait for HEADER or END or ERROR */
        {
            R_BSP_NOP();
        }
    }
    else if (FWUPCOMM_SPI_TRANS_END_RESP_DATA == mode)  /* Response Data Reception */
    {
        while ((FWUPCOMM_SPI_STATE_RESP_START   == s_fwupcomm_spi_rx_state)  /* Wait for END or ERROR */
            || (FWUPCOMM_SPI_STATE_RESP_HEADER  == s_fwupcomm_spi_rx_state))
        {
            R_BSP_NOP();
        }
    }
    else
    {
        R_BSP_NOP();
    }
#endif  /* Primary / Secondary */
}
/**********************************************************************************************************************
 End of function r_fwupcomm_rx_spi_send_recv_end
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_rx_spi_cb
 * Description  : Callback functions for RSPI FIT/SI FIT module.
 * Argument     : p_data: Pointer to the argument passed from the RSPI FIT/SCI FIT module
 * Return Value : None
 *********************************************************************************************************************/
static void r_fwupcomm_rx_spi_cb(void * p_data)
{
#if (FWUPCOMM_CFG_DEVICE_PRIMARY == 1)  /* Primary */
 #if (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI) /* RSPI FIT (Primary/Secondary Common) */
    s_event = (*(rspi_callback_data_t *)p_data).event_code;
 #else  /* SCI FIT (Primary Only) */
    sp_cb_args = (sci_cb_args_t *)p_data;
    s_event = sp_cb_args->event;
 #endif  /* RSPI FIT / SCI FIT */

    switch (s_event)
    {
        case FWUPCOMM_SPI_EVENT_RECVEND: /* Send/Receive Transmission End */
           s_fwupcomm_spi_tx_end_flag = true;  /* Transmitted */

           if (FWUPCOMM_SPI_STATE_REQ_START == s_fwupcomm_spi_tx_state)  /* For Request */
            {
                s_fwupcomm_spi_tx_state = FWUPCOMM_SPI_STATE_REQ_HEADER;  /* Transmitted Header */

                if (FWUPCOMM_CMD_HEADER_SIZE >= s_send_packet_size)  /* Request packet: 8-byte (without Data) */
                {
                    s_fwupcomm_spi_tx_state = FWUPCOMM_SPI_STATE_REQ_END;  /* Received Request packet */

                    /* Next: Waiting for Secondary ready, after that Response dummy data transmission */
                }
                else /* Request packet > 8-byte (with Data) */
                {
                    ;
                }
            }
            else if (FWUPCOMM_SPI_STATE_REQ_HEADER == s_fwupcomm_spi_tx_state)  /* For Request (Data) */
            {  /* Data Transmission: Request packet (with Data) > 8-byte */
                s_fwupcomm_spi_tx_state = FWUPCOMM_SPI_STATE_REQ_END;  /* Transmitted Request packet, Not TEND INT *?

                /* Next: Waiting for Secondary ready, after that Response dummy data transmission (main process) */
            }
            else if (FWUPCOMM_SPI_STATE_RESP_START == s_fwupcomm_spi_tx_state)  /* For Response (Dummy data transmission) */
            {
                s_fwupcomm_spi_tx_state = FWUPCOMM_SPI_STATE_RESP_HEADER;

                s_resp_data_length_info = *(sp_recv_packet_buffer + 6) + (*(sp_recv_packet_buffer + 7) << 8);

                /* Check the data length to prevent RAM corruption */
                if ((FWUPCOMM_CFG_RECV_PACKET_BUFFER_SIZE - FWUPCOMM_CMD_HEADER_SIZE) < s_resp_data_length_info)
                {
                    s_resp_data_length_info = 1U;  /* Set invalid size  0x01 or 0xFFFF */
                }

                /* Check multiples of 4 */
                if (0U != (s_resp_data_length_info & 0x03))
                {
                    if (0U != s_resp_data_length_info)
                    {
                        s_resp_data_length_info = 1U;  /* Set invalid size */
                    }
                }

                if (0U == s_resp_data_length_info)  /* Response packet: 8-byte (without Data) */
                {
                    s_fwupcomm_spi_tx_state = FWUPCOMM_SPI_STATE_RESP_END;  /* Transmitted Response dummy data */
                }
                else if (1U == s_resp_data_length_info)  /* Invalid size */
                {
                    s_fwupcomm_spi_tx_state = FWUPCOMM_SPI_STATE_RESP_DATA_LENGTH_ERROR;
                }
                else /* Response packet > 8-byte (with Data) */
                {
                }
            }
            else if (FWUPCOMM_SPI_STATE_RESP_HEADER == s_fwupcomm_spi_tx_state)  /* For Response */
            {
                s_fwupcomm_spi_tx_state = FWUPCOMM_SPI_STATE_RESP_END;
            }
            else  /* Unexpected case */
            {
                s_fwupcomm_spi_tx_state = FWUPCOMM_SPI_STATE_OTHER_ERROR;
                s_err_type              = s_event;
            }
            break;

        default:
            break;
    }  /* switch */

#else  /* Secondary */
 #if (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI)  /* RSPI FIT */
    s_event = (*(rspi_callback_data_t *)p_data).event_code;
 #else  /* SCI FIT; Not supported because the slave mode is not supported. */
 #endif
    switch (s_event)
    {
        case FWUPCOMM_SPI_EVENT_RECVEND: /* SPI RX End, Secondary: Header size received */
            s_fwupcomm_spi_tx_end_flag  = true;  /* Transmitted */

            if (FWUPCOMM_SPI_STATE_REQ_START == s_fwupcomm_spi_rx_state)  /* For Request */
            {
                s_fwupcomm_spi_rx_state = FWUPCOMM_SPI_STATE_REQ_HEADER;  /* Received Header */
                s_req_data_length_info  = (*(sp_recv_packet_buffer + 6)) + (*(sp_recv_packet_buffer + 7) << 8);

                if (0U == s_req_data_length_info)  /* Request packet: 8-byte (without Data) */
                {
                    s_recv_packet_size      = FWUPCOMM_CMD_HEADER_SIZE;
                    s_fwupcomm_spi_rx_state = FWUPCOMM_SPI_STATE_REQ_END;  /* Received Request packet */

                    /* If My Device Address, set MISO to L (Indicate the Secondary busy state) */
                    if ((FWUPCOMM_CFG_DEVICE_ADDRESS       == (*sp_recv_packet_buffer))  /* My Device Address */
                    || 	(FWUPCOMM_BROADCAST_DEVICE_ADDRESS == (*sp_recv_packet_buffer)))
                    {
                        s_my_device_address_flag    =  FWUPCOMM_MY_DEVICE_ADDRESS;

                        /* Set MISO to L (Indicate the Secondary busy state) */
                        FWUPCOMM_SPI_MISO_PODR          =  FWUPCOMM_SPI_GPIO_PODR_L;  /* L output setting */
                        if (FWUPCOMM_SPI_GPIO_PODR_L    == FWUPCOMM_SPI_MISO_PODR)
                        {
                            R_BSP_NOP();
                        }
                    }
                    /* Next: Response transmission/reception */
                }
                else /* Request packet > 8-byte (with Data) */
                {
                    /* Setup Data reception */
#if (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI)  /* RSPI FIT, 32-bit Data Access */
                    R_RSPI_WriteRead(s_fwupcomm_rspi_handle,
                                     s_fwupcomm_rspi_command,
                                     (void *)(sp_send_packet_buffer + FWUPCOMM_CMD_HEADER_SIZE),
                                     (void *)(sp_recv_packet_buffer + FWUPCOMM_CMD_HEADER_SIZE),
                                     (uint16_t)(s_req_data_length_info / 4));  /* Number of Data frames */
#else  /* SCI FIT; Not supported because the slave mode is not supported. */
#endif  /* RSPI FIT / SCI FIT */
                }
            }
            else if (FWUPCOMM_SPI_STATE_REQ_HEADER  == s_fwupcomm_spi_rx_state)  /* For Request (Data) */
            {  /* Data Reception: Request packet > 8-byte (with Data) */
                s_recv_packet_size      = s_req_data_length_info + FWUPCOMM_CMD_HEADER_SIZE;
                s_fwupcomm_spi_rx_state = FWUPCOMM_SPI_STATE_REQ_END;  /* Received Request packet */

                /* If My Device Address, set MISO to L (Indicate the Secondary busy state) */
                if ((FWUPCOMM_CFG_DEVICE_ADDRESS       == (*sp_recv_packet_buffer))  /* My Device Address */
                ||  (FWUPCOMM_BROADCAST_DEVICE_ADDRESS == (*sp_recv_packet_buffer)))
				{
                    s_my_device_address_flag    = FWUPCOMM_MY_DEVICE_ADDRESS;

                    /* Set MISO to L (Indicate the Secondary busy state) */
                    FWUPCOMM_SPI_MISO_PODR          =  FWUPCOMM_SPI_GPIO_PODR_L;  /* L output setting */
                    if (FWUPCOMM_SPI_GPIO_PODR_L    != FWUPCOMM_SPI_MISO_PODR)  /* Optimization suppression (Not same as the above) */
                    {
                        R_BSP_NOP();
                    }
                }

                /* Next: Response transmission/reception */
            }
            else if (FWUPCOMM_SPI_STATE_RESP_START  == s_fwupcomm_spi_rx_state)  /* For Response */
            {
                s_fwupcomm_spi_rx_state = FWUPCOMM_SPI_STATE_RESP_END;
            }
            else  /* Unexpected case */
            {
                s_fwupcomm_spi_rx_state = FWUPCOMM_SPI_STATE_OTHER_ERROR;
                s_err_type              = s_event;
            }
            break;

#if (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI)  /* RSPI FIT */
        case FWUPCOMM_SPI_EVENT_ERROR_MODE_FAULT:  /* RSPI_EVT_ERR_MODE_FAULT */
            s_fwupcomm_spi_rx_state = FWUPCOMM_SPI_STATE_ERROR_MODE_FAULT;
            s_err_type              = s_event;
            break;

        case FWUPCOMM_SPI_EVENT_ERROR_OVER_FLOW:  /* RSPI_EVT_ERR_READ_OVF */
            s_fwupcomm_spi_rx_state = FWUPCOMM_SPI_STATE_ERROR_OVER_FLOW;
            s_err_type              = s_event;
            break;

        case FWUPCOMM_SPI_EVENT_ERROR_PARITY:  /* RSPI_EVT_ERR_PARITY (Does not occur) */
            s_fwupcomm_spi_rx_state = FWUPCOMM_SPI_STATE_ERROR_PARITY;
            s_err_type              = s_event;
            break;

        case FWUPCOMM_SPI_EVENT_ERROR_UNDER_RUN:  /* RSPI_EVT_ERR_UNDER_RUN */
            s_fwupcomm_spi_rx_state = FWUPCOMM_SPI_EVENT_ERROR_UNDER_RUN;
            s_err_type              = s_event;
            break;

        case FWUPCOMM_SPI_EVENT_ERROR:  /* RSPI_EVT_ERR_UNDEF */
            s_fwupcomm_spi_rx_state = FWUPCOMM_SPI_STATE_OTHER_ERROR;
            s_err_type              = s_event;
            break;
#else  /* SCI FIT; Not supported because the slave mode is not supported. */
#endif  /* RSPI FIT / SCI FIT */

        default:
            s_fwupcomm_spi_rx_state = FWUPCOMM_SPI_STATE_OTHER_ERROR;
            s_err_type              = s_event;
            break;
    }

#endif  /* Primary / Secondary */
}
/**********************************************************************************************************************
 End of function r_fwupcomm_rx_spi_cb
 *********************************************************************************************************************/

#endif /* ((FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_SCI_SPI)||(FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI)) */
