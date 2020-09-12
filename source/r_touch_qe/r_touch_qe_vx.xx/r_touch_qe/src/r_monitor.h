/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_qemonitor.h
* Version      : 1.00
* Device(s)    : R5F5113xAxFP,R5F5231xAxFP,R5F51305AxFN
* Description  : This file contains definitions specific to the QETOUCH driver monitor interface.
***********************************************************************************************************************/

/***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 04.10.2018 1.00    First Release
***********************************************************************************************************************/
#ifndef QEMONITOR_H_FILE
#define QEMONITOR_H_FILE

#include "r_touch_qe_config.h"
#include "qe_common.h"


/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/
/* MONITOR REQUEST (DRIVER RECEIVE) BUFFER
 * The external tool writes to this buffer when it wants to start, stop, or change data to monitor.
 * The first and last byte of the request must always match, and their value must change with each new request.
 * The driver inspects this buffer after each scan completes.
 * If the driver reads the buffer and the first and last bytes do not match, it knows that
 *   the buffer is being written to and no action is taken.
 * If the driver reads the buffer and the first and last bytes match, AND the data type
 *   is not 0 (no data requested), the monitor data buffer is loaded with the requested data.
 * If the flag field has auto-update set, the requested data is also automatically updated
 *   after each scan completes (constant refresh).
 * If the flag field has 2-part request set, a second request in the buffer is processed if its start and end IDs
 *   match the 1st part ID, and if the toggle field differs from the last time a 2-part request was made.
 */
/* REQUEST FLAGS */
#define MON_FLG_READ_REQ         (0x01)  // 1=read request; 0=write request
/* flags valid only with read requests */
#define MON_FLG_AUTO_UPDATE      (0x02)  // 1=update data after each scan; 0=update once
#define MON_FLG_INCL_SLDR_COUNTS (0x04)  // 1=include slider/wheel sensor counts; 0=position only
#define MON_FLG_2_PART_REQ       (0x08)  // 1=request is 1st of two in buf; 0=single req; valid only with Auto Update

/* request data_type */
#define MON_DATA_STOP           (0)     // no data requested; stop updating monitor data buffer
#define MON_DATA_BUTTONS        (1)     // read only
#define MON_DATA_SLIDERS        (2)     // read only sliders and wheels
#define MON_DATA_BTNS_AND_SLDRS (3)     // read only
#define MON_DATA_ACTIVE_METHODS (4)     // read only; method and mask fields unused in this request

/* request buffer constants */
#define MON_MAX_REQUESTS        (2)
#define MON_REQ_MAX_SIZE        (sizeof(mon_req_hdr_t) + sizeof(mon_trailer_t))
#define MON_REQ_BUF_SIZE        (MON_MAX_REQUESTS * MON_REQ_MAX_SIZE)

/* reply data buffer **/
#define MON_REPLY_MAX_SIZE  (sizeof(mon_reply_hdr_t) +                                              \
                             (QE_MAX_BUTTONS * sizeof(mon_btn_t)) + sizeof(uint64_t) +              \
                             (QE_MAX_SLIDERS * (1 + QE_MAX_SLDR_ELEM_USED) * sizeof(uint16_t)) +    \
                             (QE_MAX_WHEELS * (1 + QE_MAX_WHEEL_ELEM_USED) * sizeof(uint16_t)) +    \
                             sizeof(mon_trailer_t))
#define MON_REPLY_BUF_SIZE  (MON_MAX_REQUESTS * MON_REPLY_MAX_SIZE)


/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/
#pragma pack
typedef struct
{
    uint8_t     s_id;           // start id; must match e_id (changes with each request)
    uint8_t     flags;
    uint8_t     method;
    uint8_t     data_type;
    uint64_t    button_mask;    // mask bit numbers correspond to g_buttons_<config>[] indexes
    uint8_t     slider_mask;    // mask bit numbers correspond to g_sliders_<config>[] indexes
    uint8_t     wheel_mask;     // mask bit numbers correspond to g_wheels_<config>[] indexes
} mon_req_hdr_t;
#pragma unpack

#pragma pack
typedef struct
{
    uint8_t     part2_toggle;   // 0 or 1; must alternate in 2nd request/reply each time a 2-part request is made
                                // field ignored in 1st part request/reply
    uint8_t     e_id;           // end id; must match s_id (changes with each request)
} mon_trailer_t;
#pragma unpack


/* MONITOR DATA (DRIVER "TRANSMIT") BUFFER
 * This buffer is loaded with a header, variable length data, and a trailer.
 * The Touch driver writes to this buffer after a scan completes with the data requested
 *   by the monitor request buffer.
 * The first byte of the header always matches the last byte of the trailer, and is incremented
 *   by 1 with each update.
 * If the external tool reads the buffer and the first and last bytes do not match,
 *   the data is in the process of being updated.
 * If the external tool reads the buffer and the first and last bytes have increased by more than 1
 *   (using auto update), the tool has missed an update and may need to shorten its read interval.
 * If the external tool reads the buffer and the first and last bytes are the same as the last read,
 *   the tool has read the same data again and may need to lengthen its read interval.
 */
typedef struct
{
    uint8_t     s_id;           // start id; must match e_id (changes with each request)
    uint8_t     format;         // always 0 (for future changes)
} mon_reply_hdr_t;


/* Button Data
 * The button data (if any) is placed after the buffer header.
 * The structure below is repeated for each button specified in the request mask.
 * The button records are followed by a uint64_t mask as returned by R_TOUCH_GetAllBtnStates();
 *   Note that this is NOT the same mask format as is used by "button_mask" in the monitor request.
 */
typedef struct
{
    uint16_t    reference;
    uint16_t    touch_delta;    // add to reference to get threshold
    uint16_t    primary;
    uint16_t    secondary;      // unused in SELF mode
} mon_btn_t;


/* Slider/Wheel Data
 * Button data (if any) is followed by slider data (if any) which is followed by wheel data (if any).
 * By default, only position data is provided.
 * If MON_FLG_INCL_SLDR_COUNTS is set, sensor counts follow the position in low to high order
 *    (g_sliders_<config>[] elem_index order).
 * This data (if any) is followed by the buffer trailer.
 */


/* Method Data
 * Method data begins with a byte containing the number of actively scanned methods, followed by a
 *    byte list of each active method.
 * This data is preceded by a header and followed by a trailer.
 */


/***********************************************************************************************************************
* Exported global variables
***********************************************************************************************************************/
extern uint8_t  g_mon_req_buf[];
extern uint8_t  g_mon_reply_buf[];


/***********************************************************************************************************************
* Exported global functions
***********************************************************************************************************************/
extern void monitor_init(void);
extern void monitor_update_data(void);

#endif // QEMONITOR_H_FILE
