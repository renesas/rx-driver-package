/*
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
 * File Name    : r_fwupcomm_ch.h
 * Description  : Header file for the communications interface.
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           20.05.2025 1.00    Initial Release
 *           11.11.2025 1.10    Added SPI communication support.
 ***********************************************************************************************************************/

#ifndef FWUPCOMM_SRC_CONNECTIVITY_R_FWUPCOMM_CH_H_
#define FWUPCOMM_SRC_CONNECTIVITY_R_FWUPCOMM_CH_H_

#include "r_fwupcomm_if.h"

/* Structure defining the interface for packet communications. */
typedef struct r_fwupcomm_ch_api
{
    /* Opens a communications channel. */
    fwupcomm_err_t (* open)(r_fwupcomm_hdl_t * hdl);
    /* Closes a communications channel. */
    void           (* close)(r_fwupcomm_hdl_t * hdl);
    /* Sends data by using a communications channel. */
    fwupcomm_err_t (* send)(r_fwupcomm_hdl_t * hdl, uint8_t * src, uint16_t size);
    /* Receives data by using a communications channel. */
    fwupcomm_err_t (* recv)(r_fwupcomm_hdl_t * hdl, uint8_t * dest, uint16_t size);
    /* Empties the reception buffer of the communications channel. */
    void           (* rx_reset)(r_fwupcomm_hdl_t * hdl);
} r_fwupcomm_ch_api_t;

#endif /* FWUPCOMM_SRC_CONNECTIVITY_R_FWUPCOMM_CH_H_ */
