/***********************************************************************************************************************
 * Copyright [2020-2023] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
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

#ifndef RM_COMMS_UART_R_UART_API_COMPLEMENT_H
#define RM_COMMS_UART_R_UART_API_COMPLEMENT_H


/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_smc_entry.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/* CLK SRC */
#ifndef SCI_CLK_INT
#define SCI_CLK_INT         (0x00U) /* use internal clock for baud generation */
#endif
#ifndef SCI_CLK_EXT8X
#define SCI_CLK_EXT8X       (0x03U) /* use external clock 8x baud rate (ASYNC) */
#endif
#ifndef SCI_CLK_EXT16X
#define SCI_CLK_EXT16X      (0x02U) /* use external clock 16x baud rate (ASYNC) */
#endif

/* Data size */
#ifndef SCI_DATA_7BIT
#define SCI_DATA_7BIT       (0x40U)
#endif
#ifndef SCI_DATA_8BIT
#define SCI_DATA_8BIT       (0x00U)
#endif

/* Parity enabled */
#ifndef SCI_PARITY_ON
#define SCI_PARITY_ON       (0x20U)
#endif
#ifndef SCI_PARITY_OFF
#define SCI_PARITY_OFF      (0x00U)
#endif

/* Parity type */
#ifndef SCI_ODD_PARITY
#define SCI_ODD_PARITY      (0x10U)
#endif
#ifndef SCI_EVEN_PARITY
#define SCI_EVEN_PARITY     (0x00U)
#endif

/* Stop bits */
#ifndef SCI_STOPBITS_2
#define SCI_STOPBITS_2      (0x08U)
#endif
#ifndef SCI_STOPBITS_1
#define SCI_STOPBITS_1      (0x00U)
#endif

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
#ifndef UART_INSTANCE
#define UART_INSTANCE
/* UART APIs */
typedef struct st_uart_module_api
{
    void        (* start)(void);                                    ///< Start
    void        (* stop)(void);                                     ///< Stop
    MD_STATUS   (* write)(uint8_t * const tx_buf, uint16_t tx_num); ///< Write
    MD_STATUS   (* read)(uint8_t * const rx_buf, uint16_t rx_num);  ///< Read
} uart_module_api_t;

/* Open/Close API called in API */
typedef struct st_uart_driver_app_api
{
    void (* open)(void);
    void (* close)(void);
} uart_driver_app_api_t;

/* RX Driver Instance */
typedef struct st_uart_instance
{
    uart_module_api_t const     * p_module_api; // For calling in module
    uart_driver_app_api_t const * p_app_api;    // For calling in application
} uart_instance_t;
#endif /* UART_INSTANCE */

#endif /* RM_COMMS_UART_R_UART_API_COMPLEMENT_H */
