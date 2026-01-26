/*
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
 * File Name    : r_fwupcomm_rx_spi.h
 * Description  : Functions for the RX MCU to communicate using RSPI/SCI SPI.
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           20.10.2025 1.00    Initial Release
 *           11.11.2025 1.10    Added SPI communication support.
 ***********************************************************************************************************************/

#ifndef FWUPCOMM_SRC_CONNECTIVITY_R_FWUPCOMM_RX_SPI_H_
#define FWUPCOMM_SRC_CONNECTIVITY_R_FWUPCOMM_RX_SPI_H_

#include "r_fwupcomm_config.h"
#include "r_fwupcomm_ch.h"
#include "r_fwupcomm_private.h"


#if (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI)  /* RSPI */
#include "r_rspi_rx_if.h"
#include "r_rspi_rx_pinset.h"


#if FWUPCOMM_CFG_SPI_CHANNEL == (0)
    #define FWUPCOMM_RSPI_PINSET()  R_RSPI_PinSet_RSPI0()
#elif FWUPCOMM_CFG_SPI_CHANNEL == (1)
    #define FWUPCOMM_RSPI_PINSET()  R_RSPI_PinSet_RSPI1()
#elif FWUPCOMM_CFG_SPI_CHANNEL == (2)
    #define FWUPCOMM_RSPI_PINSET()  R_RSPI_PinSet_RSPI2()
#else
    #error "Error! Invalid setting for FWUPCOMM_CFG_SPI_CHANNEL in r_fwupcomm_config.h"
#endif

#else  /* SCI */
#include "r_sci_rx_if.h"
#include "r_sci_rx_pinset.h"


#if FWUPCOMM_CFG_SPI_CHANNEL == (0)
    #define FWUPCOMM_SCI_PINSET()   R_SCI_PinSet_SCI0()
#elif FWUPCOMM_CFG_SPI_CHANNEL == (1)
    #define FWUPCOMM_SCI_PINSET()   R_SCI_PinSet_SCI1()
#elif FWUPCOMM_CFG_SPI_CHANNEL == (2)
    #define FWUPCOMM_SCI_PINSET()   R_SCI_PinSet_SCI2()
#elif FWUPCOMM_CFG_SPI_CHANNEL == (3)
    #define FWUPCOMM_SCI_PINSET()   R_SCI_PinSet_SCI3()
#elif FWUPCOMM_CFG_SPI_CHANNEL == (4)
    #define FWUPCOMM_SCI_PINSET()   R_SCI_PinSet_SCI4()
#elif FWUPCOMM_CFG_SPI_CHANNEL == (5)
    #define FWUPCOMM_SCI_PINSET()   R_SCI_PinSet_SCI5()
#elif FWUPCOMM_CFG_SPI_CHANNEL == (6)
    #define FWUPCOMM_SCI_PINSET()   R_SCI_PinSet_SCI6()
#elif FWUPCOMM_CFG_SPI_CHANNEL == (7)
    #define FWUPCOMM_SCI_PINSET()   R_SCI_PinSet_SCI7()
#elif FWUPCOMM_CFG_SPI_CHANNEL == (8)
    #define FWUPCOMM_SCI_PINSET()   R_SCI_PinSet_SCI8()
#elif FWUPCOMM_CFG_SPI_CHANNEL == (9)
    #define FWUPCOMM_SCI_PINSET()   R_SCI_PinSet_SCI9()
#elif FWUPCOMM_CFG_SPI_CHANNEL == (10)
    #define FWUPCOMM_SCI_PINSET()   R_SCI_PinSet_SCI10()
#elif FWUPCOMM_CFG_SPI_CHANNEL == (11)
    #define FWUPCOMM_SCI_PINSET()   R_SCI_PinSet_SCI11()
#elif FWUPCOMM_CFG_SPI_CHANNEL == (12)
    #define FWUPCOMM_SCI_PINSET()   R_SCI_PinSet_SCI12()
#else
    #error "Error! Invalid setting for FWUPCOMM_CFG_SPI_CHANNEL in r_fwupcomm_config.h"
#endif
#endif  /* RSPI / SCI */

/* Port No Formatting (PDR/PODR/PIDR/PMR) */
#if   ('0' == FWUPCOMM_CFG_SPI_MISO_PORTNO)
    #define FWUPCOMM_SPI_MISO_PORT          0
#elif ('1' == FWUPCOMM_CFG_SPI_MISO_PORTNO)
    #define FWUPCOMM_SPI_MISO_PORT          1
#elif ('2' == FWUPCOMM_CFG_SPI_MISO_PORTNO)
    #define FWUPCOMM_SPI_MISO_PORT          2
#elif ('3' == FWUPCOMM_CFG_SPI_MISO_PORTNO)
    #define FWUPCOMM_SPI_MISO_PORT          3
#elif ('4' == FWUPCOMM_CFG_SPI_MISO_PORTNO)
    #define FWUPCOMM_SPI_MISO_PORT          4
#elif ('5' == FWUPCOMM_CFG_SPI_MISO_PORTNO)
    #define FWUPCOMM_SPI_MISO_PORT          5
#elif ('6' == FWUPCOMM_CFG_SPI_MISO_PORTNO)
    #define FWUPCOMM_SPI_MISO_PORT          6
#elif ('7' == FWUPCOMM_CFG_SPI_MISO_PORTNO)
    #define FWUPCOMM_SPI_MISO_PORT          7
#elif ('8' == FWUPCOMM_CFG_SPI_MISO_PORTNO)
    #define FWUPCOMM_SPI_MISO_PORT          8
#elif ('9' == FWUPCOMM_CFG_SPI_MISO_PORTNO)
    #define FWUPCOMM_SPI_MISO_PORT          9
#elif (('A' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('a' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          A
#elif (('B' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('b' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          B
#elif (('C' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('c' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          C
#elif (('D' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('d' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          D
#elif (('E' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('e' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          E
#elif (('F' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('f' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          F
#elif (('G' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('g' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          G
#elif (('H' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('h' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          H
#elif (('I' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('i' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          I
#elif (('J' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('j' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          J
#elif (('K' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('k' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          K
#elif (('L' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('l' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          L
#elif (('M' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('m' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          M
#elif (('N' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('n' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          N
#elif (('O' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('o' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          O
#elif (('P' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('p' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          P
#elif (('Q' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('q' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          Q
#elif (('R' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('r' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          R
#elif (('S' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('s' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          S
#elif (('T' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('t' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          T
#elif (('U' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('u' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          U
#elif (('V' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('v' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          V
#elif (('W' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('w' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          W
#elif (('X' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('x' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          X
#elif (('Y' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('y' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          Y
#elif (('Z' == FWUPCOMM_CFG_SPI_MISO_PORTNO) || ('z' == FWUPCOMM_CFG_SPI_MISO_PORTNO))
    #define FWUPCOMM_SPI_MISO_PORT          Z
#endif

/* Port No/Bit No Formatting (ODR) */
#if ('4' > FWUPCOMM_CFG_SPI_MISO_BITNO)
    #define FWUPCOMM_SPI_MISO_ODR_REG       0  /* ODR0 */
#else
    #define FWUPCOMM_SPI_MISO_ODR_REG       1  /* ODR1 */
#endif
        
/* For PE1, OK in order to use CMOS/OpenDrain. Refer to ODR0 for PE1 */
#if   (('0' == FWUPCOMM_CFG_SPI_MISO_BITNO) || ('4' == FWUPCOMM_CFG_SPI_MISO_BITNO))
    #define FWUPCOMM_SPI_MISO_ODR_BIT       0
#elif (('1' == FWUPCOMM_CFG_SPI_MISO_BITNO) || ('5' == FWUPCOMM_CFG_SPI_MISO_BITNO))
    #define FWUPCOMM_SPI_MISO_ODR_BIT       2
#elif (('2' == FWUPCOMM_CFG_SPI_MISO_BITNO) || ('6' == FWUPCOMM_CFG_SPI_MISO_BITNO))
    #define FWUPCOMM_SPI_MISO_ODR_BIT       4
#elif (('3' == FWUPCOMM_CFG_SPI_MISO_BITNO) || ('7' == FWUPCOMM_CFG_SPI_MISO_BITNO))
    #define FWUPCOMM_SPI_MISO_ODR_BIT       6
#endif

/* Bit No Formatting */
#if   ('0' == FWUPCOMM_CFG_SPI_MISO_BITNO)
    #define FWUPCOMM_SPI_MISO_BIT           0
#elif ('1' == FWUPCOMM_CFG_SPI_MISO_BITNO)
    #define FWUPCOMM_SPI_MISO_BIT           1
#elif ('2' == FWUPCOMM_CFG_SPI_MISO_BITNO)
    #define FWUPCOMM_SPI_MISO_BIT           2
#elif ('3' == FWUPCOMM_CFG_SPI_MISO_BITNO)
    #define FWUPCOMM_SPI_MISO_BIT           3
#elif ('4' == FWUPCOMM_CFG_SPI_MISO_BITNO)
    #define FWUPCOMM_SPI_MISO_BIT           4
#elif ('5' == FWUPCOMM_CFG_SPI_MISO_BITNO)
    #define FWUPCOMM_SPI_MISO_BIT           5
#elif ('6' == FWUPCOMM_CFG_SPI_MISO_BITNO)
    #define FWUPCOMM_SPI_MISO_BIT           6
#elif ('7' == FWUPCOMM_CFG_SPI_MISO_BITNO)
    #define FWUPCOMM_SPI_MISO_BIT           7
#endif

/* **** Definitions of string conversion to access I/O registers **** */
#define FWUPCOMM_CAT_PDR(p, b)          PORT ## p.PDR.BIT.B ## b        /* "PORTp.PDR.BIT.Bb"  */
#define FWUPCOMM_PDR(p, b)              FWUPCOMM_CAT_PDR(p, b)
#define FWUPCOMM_SPI_MISO_PDR           FWUPCOMM_PDR(FWUPCOMM_SPI_MISO_PORT, FWUPCOMM_SPI_MISO_BIT)

#if 0
#define FWUPCOMM_SPI_MISO_PODR          FWUPCOMM_PODR(FWUPCOMM_SPI_MISO_PORT, FWUPCOMM_SPI_MISO_BIT)
#define FWUPCOMM_PODR(p, b)             FWUPCOMM_CAT_PODR(p, b)
#define FWUPCOMM_CAT_PODR(p, b)         PORT ## p.PODR.BIT.B ## b      /* "PORTp.PODR.BIT.Bb" */
#else
#define FWUPCOMM_CAT_PODR(p, b)         PORT ## p.PODR.BIT.B ## b      /* "PORTp.PODR.BIT.Bb" */
#define FWUPCOMM_PODR(p, b)             FWUPCOMM_CAT_PODR(p, b)
#define FWUPCOMM_SPI_MISO_PODR          FWUPCOMM_PODR(FWUPCOMM_SPI_MISO_PORT, FWUPCOMM_SPI_MISO_BIT)
#endif

#define FWUPCOMM_CAT_PIDR(p, b)         PORT ## p.PIDR.BIT.B ## b       /* "PORTp.PIDR.BIT.Bb" */
#define FWUPCOMM_PIDR(p, b)             FWUPCOMM_CAT_PIDR(p, b)
#define FWUPCOMM_SPI_MISO_PIDR          FWUPCOMM_PIDR(FWUPCOMM_SPI_MISO_PORT, FWUPCOMM_SPI_MISO_BIT)

#define FWUPCOMM_CAT_PMR(p, b)          PORT ## p.PMR.BIT.B ## b        /* "PORTp.PMR.BIT.Bb"  */
#define FWUPCOMM_PMR(p, b)              FWUPCOMM_CAT_PMR(p, b)
#define FWUPCOMM_SPI_MISO_PMR           FWUPCOMM_PMR(FWUPCOMM_SPI_MISO_PORT, FWUPCOMM_SPI_MISO_BIT)

#define FWUPCOMM_CAT_ODR(p, r, b)       PORT ## p.ODR ## r.BIT.B ## b   /* "PORTp.ODRr.BIT.Bb" */
#define FWUPCOMM_ODR(p, r, b)           FWUPCOMM_CAT_ODR(p, r, b)
#define FWUPCOMM_SPI_MISO_ODR           FWUPCOMM_ODR(FWUPCOMM_SPI_MISO_PORT, FWUPCOMM_SPI_MISO_ODR_REG, \
                                                                             FWUPCOMM_SPI_MISO_ODR_BIT)

/* SPI GPIO */
/* Secondary */
#define FWUPCOMM_SPI_GPIO_PDR_IN        (0U)    /* Input */
#define FWUPCOMM_SPI_GPIO_PDR_OUT       (1U)    /* Output */

#define FWUPCOMM_SPI_GPIO_PODR_L        (0U)    /* L output */
#define FWUPCOMM_SPI_GPIO_PODR_H        (1U)    /* H output or Open-Drain */

#define FWUPCOMM_SPI_GPIO_PMR_GPIO      (0U)    /* GPIO setting */
#define FWUPCOMM_SPI_GPIO_PMR_PERI      (1U)    /* Peripheral setting */

#define FWUPCOMM_SPI_GPIO_ODR_CMOS      (0U)    /* CMOS setting */
#define FWUPCOMM_SPI_GPIO_ODR_OD        (1U)    /* Open-Drain setting */

/* SPI Event */
#if (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_RSPI_SPI)  /* RSPI */
#define FWUPCOMM_SPI_EVENT_RECVEND                  (RSPI_EVT_TRANSFER_COMPLETE)
#define FWUPCOMM_SPI_EVENT_ERROR_MODE_FAULT         (RSPI_EVT_ERR_MODE_FAULT)
#define FWUPCOMM_SPI_EVENT_ERROR_OVER_FLOW          (RSPI_EVT_ERR_READ_OVF)
#define FWUPCOMM_SPI_EVENT_ERROR_PARITY             (RSPI_EVT_ERR_PARITY)
#define FWUPCOMM_SPI_EVENT_ERROR_UNDER_RUN          (RSPI_EVT_ERR_UNDER_RUN)
#define FWUPCOMM_SPI_EVENT_ERROR                    (RSPI_EVT_ERR_UNDEF)
#else  /* SCI */
#define FWUPCOMM_SPI_EVENT_RECVEND                  (SCI_EVT_XFER_DONE)
#define FWUPCOMM_SPI_EVENT_RX_SYNC_DONE             (SCI_EVT_RX_SYNC_DONE)   /* for DMAC */
#define FWUPCOMM_SPI_EVENT_ERROR_ABORTED            (SCI_EVT_XFER_ABORTED)
#define FWUPCOMM_SPI_EVENT_ERROR_OVER_FLOW          (SCI_EVT_OVFL_ERR)
#endif

/* SPI Transmission/Reception End Mode */
#define FWUPCOMM_SPI_TRANS_END_REQ_HEADER           (2U)    /* Request Header End mode */
#define FWUPCOMM_SPI_TRANS_END_REQ_DATA             (3U)    /* Request Data End mode */
#define FWUPCOMM_SPI_TRANS_END_RESP_HEADER          (5U)    /* Response Header End */
#define FWUPCOMM_SPI_TRANS_END_RESP_DATA            (6U)    /* Response Data End */

/* SPI State */
#define FWUPCOMM_SPI_STATE_REQ_START                (1U)    /* Request Transmission Start */
#define FWUPCOMM_SPI_STATE_REQ_HEADER               (2U)    /* Request Header Transmitted (Data not transmitted) */
#define FWUPCOMM_SPI_STATE_REQ_END                  (3U)    /* Request (Header and/or Data) Transmitted */
#define FWUPCOMM_SPI_STATE_RESP_START               (4U)    /* Response Transmission Start */
#define FWUPCOMM_SPI_STATE_RESP_HEADER              (5U)    /* Response Header Transmitted (Data not transmitted) */
#define FWUPCOMM_SPI_STATE_RESP_END                 (6U)    /* Response (Header and/or Data) Transmitted */
#define FWUPCOMM_SPI_STATE_RESP_DATA_LENGTH_ERROR   (7U)    /* Response Received data length error */
#define FWUPCOMM_SPI_STATE_RX_SYNC_DONE             (8U)    /* SYNC_DONE */
#define FWUPCOMM_SPI_STATE_OTHER_ERROR              (9U)    /* Unexpected Event error */
#define FWUPCOMM_SPI_STATE_ERROR_MODE_FAULT         (10U)   /* Unexpected Event error */
#define FWUPCOMM_SPI_STATE_ERROR_OVER_FLOW          (11U)   /* Unexpected Event error */
#define FWUPCOMM_SPI_STATE_ERROR_PARITY             (12U)   /* Unexpected Event error */
#define FWUPCOMM_SPI_STATE_ERROR_UNDER_RUN          (13U)   /* Unexpected Event error */
#define FWUPCOMM_SPI_STATE_ERROR_ABORTED            (14U)   /* Unexpected Event error */

/* SPI Transmission Interval */
#if (FWUPCOMM_CFG_DEVICE_PRIMARY == 1)  /* Primary */
#define FWUPCOMM_SPI_PRIMARY_SEND_INTERVAL_US                   (50U)    /* Transmission Interval */
#define FWUPCOMM_SPI_PRIMARY_SEND_MISO_H_DETECT_START_DELAY_US  (100U)   /* Primary MISO=H detection start delay */
#define FWUPCOMM_SPI_PRIMARY_SEND_MISO_H_DETECT_END_DELAY_US    (200U)   /* Primary MISO=H detection end delay */
#endif

/* Bus H charge Time */
/* Wait for a bus H charge time (Time Constant) of RC circuit
 * In case of Pull-Up register: 100kohm, Bus capacity: 100pF
 * Tau = 100k * 100pF = 10 * 10^(-6) = 10us -> Sets 2 x Tau(=20us) */
#define FWUPCOMM_SPI_BUS_H_CHARGE_WAIT_US           (20U)   /* [Secondary] */

/* SPI MISO = H Hold Time */
#define FWUPCOMM_SPI_MISO_HOLD_US                   (100U)  /* [Secondary] MISO Hold time after MISO = L -> H */

/* SPI Req-Resp Switching Waiting Time (Optional) */
#define FWUPCOMM_SPI_REQ_RESP_WAIT_US               (0U)    /* [Primary/Secondary] at end of send Request operation */


/* SPI Secondary Initialization L pulse Time */
#define FWUPCOMM_SPI_SECONDARY_INIT_L_PULSE_HOLD_MS (1U)    /* [Secondary] Initialization L Pulse time */

/* Structure variable defining the interface for packet communications */
extern r_fwupcomm_ch_api_t const r_fwupcomm_ch_api;

/* Communications interface functions */
fwupcomm_err_t  r_fwupcomm_rx_spi_open (r_fwupcomm_hdl_t * hdl);
void            r_fwupcomm_rx_spi_close (r_fwupcomm_hdl_t * hdl);
fwupcomm_err_t  r_fwupcomm_rx_spi_send (r_fwupcomm_hdl_t * hdl, uint8_t * const src, uint16_t size);
fwupcomm_err_t  r_fwupcomm_rx_spi_recv (r_fwupcomm_hdl_t * hdl, uint8_t * const dest, uint16_t size);
void            r_fwupcomm_rx_spi_rx_reset (r_fwupcomm_hdl_t * hdl);

#endif /* FWUPCOMM_SRC_CONNECTIVITY_R_FWUPCOMM_RX_SPI_H_ */

